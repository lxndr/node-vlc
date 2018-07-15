const path = require('path');
const {VlcMedia, VlcMediaPlayer} = require('..');

const track1 = 'file://' + path.resolve('__tests__/fixtures/bird-whistling.wav');

let player = null;
let media = null;

describe('VlcMediaPlayer', () => {
  it('creates new', () => {
    player = new VlcMediaPlayer();
  });

  it('sets media', () => {
    player.media = new VlcMedia(track1);
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

  afterAll(() => {
    player.media.close();
    player.close();
  });
});
