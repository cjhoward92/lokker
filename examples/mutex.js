const locker = require('..');

const mut = new locker.Mutex();

const first = async () => {
  console.log('First is going to lock');
  await mut.lock();

  console.log('First has locked');

  setTimeout(() => {
    console.log('First is unlocking');
    mut.unlock();
  }, 3000);
};

const second = async () => {
  console.log('Second is going to lock');
  await mut.lock();

  console.log('Second has finished and will unlock');
  mut.unlock();
};

first()
  .then(second)
  .then(() => console.log('We are done!'));