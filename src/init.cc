#include "init.h"

using Lokker::Mutex;

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  Mutex::Init(exports);
}

NODE_MODULE(lokker, Init)
