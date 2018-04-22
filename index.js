const util = require('util');

const addon = require('./build/Release/addon.node');


const Mutex = addon.Mutex;
const _lock = Mutex.prototype.lock;
Mutex.prototype.lock = util.promisify(_lock);

module.exports = {
  Mutex
};
