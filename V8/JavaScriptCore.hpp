#pragma once





# include <Siv3D.hpp>

#include "include/v8.h"


#include "Allocator.hpp"

#include <stdio.h>
#include <iostream>
#include <string>


#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include <sstream>


#include "js_object.hpp"
#include "js_function.hpp"
#include "Template.hpp"

class JavaScript
{

private:

	v8::Isolate *m_isolate = nullptr;

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

	Property_Get(s3d::String, version) const
	{
		return this->_version;
	};

	Property_Get(v8::Platform *, platform) const
	{
		return this->_platform;
	};


	void initialize()
	{


		v8::Isolate::CreateParams create_params;


		create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();


		auto isolate = v8::Isolate::New(create_params);

		isolate->Enter();



		/*
		using wss = std::wstringstream;
		const auto this_id = static_cast<wss &>(wss() << std::this_thread::get_id()).str();
		Println(L"thread-id: ", this_id);
		*/


		// Isolate を現在のスレッドに割り当てる

		// Isolate::Scope isolate_scope(isolate);

		// v8::HandleScope scope(isolate);



		this->_scope = std::make_unique<Instance<v8::HandleScope>>(isolate);
		


		//this->___scope = v8::HandleScope(isolate);

		this->m_isolate = isolate;


		//v8::Unlocker locker(this->isolate);

		// CL(locker.IsLocked(this->isolate));



		// this->initialize_isolate();



		this->_version = Widen(v8::V8::GetVersion());



		v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);


		v8::Local<v8::ObjectTemplate> globalS3d = v8::ObjectTemplate::New(isolate);

		/*
		global->Set(
			v8::String::NewFromUtf8(this->isolate, "siv3d", v8::NewStringType::kNormal)
			.ToLocalChecked(),
			globalS3d);

		*/











		this->globalObject = global;
		this->siv3d = globalS3d;


	}


	JS_Template define()
	{
		return JS_Template(this->globalObject);
	}


	v8::Local<v8::ObjectTemplate> globalObject;
	v8::Local<v8::ObjectTemplate> siv3d;

	A2 __GLOBAL;



	A2 global()
	{
		return this->__GLOBAL.value;
	}

	std::unique_ptr<JS::StartupDataBlob> nativesDataBlob;
	std::unique_ptr<JS::StartupDataBlob> snapshotDataBlob;

	void setSnapshot(const JS::StartupData &snapshot)
	{

		this->nativesDataBlob =
			std::move(std::make_unique<JS::StartupDataBlob>(snapshot.natives_blob));

		this->snapshotDataBlob =
			std::move(std::make_unique<JS::StartupDataBlob>(snapshot.snapshot_blob));


		v8::V8::SetNativesDataBlob(this->nativesDataBlob->get());
		v8::V8::SetSnapshotDataBlob(this->snapshotDataBlob->get());

	}



	JavaScript(const JS::StartupData &snapshot = JS::StartupData::Default)
	{

		v8::V8::InitializeICU();

		this->setSnapshot(snapshot);

		// V8::InitializeExternalStartupData("./");
		
		// v8::V8::SetNativesDataBlob()


		// this->_platform = v8::platform::CreateDefaultPlatform();


		this->_aaa.reset(v8::platform::CreateDefaultPlatform());

		
		v8::V8::InitializePlatform(this->_aaa.get());
		v8::V8::Initialize();

		this->initialize();


	}

	s3d::String compile(const s3d::String &source)
	{


		// JS を実行するコンテキストを生成する
		auto context1 = v8::Context::New(
			ISOLATE
			, nullptr, this->globalObject);


		context1->Enter();


		// new C_B(this->context);
		// new C_B(this->context);
		// v8::Context::Scope context_scope(context);


		this->_context = context1;




		this->_global = A2(context->Global());
		this->_global.context_ptr = context1;

		this->__GLOBAL = A2(context->Global());




		s3d::String result;

		//isolate->Enter();

		v8::Local<v8::String> sourcev8 = ToStringV8(source);


		//	const auto isolate = Isolate::GetCurrent();

		v8::Local<v8::Context> context = this->context;//Context::New(isolate);
		
		
		// v8::Context::Scope context_scope(context);

		//const auto context = isolate->GetCurrentContext();

		v8::TryCatch try_catch(this->m_isolate);


		// Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();



		// Compile the script and check for errors.
		v8::Local<v8::Script> compiled_script;



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
		v8::Local<v8::Value> cresult;
		if (!compiled_script->Run(context).ToLocal(&cresult)) {
			// The TryCatch above is still in effect and will have caught the error.

			v8::String::Utf8Value error(try_catch.Exception());
			// Running the script failed; bail out.			
			//			CL(Widen(*error));

			result = FromV8UTF8(error);// Widen(UTF8toSjis(*error));

			CL(result);

		}
		else
		{


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

		v8::V8::Dispose();
		v8::V8::ShutdownPlatform();
		
		// delete this->platform;

	}


};