/*
 * Copyright 2018 Humanity
 */

#include "event.h"

Event* Event::create(const libvlc_event_t* event) {
  return new Event();
}

v8::Local<v8::Value> Event::value() const {
  return Nan::Undefined();
}

Event* BufferEvent::create(const libvlc_event_t* event) {
  auto r = new BufferEvent();
  r->cache = event->u.media_player_buffering.new_cache;
  return r;
}

v8::Local<v8::Value> BufferEvent::value() const {
  return Nan::New(cache);
}

Event* LengthChangeEvent::create(const libvlc_event_t* event) {
  auto r = new LengthChangeEvent();
  r->length = event->u.media_player_length_changed.new_length;
  return r;
}

v8::Local<v8::Value> LengthChangeEvent::value() const {
  return Nan::New(static_cast<int32_t>(length));
}

Event* TimeChangeEvent::create(const libvlc_event_t* event) {
  auto r = new TimeChangeEvent();
  r->time = event->u.media_player_time_changed.new_time;
  return r;
}

v8::Local<v8::Value> TimeChangeEvent::value() const {
  return Nan::New(static_cast<int32_t>(time));
}

Event* PositionChangeEvent::create(const libvlc_event_t* event) {
  auto r = new PositionChangeEvent();
  r->position = event->u.media_player_position_changed.new_position;
  return r;
}

v8::Local<v8::Value> PositionChangeEvent::value() const {
  return Nan::New(position);
}

Event* MetaChangeEvent::create(const libvlc_event_t* event) {
  auto r = new MetaChangeEvent();
  r->type = event->u.media_meta_changed.meta_type;
  return r;
}

v8::Local<v8::Value> MetaChangeEvent::value() const {
  return Nan::New(type);
}

Event* DurationChangeEvent::create(const libvlc_event_t* event) {
  auto r = new DurationChangeEvent();
  r->duration = event->u.media_duration_changed.new_duration;
  return r;
}

v8::Local<v8::Value> DurationChangeEvent::value() const {
  return Nan::New(static_cast<double>(duration));
}
