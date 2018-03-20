/*
 * Copyright 2018 Humanity
 */

#ifndef SRC_OBJECT_H_
#define SRC_OBJECT_H_

#include <uv.h>
#include <nan.h>
#include <map>
#include <mutex>
#include <memory>
#include <string>
#include <vector>
#include <deque>

struct Event;

class Object : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::FunctionTemplate> tpl,
    const std::vector<std::string>& availableEvents);

 protected:
  explicit Object(const std::vector<std::string>& availableEvents);
  virtual ~Object();
  virtual libvlc_event_manager_t* GetEventManager() const = 0;
  virtual void OnClose() = 0;
  inline bool IsClosed() {return m_closed;}

 private:
  static void event_cb(const libvlc_event_t* p_event, Object* self);
  static void async_cb(uv_async_t* handle);

  static NAN_METHOD(Close);
  static NAN_GETTER(CallbackGetter);
  static NAN_SETTER(CallbackSetter);

  void ResetCallback(const std::string& name, v8::Local<v8::Value> fn = v8::Local<v8::Value>());
  void Emit(const std::string& name, v8::Local<v8::Value> arg);
  void Close();

  bool m_closed;
  uv_async_t m_async;
  std::mutex m_mutex;
  std::deque<std::unique_ptr<Event>> m_events;
  std::map<std::string, Nan::Callback> m_callbacks;
};

#endif  // SRC_OBJECT_H_
