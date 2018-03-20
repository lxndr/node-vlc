/*
 * Copyright 2018 Humanity
 */

#include "core.h"

static libvlc_instance_t* vlc = nullptr;

libvlc_instance_t* NewVlc() {
  if (!vlc)
    vlc = libvlc_new(0, NULL);
  return vlc;
}

libvlc_instance_t* GetVlc() {
  return vlc;
}
