/*
 * Copyright 2018 Humanity
 */

#include "event.h"
#include "object.h"

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
  {libvlc_MediaMetaChanged,            "onmeta",     MetaChangeEvent::create    },

  {libvlc_MediaPlayerOpening,          "onopen",     Event::create              },
  {libvlc_MediaPlayerBuffering,        "onbuffer",   BufferEvent::create        },
  {libvlc_MediaPlayerPlaying,          "onplay",     Event::create              },
  {libvlc_MediaPlayerPaused,           "onpause",    Event::create              },
  {libvlc_MediaPlayerStopped,          "onstop",     Event::create              },

  {libvlc_MediaPlayerTimeChanged,      "ontime",     TimeChangeEvent::create    },
  {libvlc_MediaPlayerPositionChanged,  "onposition", PositionChangeEvent::create},
  {libvlc_MediaPlayerEndReached,       "onend",      Event::create              },
  {libvlc_MediaPlayerEncounteredError, "onerror",    Event::create              },
};

void Object::Init(v8::Local<v8::FunctionTemplate> tpl, const std::vector<std::string>& availableEvents) {
  Nan::SetPrototypeMethod(tpl, "close", Close);

  auto inst = tpl->InstanceTemplate();
  for (auto& name : availableEvents)
    Nan::SetAccessor(inst, Nan::New(name).ToLocalChecked(), CallbackGetter, CallbackSetter);
}

Object::Object(const std::vector<std::string>& availableEvents) : m_closed(false) {
  for (auto& name : availableEvents)
    m_callbacks.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple());

  uv_async_init(uv_default_loop(), &m_async, async_cb);
  m_async.data = this;
}

Object::~Object() {
  Close();
}

void Object::Emit(const std::string& name, v8::Local<v8::Value> arg) {
  v8::Local<v8::Value> argv[] = {arg};
  int argc = arg->IsUndefined() ? 0 : 1;
  Nan::Call(m_callbacks[name], handle(), argc, argv);
}

void Object::Close() {
  if (m_closed)
    return;

  m_closed = true;
  uv_close(reinterpret_cast<uv_handle_t*>(&m_async), nullptr);

  OnClose();
}

void Object::event_cb(const libvlc_event_t* p_event, Object* self) {
  auto type = p_event->type;

  auto it = std::find(eventMap.begin(), eventMap.end(), type);
  auto event = it->createEvent(p_event);
  event->name = it->name;

  self->m_mutex.lock();
  self->m_events.emplace_back(event);
  self->m_mutex.unlock();

  uv_async_send(&self->m_async);
}

void Object::async_cb(uv_async_t* handle) {
  auto self = reinterpret_cast<Object*>(handle->data);
  Nan::HandleScope scope;

  self->m_mutex.lock();
  for (const auto& event : self->m_events)
    self->Emit(event->name, event->value());
  self->m_events.clear();
  self->m_mutex.unlock();
}

NAN_METHOD(Object::Close) {
  auto self = Nan::ObjectWrap::Unwrap<Object>(info.Holder());
  self->Close();
}

NAN_GETTER(Object::CallbackGetter) {
  Nan::Utf8String name(property);
  auto self = Nan::ObjectWrap::Unwrap<Object>(info.Holder());
  auto fn = self->m_callbacks[*name].GetFunction();
  info.GetReturnValue().Set(fn);
}

NAN_SETTER(Object::CallbackSetter) {
  Nan::Utf8String name(property);
  auto self = Nan::ObjectWrap::Unwrap<Object>(info.Holder());
  auto em = self->GetEventManager();
  auto idesc = std::find(eventMap.begin(), eventMap.end(), *name);

  if (value->IsFunction()) {
    auto fn = value.As<v8::Function>();
    self->m_callbacks[*name].Reset(fn);
    libvlc_event_attach(em, idesc->type, (libvlc_callback_t) event_cb, self);
  } else if (value->IsNullOrUndefined()) {
    self->m_callbacks[*name].Reset();
    libvlc_event_detach(em, idesc->type, (libvlc_callback_t) event_cb, self);
  } else {
    Nan::ThrowTypeError("Callback must be a function");
  }

  info.GetReturnValue().Set(true);
}
