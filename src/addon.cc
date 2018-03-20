/*
 * Copyright 2018 Humanity
 */

#include "core.h"
#include "media.h"
#include "media-player.h"

void ExportClass(v8::Local<v8::Object> target, const char* name, v8::Local<v8::Function> constructor) {
  Nan::Set(target, Nan::New(name).ToLocalChecked(), constructor);
}

NAN_MODULE_INIT(Init) {
  NewVlc();

  ExportClass(target, "VlcMedia", Media::Init());
  ExportClass(target, "VlcMediaPlayer", MediaPlayer::Init());
}

NODE_MODULE(addon, Init)
