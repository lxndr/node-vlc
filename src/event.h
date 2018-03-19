#pragma once

#include <nan.h>
#include <vlc/vlc.h>

struct Event {
  std::string name;

  static Event* create(const libvlc_event_t* event);
  virtual v8::Local<v8::Value> value() const;
};

struct BufferingEvent : Event {
  float cache;

  static Event* create(const libvlc_event_t* event);
  v8::Local<v8::Value> value() const override;
};

struct TimeChangedEvent : Event {
  libvlc_time_t time;

  static Event* create(const libvlc_event_t* event);
  v8::Local<v8::Value> value() const override;
};

struct PositionChangedEvent : Event {
  float position;

  static Event* create(const libvlc_event_t* event);
  v8::Local<v8::Value> value() const override;
};
