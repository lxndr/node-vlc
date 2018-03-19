{
  "targets": [{
    "target_name": "vlc",
    "cflags_cc!": ["-fno-rtti"],
    "sources": [
      "src/addon.cc",
      "src/media.cc",
      "src/media-player.cc",
      "src/event-manager.cc",
      "src/event.cc"
    ],
    "libraries": [
      "<!@(pkg-config --libs libvlc)"
    ],
    "include_dirs": [
      "<!(node -e \"require('nan')\")"
    ]
  }]
}
