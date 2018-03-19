#include "core.h"
#include "media.h"

static Nan::Persistent<v8::Function> constructor;

v8::Local<v8::Function> Media::Init() {
  auto tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("VlcMedia").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  auto inst = tpl->InstanceTemplate();
  Nan::SetAccessor(inst, Nan::New("mrl").ToLocalChecked(), MrlGetter);
  Nan::SetAccessor(inst, Nan::New("duration").ToLocalChecked(), DurationGetter);

  constructor.Reset(tpl->GetFunction());
  return tpl->GetFunction();
}


NAN_METHOD(Media::New) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  Nan::Utf8String mrl(info[0]->ToString());

  if (info.IsConstructCall()) {
    auto obj = new Media(GetVlc(), *mrl);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    Nan::ThrowTypeError("Called as plain function is not permitted");
  }
}


Media::Media(libvlc_instance_t* vlc, const char* mrl) {
  m_vlc_media = libvlc_media_new_location(vlc, mrl);
}

Media::~Media() {
  libvlc_media_release(m_vlc_media);
}

NAN_GETTER(Media::MrlGetter) {
  auto self = Nan::ObjectWrap::Unwrap<Media>(info.Holder());
  auto mrl = libvlc_media_get_mrl(self->m_vlc_media);
  info.GetReturnValue().Set(Nan::New((const char*) mrl).ToLocalChecked());
  free(mrl);
}

NAN_GETTER(Media::DurationGetter) {
  auto self = Nan::ObjectWrap::Unwrap<Media>(info.Holder());
  auto val = libvlc_media_get_duration(self->m_vlc_media);
  auto ret = (double) val / 1000.0;
  info.GetReturnValue().Set(Nan::New(ret));
}
