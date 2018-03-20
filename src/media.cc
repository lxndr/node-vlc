#include "core.h"
#include "media.h"

static Nan::Persistent<v8::Function> constructor;
static const std::vector<std::string> availableEvents = {
  "onmeta", "onduration"
};


v8::Local<v8::Function> Media::Init() {
  auto tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("VlcMedia").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "getMeta", GetMeta);

  auto inst = tpl->InstanceTemplate();
  Nan::SetAccessor(inst, Nan::New("mrl").ToLocalChecked(), MrlGetter);
  Nan::SetAccessor(inst, Nan::New("duration").ToLocalChecked(), DurationGetter);

  Object::Init(tpl, availableEvents);

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


Media::Media(libvlc_instance_t* vlc, const char* mrl) : Object(availableEvents) {
  m_vlc_media = libvlc_media_new_location(vlc, mrl);
}

libvlc_event_manager_t* Media::GetEventManager() const {
  return libvlc_media_event_manager(m_vlc_media);
}

void Media::OnClose() {
  libvlc_media_release(m_vlc_media);
}

NAN_METHOD(Media::GetMeta) {
  if (!(info.Length() >= 1 && info[0]->IsNumber())) {
    Nan::ThrowTypeError("Argument #1 must be a number");
    return;
  }

  auto self = Nan::ObjectWrap::Unwrap<Media>(info.Holder());
  auto type = info[0]->Int32Value();
  auto meta = libvlc_media_get_meta(self->m_vlc_media, (libvlc_meta_t) type);

  if (meta) {
    info.GetReturnValue().Set(Nan::New((const char*) meta).ToLocalChecked());
    libvlc_free(meta);
  } else {
    info.GetReturnValue().SetNull();
  }
}

NAN_GETTER(Media::MrlGetter) {
  auto self = Nan::ObjectWrap::Unwrap<Media>(info.Holder());
  auto mrl = libvlc_media_get_mrl(self->m_vlc_media);
  info.GetReturnValue().Set(Nan::New((const char*) mrl).ToLocalChecked());
  libvlc_free(mrl);
}

NAN_GETTER(Media::DurationGetter) {
  auto self = Nan::ObjectWrap::Unwrap<Media>(info.Holder());
  auto val = libvlc_media_get_duration(self->m_vlc_media);
  info.GetReturnValue().Set((int32_t) val);
}
