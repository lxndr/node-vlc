#include "event-manager.h"

typedef Event* (*CreateEventFn) (const libvlc_event_t* event);

struct EventDesc {
  int type;
  std::string name;
  CreateEventFn createEvent;

  bool operator==(int val)
    {return type == val;}

  bool operator==(const char* val)
    {return name == val;}
};

static std::vector<EventDesc> eventMap = {
  {libvlc_MediaPlayerOpening,          "opening",          Event::create},
  {libvlc_MediaPlayerBuffering,        "buffering",        BufferingEvent::create},
  {libvlc_MediaPlayerPlaying,          "playing",          Event::create},
  {libvlc_MediaPlayerPaused,           "pause",            Event::create},
  {libvlc_MediaPlayerStopped,          "stop",             Event::create},

  {libvlc_MediaPlayerTimeChanged,      "time-changed",     TimeChangedEvent::create},
  {libvlc_MediaPlayerPositionChanged,  "position-changed", PositionChangedEvent::create},
  {libvlc_MediaPlayerEndReached,       "end",              Event::create},
  {libvlc_MediaPlayerEncounteredError, "error",            Event::create},
};

Event* Event::create(const libvlc_event_t* event) {
  return new Event();
}

v8::Local<v8::Value> Event::value() const {
  return Nan::Undefined();
}

Event* BufferingEvent::create(const libvlc_event_t* event) {
  auto r = new BufferingEvent();
  r->cache = event->u.media_player_buffering.new_cache;
  return r;
}

v8::Local<v8::Value> BufferingEvent::value() const {
  return Nan::New(cache);
}

Event* TimeChangedEvent::create(const libvlc_event_t* event) {
  auto r = new TimeChangedEvent();
  r->time = event->u.media_player_time_changed.new_time;
  return r;
}

v8::Local<v8::Value> TimeChangedEvent::value() const {
  return Nan::New(static_cast<int32_t>(time));
}

Event* PositionChangedEvent::create(const libvlc_event_t* event) {
  auto r = new PositionChangedEvent();
  r->position = event->u.media_player_position_changed.new_position;
  return r;
}

v8::Local<v8::Value> PositionChangedEvent::value() const {
  return Nan::New(position);
}

EventManager::EventManager() : m_closed(false) {
  uv_async_init(uv_default_loop(), &m_async, async_cb);
  m_async.data = this;
}

EventManager::~EventManager() {
  Destroy();
}

void EventManager::SetupEventCallbacks(libvlc_event_manager_t* em) {
  for (auto& desc : eventMap)
    libvlc_event_attach(em, desc.type, reinterpret_cast<libvlc_callback_t>(event_cb), this);
}

void EventManager::Emit(const std::string& name, const v8::Local<v8::Value>& arg) {
  auto fn = v8::Local<v8::Function>::Cast(
    handle()->Get(Nan::New("emit").ToLocalChecked())
  );

  if (arg->IsUndefined()) {
    v8::Local<v8::Value> args[] = {Nan::New(name).ToLocalChecked()};
    Nan::Call(fn, handle(), 1, args);
  } else {
    v8::Local<v8::Value> args[] = {Nan::New(name).ToLocalChecked(), arg};
    Nan::Call(fn, handle(), 2, args);
  }
}

void EventManager::Destroy() {
  if (m_closed)
    return;

  m_closed = true;
  uv_close(reinterpret_cast<uv_handle_t*>(&m_async), nullptr);
}

void EventManager::event_cb(const libvlc_event_t* p_event, EventManager* self) {
  auto type = p_event->type;

  auto it = std::find(eventMap.begin(), eventMap.end(), type);
  auto event = it->createEvent(p_event);
  event->name = it->name;

  self->m_mutex.lock();
  self->m_events.emplace_back(event);
  self->m_mutex.unlock();

  uv_async_send(&self->m_async);
}

void EventManager::async_cb(uv_async_t* handle) {
  auto self = reinterpret_cast<EventManager*>(handle->data);
  Nan::HandleScope scope;

  self->m_mutex.lock();
  for (const auto& event : self->m_events) {
    self->Emit(event->name, event->value());
  }
  self->m_events.clear();
  self->m_mutex.unlock();
}
