#include <node.h>

namespace demo {

using v8::Function;
using v8::Exception;
using v8::FunctionTemplate;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

// This is the implementation of the "add" method
// Input arguments are passed using the
// const FunctionCallbackInfo<Value>& args struct
void Add(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check the number of arguments passed.
  if (args.Length() < 2) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  // Perform the operation
  double value = args[0]->NumberValue() + args[1]->NumberValue();
  Local<Number> num = Number::New(isolate, value);

  // Set the return value (using the passed in
  // FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(num);
}

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void RunCallback(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Function> cb = Local<Function>::Cast(args[0]);
  const unsigned argc = 1;
  Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "hello world") };
  cb->Call(Null(isolate), argc, argv);
}

// void CreateFunction(const FunctionCallbackInfo<Value>& args) {
//   Isolate* isolate = args.GetIsolate();

//   Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, MyFunction);
//   Local<Function> fn = tpl->GetFunction();

//   // omit this to make it anonymous
//   fn->SetName(String::NewFromUtf8(isolate, "theFunction"));

//   args.GetReturnValue().Set(fn);
// }

void CreateObject(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Local<Object> obj = Object::New(isolate);
  
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, Add);
  Local<Function> fn = tpl->GetFunction();

  obj->Set(String::NewFromUtf8(isolate, "msg"), args[0]->ToString());
  obj->Set(String::NewFromUtf8(isolate, "add"), fn);

  args.GetReturnValue().Set(obj);
}

void Init(Local<Object> exports, Local<Object> module) {
//   NODE_SET_METHOD(exports, "add", Add);
//   NODE_SET_METHOD(exports, "hello", Method);
//   NODE_SET_METHOD(exports, "runCallback", RunCallback);
    NODE_SET_METHOD(module, "exports", CreateObject);
}

NODE_MODULE(addon, Init)

}  // namespace demo