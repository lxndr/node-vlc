const path = require('path');
const {expect} = require('chai');
const {VlcMedia} = require('..');

const fname = 'file://' + path.resolve('tests/fixtures/bird-whistling.wav');

describe('VlcMedia', () => {
  let media = null;

  it ('creates new', () => {
    media = new VlcMedia(fname);
  });

  after(() => {
    media.close();
  });
});
