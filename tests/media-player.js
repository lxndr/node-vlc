const path = require('path');
const {expect} = require('chai');
const {VlcMedia, VlcMediaPlayer} = require('..');

const track1 = 'file://' + path.resolve('tests/fixtures/bird-whistling.wav');

describe('VlcMediaPlayer', () => {
  let player = null;
  let media = null;

  it('creates new', () => {
    player = new VlcMediaPlayer();
  });

  it('sets media', () => {
    media = new VlcMedia(track1);
    player.media = media;
    player.media = null;
    player.media = media;
  });

  it('plays', done => {
    player.onplay = () => {
      done();
    };

    player.onerror = error => {
      done(error);
    };

    player.play();
  });

  it('pauses', done => {
    player.onpause = () => {
      done();
    };

    player.onerror = error => {
      done(error);
    };

    player.pause();
  });

  it('stops', done => {
    player.onstop = () => {
      done();
    };

    player.onerror = error => {
      done(error);
    };

    player.stop();
  });

  after(() => {
    player.media.close();
    player.close();
  });
});
