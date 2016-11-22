/*******************************************************************************
 * Copyright 2016 IBM Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/


#include "headlessutils.h"
#include <iostream>
#include "nan.h"
#include "uv.h"

namespace headless {

Nan::Callback* zipFunction;

void setZipFunction(Nan::Callback* zF) {
	zipFunction = zF;
}

uv_async_t async_zip;

uv_loop_t* loop;
const char* outputDir;

void asyncfunc(uv_async_t* handle) {
	Nan::HandleScope scope;
 	v8::Isolate* isolate = v8::Isolate::GetCurrent();
  	v8::Local<v8::Value> argv[] = { v8::String::NewFromUtf8(isolate, outputDir) };
  	headless::zipFunction->Call(1, argv);
}


const char* headlessRecVersion = "1.0";

void start() {
	loop = uv_default_loop();
	uv_async_init(loop, &async_zip, asyncfunc);
}

void stop() {
	uv_close((uv_handle_t*) &async_zip, NULL);
}

void zip(const char* dir) {
	outputDir = dir;
	uv_async_send(&async_zip);
}


} /* end namespace headless */

