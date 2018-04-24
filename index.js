const util = require('util');

const libpath = process.env.DEBUG
  ? './build/Debug/addon.node'
  : './build/Release/addon.node';

const addon = require(libpath);

const Mutex = addon.Mutex;
const _lock = Mutex.prototype.lock;
Mutex.prototype.lock = util.promisify(_lock);

module.exports = {
  Mutex
};
