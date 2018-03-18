const path = require('path');
const {EventEmitter} = require('events');
const {expect} = require('chai');
const {VlcMedia, VlcMediaPlayer} = require('..');

describe('VlcMediaPlayer', () => {
  let player = null;

  it('creates new', () => {
    const fname = 'file://' + path.resolve('tests/fixtures/bird-whistling.wav');
    const media = new VlcMedia(fname);
    player = new VlcMediaPlayer(media);
    expect(player instanceof VlcMediaPlayer).to.be.true;
  });

  it('inherits EventEmitter', () => {
    expect(player instanceof EventEmitter).to.be.true;
  });

  it('plays', done => {
    player.removeAllListeners();

    player.on('playing', () => {
      done();
    });

    player.on('error', error => {
      done(error);
    });

    player.play();
  });

  it('pauses', done => {
    player.removeAllListeners();

    player.on('paused', () => {
      done();
    });

    player.on('error', error => {
      done(error);
    });

    player.pause();
  });

  it('stops', done => {
    player.removeAllListeners();

    player.on('stopped', () => {
      done();
    });

    player.on('error', error => {
      done(error);
    });

    player.stop();
  });

  after(() => {
    player.close();
  });
});
