#pragma once

#include <uv.h>
#include <mutex>
#include <nan.h>

struct Event;

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
