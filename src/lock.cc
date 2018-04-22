#include <node.h>
#include <stdio.h>

#include "lock.h"

class LockWorker : public Nan::AsyncWorker {
  public:
  LockWorker(Nan::Callback *callback) : AsyncWorker(callback) {}
  ~LockWorker() {}

  void Execute() {
    uv_mutex_lock(&mutex);
  }

  void HandleOkCallback() {
    Nan::HandleScope scope;

    v8::Local<v8::Value> argv[2] = {
      Nan::Null(),
      Nan::Null()
    };
    callback->Call(2, argv, async_resource);
  }
};

NAN_METHOD(Lock) {
  Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
  Nan::AsyncQueueWorker(new LockWorker(callback));
}

NAN_METHOD(Initialize) {
  uv_mutex_init(&mutex);
}

NAN_METHOD(Unlock) {
  uv_mutex_unlock(&mutex);
}

NAN_METHOD(Shutdown) {
  uv_mutex_destroy(&mutex);
}

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  exports->Set(
    Nan::New("lock").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Lock)).ToLocalChecked()
  );
  exports->Set(
    Nan::New("init").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Initialize)).ToLocalChecked()
  );
  exports->Set(
    Nan::New("unlock").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Unlock)).ToLocalChecked()
  );
  exports->Set(
    Nan::New("shutdown").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Shutdown)).ToLocalChecked()
  );
}

NODE_MODULE(addon, Init)