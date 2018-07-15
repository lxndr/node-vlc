const path = require('path');
const {VlcMedia} = require('..');

const fname = 'file://' + path.resolve('__tests__/fixtures/bird-whistling.wav');

let media = null;

describe('VlcMedia', () => {
  it('creates new', () => {
    media = new VlcMedia(fname);
  });

  it('sets a callback', () => {
    media.onmeta = () => {};
    expect(typeof media.onmeta === 'function').toBe(true);
  });

  it('unsets a callback', () => {
    media.onmeta = null;
    expect(media.onmeta).toBe(undefined);
  });

  afterAll(() => {
    media.close();
  });
});
