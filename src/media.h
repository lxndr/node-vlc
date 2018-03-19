#pragma once

#include <uv.h>
#include <nan.h>
#include <vlc/vlc.h>

class Media : public Nan::ObjectWrap {
public:
  static v8::Local<v8::Function> Init();

  inline libvlc_media_t* GetVlcMedia() const
    {return m_vlc_media;}

private:
  explicit Media(libvlc_instance_t* vlc, const char* mrl);
  ~Media();

  static NAN_METHOD(New);
  static NAN_GETTER(MrlGetter);
  static NAN_GETTER(DurationGetter);

private:
  libvlc_media_t* m_vlc_media;
};
