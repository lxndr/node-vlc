const path = require('path');
const {expect} = require('chai');
const {VlcMedia} = require('..');

const fname = 'file://' + path.resolve('tests/fixtures/bird-whistling.wav');

describe('VlcMedia', () => {
  let media = null;

  it('creates new', () => {
    media = new VlcMedia(fname);
  });

  it('sets a callback', () => {
    media.onmeta = () => {};
    media.onmeta = ()  => {}; // set twice
    expect(typeof media.onmeta === 'function').to.be.true;
  });

  it('unsets a callback', () => {
    media.onmeta = null;
    expect(media.onmeta).to.be.undefined;
  });

  after(() => {
    media.close();
  });
});
