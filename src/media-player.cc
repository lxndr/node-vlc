/*
 * Copyright 2018 Humanity
 */

#include <vector>
#include <string>
#include "core.h"
#include "media.h"
#include "media-player.h"

static Nan::Persistent<v8::Function> constructor;
static const std::vector<std::string> availableEvents = {
  "onopen", "onbuffer", "onplay", "onpause", "onstop",
  "ontime", "onposition", "onend", "onerror"
};


v8::Local<v8::Function> MediaPlayer::Init() {
  auto tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("VlcMediaPlayer").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "play", Play);
  Nan::SetPrototypeMethod(tpl, "pause", Pause);
  Nan::SetPrototypeMethod(tpl, "stop", Stop);

  auto inst = tpl->InstanceTemplate();
  Nan::SetAccessor(inst, Nan::New("media").ToLocalChecked(), MediaGetter, MediaSetter);
  Nan::SetAccessor(inst, Nan::New("length").ToLocalChecked(), LengthGetter);
  Nan::SetAccessor(inst, Nan::New("time").ToLocalChecked(), TimeGetter, TimeSetter);
  Nan::SetAccessor(inst, Nan::New("position").ToLocalChecked(), PositionGetter, PositionSetter);
  Nan::SetAccessor(inst, Nan::New("state").ToLocalChecked(), StateGetter);

  Object::Init(tpl, availableEvents);

  constructor.Reset(tpl->GetFunction());
  return tpl->GetFunction();
}


NAN_METHOD(MediaPlayer::New) {
  if (info.IsConstructCall()) {
    auto self = new MediaPlayer(GetVlc());
    self->Wrap(info.This());

    if (info.Length() > 0) {
      auto media = info[0]->ToObject();
      self->SetMedia(media);
    }

    info.GetReturnValue().Set(info.This());
  } else {
    Nan::ThrowTypeError("Called as plain function is not permitted");
  }
}


MediaPlayer::MediaPlayer(libvlc_instance_t* vlc) : Object(availableEvents) {
  m_vlc_player = libvlc_media_player_new(vlc);
}

libvlc_event_manager_t* MediaPlayer::GetEventManager() const {
  return libvlc_media_player_event_manager(m_vlc_player);
}

void MediaPlayer::OnClose() {
  libvlc_media_player_stop(m_vlc_player);
  libvlc_media_player_release(m_vlc_player);
}

void MediaPlayer::SetMedia(v8::Local<v8::Value> value) {
  if (value.IsEmpty() || value->IsNullOrUndefined()) {
    libvlc_media_player_set_media(m_vlc_player, nullptr);
    m_media.Reset();
  } else if (value->IsObject()) {
    auto media = Nan::ObjectWrap::Unwrap<Media>(value->ToObject());
    libvlc_media_player_set_media(m_vlc_player, media->VlcHandle());
    m_media.Reset(value);
  } else {
    Nan::ThrowTypeError("Argument must be a VlcMedia");
  }
}


NAN_METHOD(MediaPlayer::Play) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());
  libvlc_media_player_play(self->m_vlc_player);
}

NAN_METHOD(MediaPlayer::Pause) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());
  libvlc_media_player_pause(self->m_vlc_player);
}

NAN_METHOD(MediaPlayer::Stop) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());

  /*
   * NOTE: libvlc definitely freezes calling libvlc_media_player_stop
   * when current state is libvlc_Ended
   */
  auto state = libvlc_media_player_get_state(self->m_vlc_player);
  if (state == libvlc_Stopped || state == libvlc_Ended)
    return;

  libvlc_media_player_stop(self->m_vlc_player);
}


NAN_GETTER(MediaPlayer::MediaGetter) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());
  info.GetReturnValue().Set(Nan::New(self->m_media));
}

NAN_SETTER(MediaPlayer::MediaSetter) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());
  self->SetMedia(value);
  info.GetReturnValue().Set(true);
}

NAN_GETTER(MediaPlayer::LengthGetter) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());
  auto length = libvlc_media_player_get_length(self->m_vlc_player);
  info.GetReturnValue().Set(static_cast<int32_t>(length));
}

NAN_GETTER(MediaPlayer::TimeGetter) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());
  auto time = libvlc_media_player_get_time(self->m_vlc_player);
  info.GetReturnValue().Set(static_cast<int32_t>(time));
}

NAN_SETTER(MediaPlayer::TimeSetter) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());
  auto time = static_cast<libvlc_time_t>(value->IntegerValue());
  libvlc_media_player_set_time(self->m_vlc_player, time);
  info.GetReturnValue().Set(true);
}

NAN_GETTER(MediaPlayer::PositionGetter) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());
  auto pos = libvlc_media_player_get_position(self->m_vlc_player);
  info.GetReturnValue().Set(pos);
}

NAN_SETTER(MediaPlayer::PositionSetter) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());
  auto pos = static_cast<float>(value->NumberValue());
  libvlc_media_player_set_position(self->m_vlc_player, pos);
  info.GetReturnValue().Set(true);
}

NAN_GETTER(MediaPlayer::StateGetter) {
  auto self = Nan::ObjectWrap::Unwrap<MediaPlayer>(info.Holder());
  auto state = libvlc_media_player_get_state(self->m_vlc_player);
  info.GetReturnValue().Set(state);
}
