const path = require('path');
const {EventEmitter} = require('events');
const {expect} = require('chai');
const {VlcMedia} = require('..');

const fname = 'file://' + path.resolve('tests/fixtures/bird-whistling.wav');

describe('VlcMedia', () => {
  let media = null;

  it ('creates new', () => {
    media = new VlcMedia(fname);
  });

  it('inherits EventEmitter', () => {
    expect(media instanceof EventEmitter).to.be.true;
  });

  after(() => {
    media.close();
  });
});
