#ifndef __LOKKER_MUTEX_H__
#define __LOKKER_MUTEX_H__

#include <nan.h>
#include <node.h>
#include <uv.h>

using namespace Nan;

namespace Lokker {

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
  static void Init(v8::Local<v8::Object>);

  private:
  explicit Mutex(bool recursive);
  ~Mutex();

  static NAN_METHOD(New);
  static NAN_METHOD(Lock);
  static NAN_METHOD(Unlock);

  static Persistent<v8::Function> constructor;
  uv_mutex_t _mutex;
  bool _recursive;
};

}
#endif