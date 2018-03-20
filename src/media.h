/*
 * Copyright 2018 Humanity
 */

#ifndef SRC_MEDIA_H_
#define SRC_MEDIA_H_

#include <uv.h>
#include <nan.h>
#include <vlc/vlc.h>
#include "object.h"

class Media : public Object {
 public:
  static v8::Local<v8::Function> Init();

  inline libvlc_media_t* GetVlcMedia() const
    {return m_vlc_media;}

 private:
  explicit Media(libvlc_instance_t* vlc, const char* mrl);
  libvlc_event_manager_t* GetEventManager() const override;
  void OnClose() override;

  static NAN_METHOD(New);
  static NAN_METHOD(Close);
  static NAN_METHOD(GetMeta);

  static NAN_GETTER(MrlGetter);
  static NAN_GETTER(DurationGetter);

 private:
  libvlc_media_t* m_vlc_media;
};

#endif  // SRC_MEDIA_H_
