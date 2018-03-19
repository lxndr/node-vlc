#pragma once

#include <map>
#include <mutex>
#include <uv.h>
#include <nan.h>

struct Event;

class EventManager : public Nan::ObjectWrap {
public:
  static void Init(v8::Local<v8::FunctionTemplate> tpl, const std::vector<std::string>& availableEvents);

  EventManager(const std::vector<std::string>& availableEvents);
  virtual ~EventManager();
  virtual void Close();

protected:
  virtual libvlc_event_manager_t* GetVlcEventManager() const = 0;
  void Emit(const std::string& name, v8::Local<v8::Value> arg);

private:
  static void event_cb(const libvlc_event_t* p_event, EventManager* self);
  static void async_cb(uv_async_t* handle);

  static NAN_GETTER(CallbackGetter);
  static NAN_SETTER(CallbackSetter);

  bool m_closed;
  uv_async_t m_async;
  std::mutex m_mutex;
  std::deque<std::unique_ptr<Event>> m_events;
  std::map<std::string, Nan::Callback> m_callbacks;
};
