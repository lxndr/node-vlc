#include "event.h"

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
