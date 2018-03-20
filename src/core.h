/*
 * Copyright 2018 Humanity
 */

#ifndef SRC_CORE_H_
#define SRC_CORE_H_

#include <vlc/vlc.h>

libvlc_instance_t* NewVlc();
libvlc_instance_t* GetVlc();

#endif  // SRC_CORE_H_
