const {VlcMedia, VlcMediaPlayer} = require('bindings')('vlc');
const {EventEmitter} = require('events');
const {inherits} = require('util');

// inherits(VlcMedia, EventEmitter);
inherits(VlcMediaPlayer, EventEmitter);

module.exports = {VlcMedia, VlcMediaPlayer};
