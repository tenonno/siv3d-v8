#pragma once



#include "include/v8.h"


#include <stdio.h>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>




// 名前（仮）
// JS + Siv + V8 = jsiv8
#define JS jsiv8



#ifdef _WIN64

#pragma comment(lib, "v8.dll.lib")
#pragma comment(lib, "v8_libbase.dll.lib")
#pragma comment(lib, "v8_libplatform.dll.lib")

#else

#endif


// スナップショットのリソース番号
#define V8_X64_NATIVES_BLOB  8000
#define V8_X64_SNAPSHOT_BLOB 8001


#ifdef _WIN64

#define V8_NATIVES_BLOB  V8_X64_NATIVES_BLOB
#define V8_SNAPSHOT_BLOB V8_X64_SNAPSHOT_BLOB

#else

#endif







#include "StartupData.hpp"




template <class ... Args>
inline void CL(const Args& ... args)
{
	MessageBox::Show(Format(args...));
}



#include <thread>


// namespace Cast

#define ISOLATE v8::Isolate::GetCurrent()


inline v8::Local<v8::String> ToStringV8(const s3d::String &string)
{

	const auto isolate = v8::Isolate::GetCurrent();


	return v8::String::NewFromTwoByte(
		isolate,
		reinterpret_cast<const uint16 *>(string.c_str()));

}


#include "ToV8.hpp"




#define _CRT_SECURE_NO_WARNINGS



#include <windows.h>

#undef MessageBox


#pragma warning(disable : 4996)


inline s3d::String FromV8UTF8(const v8::String::Utf8Value &value)
{

	return s3d::CharacterSet::FromUTF8(*value);


}

inline s3d::String FromV8UTF8(const v8::Local<v8::String> &string)
{

	return FromV8UTF8(v8::String::Utf8Value(string));
}



struct Text
{
	s3d::String text;
	s3d::Color color;
};


s3d::String FormatV8(const v8::Local<v8::Value> &value)
{

	if (value->IsUndefined())
	{
		return L"undefined";
	}

	if (value->IsBoolean())
	{
		return value->ToBoolean()->Value() ? L"true" : L"false";
	}

	if (value->IsString())
	{
		return FromV8UTF8(value->ToString());
	}

	if (value->IsNumber())
	{

		return Format(value->ToNumber()->Value());

	}


	if (value->IsObject())
	{

		return FromV8UTF8(value->ToObject()->ObjectProtoToString());

	}

	if (value->IsNull()) {
		return L"null";
	}

	return L"?";

}



namespace JS
{


	class JS_Object
	{
		std::unordered_map<s3d::String, JS_Object *> properties;


		JS_Object *parent;

	public:


		virtual JS_Object *toPointer()
		{


		}


	};



	/*
	class JS_Value
	{

	s3d::Array<JS_Value *> properties;

	public:

	JS_Object operator[](const s3d::String &name)
	{

	const auto obj = JS_Object();
	return obj;
	}

	};




	class JS_Object : public JS_Value
	{




	public:

	v8::Local<v8::ObjectTemplate> toJS()
	{

	}





	};

	*/



	v8::Local<v8::Context> context;


	struct CompileResult
	{



	};







}

#include "Test.hpp"


#include <future>


#include <sstream>



template<class T>
class Instance
{

	T instance;


public:

	template<class ...Types>
	Instance(Types&&... args) : instance(std::forward<Types>(args)...)
	{

	}

};

#include "Function.hpp"


#include "JavaScriptCore.hpp"




class Promise
{

	v8::Persistent<v8::Promise::Resolver> resolver;

public:

	Promise(v8::FunctionCallback callback)
	{



	}



};


namespace JS
{

}


namespace JS
{

	Object CreateSiv3D() {

		auto siv3d = Object();

		auto isolate = ISOLATE;


		siv3d[L"clearPrint"] = v8::FunctionTemplate::New(
			isolate,
			[](const v8::FunctionCallbackInfo<v8::Value> &args)
		{

			s3d::ClearPrint();

		});



		siv3d[L"println"] = v8::FunctionTemplate::New(
			isolate,
			[](const v8::FunctionCallbackInfo<v8::Value> &args)
		{

			int size = args.Length();

			s3d::String result;

			for (int i = 0; i < size; ++i)
			{

				if (result.length) result += L", ";

				result += FormatV8(args[i]);

			}



			// MessageBox::Show(result);

			s3d::Println(result);



			args.GetReturnValue().SetUndefined();

		});







		return siv3d;

	}

}








namespace JS
{


	void PromiseCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
	{
	}



	template<class T>
	class Promise
	{
		std::function<T> callback;
		v8::Persistent<v8::Promise::Resolver> resolver;

	public:

		Promise(const std::function<T> &func)
			: callback(func)
		{



		}


		void function(const v8::FunctionCallbackInfo<v8::Value> &args)
		{

			const auto isolate = args.GetIsolate();


			this->resolver.Reset(
				isolate,
				v8::Promise::Resolver::New(isolate)
			);

			auto local = v8::Local<v8::Promise::Resolver>::New(isolate, this->resolver);

			args.GetReturnValue().Set(local->GetPromise());

		}



		v8::Local<v8::FunctionTemplate> toV8()
		{




			auto promise = JS::PromiseResolver<void()>([]()
			{

				CL(L"1");

			});



			return v8::FunctionTemplate::New(
				ISOLATE,
				&promise.function
			);
		}


		template<class T>
		void resolve(const T &value)
		{


			const auto localResolver = v8::Local<v8::Promise::Resolver>::New(ISOLATE, this->resolver);

			localResolver->Resolve(ToV8::Value(value));




			this->resolver.Reset();
		}


	};


	template<class T>
	using PromiseResolver = Promise<T>;

}



namespace JS
{

	using Args = v8::FunctionCallbackInfo<v8::Value>;

}



