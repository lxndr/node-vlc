/*
 * Copyright 2018 Humanity
 */

#ifndef SRC_MEDIA_PLAYER_H_
#define SRC_MEDIA_PLAYER_H_

#include <uv.h>
#include <nan.h>
#include <vlc/vlc.h>
#include "object.h"

class MediaPlayer : public Object {
 public:
  static v8::Local<v8::Function> Init();
  void SetMedia(v8::Local<v8::Value> value);

 private:
  explicit MediaPlayer(libvlc_instance_t* vlc);
  libvlc_event_manager_t* GetEventManager() const override;
  void OnClose() override;

  static NAN_METHOD(New);
  static NAN_METHOD(Play);
  static NAN_METHOD(Pause);
  static NAN_METHOD(Stop);

  static NAN_GETTER(MediaGetter);
  static NAN_SETTER(MediaSetter);
  static NAN_GETTER(LengthGetter);
  static NAN_GETTER(TimeGetter);
  static NAN_SETTER(TimeSetter);
  static NAN_GETTER(PositionGetter);
  static NAN_SETTER(PositionSetter);
  static NAN_GETTER(StateGetter);

 private:
  libvlc_media_player_t* m_vlc_player;
  Nan::Persistent<v8::Value> m_media;
};

#endif  // SRC_MEDIA_PLAYER_H_
