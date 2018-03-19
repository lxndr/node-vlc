const path = require('path');
const {expect} = require('chai');
const {VlcMedia, VlcMediaPlayer} = require('..');

describe('VlcMediaPlayer', () => {
  let player = null;

  it('creates new', () => {
    const fname = 'file://' + path.resolve('tests/fixtures/bird-whistling.wav');
    const media = new VlcMedia(fname);
    player = new VlcMediaPlayer(media);
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
