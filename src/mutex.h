#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <nan.h>
#include <node.h>
#include <uv.h>

using namespace Nan;

class LockWorker : public AsyncWorker {
  public:
  LockWorker(Nan::Callback *callback, uv_mutex_t *mutex);
  ~LockWorker();

  void Execute();
  void HandleOkCallback();

  private:
  uv_mutex_t *_mutex;
};

class Mutex : public ObjectWrap {
  public:
  static NAN_MODULE_INIT(Init);

  private:
  explicit Mutex();
  ~Mutex();

  static NAN_METHOD(New);
  static NAN_METHOD(Lock);
  static NAN_METHOD(Unlock);

  static Persistent<v8::Function> constructor;
  uv_mutex_t _mutex;
};

#endif