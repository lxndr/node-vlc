#include "media.h"
#include "media-player.h"

NAN_MODULE_INIT(Init) {
  libvlc_instance_t* vlc = libvlc_new(0, NULL);

  Media::Init(target, vlc);
  MediaPlayer::Init(target, vlc);
}

NODE_MODULE(addon, Init)
