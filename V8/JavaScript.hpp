#pragma once

# include <Siv3D.hpp>

#pragma comment(lib,"v8.dll.lib")
#pragma comment(lib,"v8_libbase.dll.lib")
#pragma comment(lib,"v8_libplatform.dll.lib")


#include "include/v8.h"


#include "CV8Initializer.hpp"

#include <stdio.h>
#include <iostream>
#include <string>


#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include <sstream>



class JavaScript
{

private:

	v8::Isolate *_isolate = nullptr;

	s3d::String _version;


	v8::Platform *_platform = nullptr;


	std::unique_ptr<v8::Platform> _aaa;



	v8::Local<v8::Context> _context;

	std::unique_ptr<Instance<v8::HandleScope>> _scope;


	A2 _global;

public:

	using Scope = v8::Context::Scope;

	Property_Get(v8::Local<v8::Context>, context) const
	{
		return this->_context;
	};


	Property_Get(v8::Isolate *, isolate) const
	{
		return this->_isolate;
	};

	Property_Get(s3d::String, version) const
	{
		return this->_version;
	};

	Property_Get(v8::Platform *, platform) const
	{
		return this->_platform;
	};

	/*

	A2 GetGlobal()
	{
		return (this->context->Global());
	}


	__declspec(property(get = GetGlobal)) A2 global;
	*/
	/*
	jsiv8::JS_Value root() const
	{
		const jsiv8::JS_Object root;
		return root;
	}
	*/
	



	void initialize()
	{


		v8::Isolate::CreateParams create_params;


		create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();


		auto isolate = v8::Isolate::New(create_params);

		isolate->Enter();




		using wss = std::wstringstream;
		const auto this_id = static_cast<wss &>(wss() << std::this_thread::get_id()).str();
		Println(L"thread-id: ", this_id);



		// Isolate を現在のスレッドに割り当てる

		// Isolate::Scope isolate_scope(isolate);

		// v8::HandleScope scope(isolate);



		this->_scope = std::make_unique<Instance<v8::HandleScope>>(isolate);
		


		//this->___scope = v8::HandleScope(isolate);

		this->_isolate = isolate;


		//v8::Unlocker locker(this->isolate);

		// CL(locker.IsLocked(this->isolate));



		// this->initialize_isolate();



		this->_version = Widen(v8::V8::GetVersion());



		v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(this->isolate);


		v8::Local<v8::ObjectTemplate> globalS3d = v8::ObjectTemplate::New(this->isolate);

		global->Set(
			v8::String::NewFromUtf8(this->isolate, "siv3d", v8::NewStringType::kNormal)
			.ToLocalChecked(),
			globalS3d);

		globalS3d->Set(
			v8::String::NewFromUtf8(this->isolate, "println", v8::NewStringType::kNormal)
			.ToLocalChecked(),
			v8::FunctionTemplate::New(isolate, test));


		globalS3d->Set(
			ToStringV8(L"clearPrint"),
			v8::FunctionTemplate::New(isolate,
				[](const v8::FunctionCallbackInfo<v8::Value> &args) -> void
		{

			s3d::ClearPrint();

		}));

		this->globalObject = global;
		this->siv3d = globalS3d;

		g::isolate = isolate;

	}

	v8::Local<v8::ObjectTemplate> globalObject;
	v8::Local<v8::ObjectTemplate> siv3d;

	A2 __GLOBAL;


	inline A2 __GET_GLOBAL__()
	{

		jsiv8::GET = true;

		return this->__GLOBAL.value;
	}

	__declspec(property(get = __GET_GLOBAL__)) A2 __GET__;


	A2 global()
	{
		return this->__GLOBAL.value;
	}

	JavaScript()
	{

		V8::InitializeICU();
		V8::InitializeExternalStartupData("./");
		
		// this->_platform = v8::platform::CreateDefaultPlatform();
		
		
		this->_aaa.reset(v8::platform::CreateDefaultPlatform());

		
		V8::InitializePlatform(this->_aaa.get());
		V8::Initialize();

		this->initialize();


	}

	s3d::String compile(const s3d::String &source)
	{


		// JS を実行するコンテキストを生成する
		v8::Local<v8::Context> context1 = v8::Context::New(this->isolate, nullptr, this->globalObject);


		context1->Enter();


		// new C_B(this->context);

		// new C_B(this->context);


		// v8::Context::Scope context_scope(context);


		this->_context = context1;




		this->_global = A2(context->Global());
		this->_global.context_ptr = context1;

		this->__GLOBAL = A2(context->Global());



		auto isolate = this->isolate;


		s3d::String result;

		//isolate->Enter();

		Local<v8::String> sourcev8 = ToStringV8(source);


		//	const auto isolate = Isolate::GetCurrent();

		Local<Context> context = this->context;//Context::New(isolate);
		
		
		// v8::Context::Scope context_scope(context);

		//const auto context = isolate->GetCurrentContext();

		TryCatch try_catch(isolate);


		// Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();



		// Compile the script and check for errors.
		Local<v8::Script> compiled_script;



		if (!v8::Script::Compile(context, sourcev8).ToLocal(&compiled_script)) {
			v8::String::Utf8Value error(try_catch.Exception());

			return FromV8UTF8(error);

		}
		/*
		// terminate script in 5 seconds
		v8::Concurrency::create_task([isolate]
		{
			Concurrency::wait(5000);
			v8::V8::TerminateExecution(isolate);
		});
		*/


		// Run the script!
		Local<Value> cresult;
		if (!compiled_script->Run(context).ToLocal(&cresult)) {
			// The TryCatch above is still in effect and will have caught the error.

			v8::String::Utf8Value error(try_catch.Exception());
			// Running the script failed; bail out.			
			//			CL(Widen(*error));

			result = Widen(UTF8toSjis(*error));

			CL(result);

		}
		else
		{

			//auto a = *cresult;



			/*
			std::wstring inputwstr = L"太郎は次郎が持っている本を花子に渡した。";
			const wchar_t *inputw = inputwstr.c_str();
			char input[1024];
			wcstombs(input, inputw, sizeof(wchar_t)*int(inputwstr.size()));
			std::cout << "INPUT: " << input << std::endl;



			v8::String::Utf8Value utf8(cresult);


			::setlocale(LC_CTYPE, "jpn");

			wchar_t ws[255];

			int len = mbstowcs(ws, *utf8, 255);
			*/

			/*
			wchar_t wc[100];
			const char c[] = "あいうえお";
			mbstowcs(wc, c, sizeof(c));
			*/
			/*

			CL(len);
			*/

			// CL(utf8.length());


			v8::String::Utf8Value utf8(cresult);

			//CL(sizeof(*utf8));

			result = FromV8UTF8(utf8);

		}

		//isolate->Enter();

		// this->global[L"siv3d"][L"main"](123);

		//		CL(result);
		return result;



		// CL(L"dead? ", isolate->IsDead())

		//CL(Isolate::GetCurrent() == this->isolate);
		//return Compile(source, isolate);//Isolate::GetCurrent());
	}

	~JavaScript()
	{

		this->context->Exit();

		V8::Dispose();
		V8::ShutdownPlatform();
		
		// delete this->platform;

	}


};