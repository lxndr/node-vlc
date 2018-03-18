#include <vlc/vlc.h>

const char* vlc_state_to_string(libvlc_state_t state) {
  switch (state) {
  case libvlc_NothingSpecial:
    return "idle";
  case libvlc_Opening:
    return "opening";
  case libvlc_Buffering:
    return "buffering";
  case libvlc_Playing:
    return "playing";
  case libvlc_Paused:
    return "paused";
  case libvlc_Stopped:
    return "stopped";
  case libvlc_Ended:
    return "ended";
  case libvlc_Error:
    return "error";
  default:
    return "";
  }
}
