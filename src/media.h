#pragma once

#include <uv.h>
#include <nan.h>
#include <vlc/vlc.h>
#include "event-manager.h"

class Media : public EventManager {
public:
  static v8::Local<v8::Function> Init();
  void Close() override;

  inline libvlc_media_t* GetVlcMedia() const
    {return m_vlc_media;}

private:
  explicit Media(libvlc_instance_t* vlc, const char* mrl);
  libvlc_event_manager_t* GetVlcEventManager() const override;

  static NAN_METHOD(New);
  static NAN_METHOD(Close);
  static NAN_METHOD(GetMeta);

  static NAN_GETTER(MrlGetter);
  static NAN_GETTER(DurationGetter);

private:
  libvlc_media_t* m_vlc_media;
};
