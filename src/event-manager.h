#pragma once

#include <uv.h>
#include <mutex>
#include <nan.h>
#include <vlc/vlc.h>

class EventManager;

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

class EventManager : public Nan::ObjectWrap {
public:
  EventManager();
  virtual ~EventManager();

  void SetupEventCallbacks(libvlc_event_manager_t* em);
  void Emit(const std::string& name, const v8::Local<v8::Value>& arg);
  void Destroy();

private:
  static void event_cb(const libvlc_event_t* p_event, EventManager* self);
  static void async_cb(uv_async_t* handle);

  bool m_closed;
  uv_async_t m_async;
  std::mutex m_mutex;
  std::deque<std::unique_ptr<Event>> m_events;
};
