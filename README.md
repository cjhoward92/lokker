# Lokker

A simple library to facilitate locking from node.js.

### Why do I need locks if node only has a single thread?

While it is true that node itself only allows utilizes a single thread, it is also true that native node modules can spawn their own threads, and when this happens it is sometimes useful to have the ability to spawn and control locks!

### Why wouldn't I just lock from the native code?

You could! No one can tell you where you should implement your locks. However, sometimes you may need a 3rd party library that uses native node modules. This library may change some shared resource, and the library may not utilize any locks, so this leaves you in a sticky place!

### Can't I just chain promises to verify that they occur in the order I wish?

Sure, but that can be 1) confusing and 2) ugly to do in a project, especially as it grows in size! Why not just use the age old locking construct? Locks exist in most languages to some extent, are pretty well understood, and don't really vary in terms of how they function. That being said, you can manage thread safety in any way you see fit.

## Example usage

See `examples` for more in-depth examples of how to use the code.

Mutex example:
```javascript
const lokker = require('lokker');

const mutex = new lokker.Mutex();

const someFunc1 = async () => {
  // ... do some work
  await mutex.lock();
  // mutate some resources
  mutex.unlock();
};

const someFunc2 = async () => {
  // ... do some work
  await mutex.lock();
  // mutate the same stuff as someFunc2
  mutex.unlock();
};

someFunc1().then(someFunc2).then(() => console.log('We are done doing stuff!'));
```