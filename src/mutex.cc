#include "mutex.h"

namespace Lokker {

LockWorker::LockWorker(Nan::Callback *callback, uv_mutex_t *mutex)
  : AsyncWorker(callback) {
    _mutex = mutex;
  }

LockWorker::~LockWorker() {}

void LockWorker::Execute() {
  uv_mutex_lock(_mutex);
}

void LockWorker::HandleOkCallback() {
  Nan::HandleScope scope;

  v8::Local<v8::Value> argv[2] = {
    Nan::Null(),
    Nan::Null()
  };
  callback->Call(2, argv, async_resource);
}

Mutex::Mutex(bool recursive) {
  _recursive = recursive;

  int err;
  if (_recursive) {
    printf("Using recursive\n");
    err = uv_mutex_init_recursive(&_mutex);
  } else {
    printf("Non recursive\n");
    err = uv_mutex_init(&_mutex);
  }

  if (err)
    Nan::ThrowError("Could not spawn the mutex");
}

Mutex::~Mutex() {
  uv_mutex_destroy(&_mutex);
}

NAN_METHOD(Mutex::New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowError("non-constructor invocation not supported");
  }

  if (!info[0]->IsBoolean()) {
    return Nan::ThrowError("Boolean value 'recursive' is required");
  }
  
  v8::Local<v8::Boolean> rec = Nan::To<v8::Boolean>(info[0]).ToLocalChecked();
  Mutex *mut = new Mutex(rec->BooleanValue());
  mut->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(Mutex::Lock) {
  Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
  Mutex *mut = ObjectWrap::Unwrap<Mutex>(info.Holder());
  Nan::AsyncQueueWorker(new LockWorker(callback, &mut->_mutex));
}

NAN_METHOD(Mutex::Unlock) {
  Mutex *mut = ObjectWrap::Unwrap<Mutex>(info.Holder());
  uv_mutex_unlock(&mut->_mutex);
}

Nan::Persistent<v8::Function> Mutex::constructor;
void Mutex::Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Mutex").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(2);

  SetPrototypeMethod(tpl, "lock", Lock);
  SetPrototypeMethod(tpl, "unlock", Unlock);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::New("Mutex").ToLocalChecked(),
    Nan::GetFunction(tpl).ToLocalChecked());
}

}