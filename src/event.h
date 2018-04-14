/*
 * Copyright 2018 Humanity
 */

#ifndef SRC_EVENT_H_
#define SRC_EVENT_H_

#include <nan.h>
#include <vlc/vlc.h>
#include <string>

struct Event {
  std::string name;

  static Event* create(const libvlc_event_t* event);
  virtual v8::Local<v8::Value> value() const;
};

struct BufferEvent : Event {
  float cache;

  static Event* create(const libvlc_event_t* event);
  v8::Local<v8::Value> value() const override;
};

struct LengthChangeEvent : Event {
  libvlc_time_t length;

  static Event* create(const libvlc_event_t* event);
  v8::Local<v8::Value> value() const override;
};

struct TimeChangeEvent : Event {
  libvlc_time_t time;

  static Event* create(const libvlc_event_t* event);
  v8::Local<v8::Value> value() const override;
};

struct PositionChangeEvent : Event {
  float position;

  static Event* create(const libvlc_event_t* event);
  v8::Local<v8::Value> value() const override;
};

struct MetaChangeEvent : Event {
  int type;

  static Event* create(const libvlc_event_t* event);
  v8::Local<v8::Value> value() const override;
};

struct DurationChangeEvent : Event {
  int64_t duration;

  static Event* create(const libvlc_event_t* event);
  v8::Local<v8::Value> value() const override;
};

#endif  // SRC_EVENT_H_
