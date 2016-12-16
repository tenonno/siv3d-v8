
#pragma once
# include <Siv3D.hpp>

#pragma comment(lib,"v8.dll.lib")
#pragma comment(lib,"v8_libbase.dll.lib")
#pragma comment(lib,"v8_libplatform.dll.lib")


#include "include/v8.h"



#include <stdio.h>
#include <iostream>
#include <string>


#include "include/libplatform/libplatform.h"
#include "include/v8.h"


namespace NS_JS
{
	v8::Local<v8::Context> context;
}

#include <type_traits>


using ArrayValueV8 = s3d::Array<v8::Local<v8::Value>>;

template<class T>
ArrayValueV8 func2(T t)
{
	ArrayValueV8 result;



	result.emplace_back(v8::Undefined(Isolate::GetCurrent()));

	return result;
};


template<>
ArrayValueV8 func2(const wchar_t *t)
{

	return ArrayValueV8{ ToStringV8(t) };

}


template<>
ArrayValueV8 func2(const double t)
{

	ArrayValueV8 result;

	result.emplace_back(ToNumberV8(t));

	return result;
}


template<>
ArrayValueV8 func2(const int value)
{
	return func2(static_cast<double>(value));
}




template<>
ArrayValueV8 func2(const bool value)
{
	return ArrayValueV8{ ToBooleanV8(value) };
}





template<class First, class... Rest>

ArrayValueV8 func2(const First& first, const Rest&... rest)
{
	ArrayValueV8 &&a = func2(first);
	ArrayValueV8 &&b = func2(rest...);
	std::copy(b.begin(), b.end(), std::back_inserter(a));

	return a;

}


template<class... Rest>

ArrayValueV8 func(const Rest&... rest)
{

	ArrayValueV8 &&a = func2(rest...);

	for (auto &aa : a)
	{

		// Println(FromV8UTF8(aa->ToString()));

	}

	return a;
}



class A2
{



	bool initialized = true;

public:
	A2()
	{
		// if (jsiv8::GET) CL(L"const");

		this->initialized = false;
	}
	v8::Local<v8::Value> value;

	v8::Local<v8::Context> context_ptr;

	v8::Local<v8::Value> _get() const
	{
		return this->value;
	}

	A2(const v8::Local<v8::Value> &value)
	{

		this->value = value;
	}

	A2(const A2 &ref) = delete;
	/*
	{
		if (jsiv8::GET) CL(L"const1");

		this->value = ref.value;

	}
	*/

	A2& operator=(A2&& _Right) noexcept
	{
		this->initialized = true;
		this->value = _Right._get();
		return (*this);
	}


	A2 operator[](s3d::String name)
	{

		if (this->value.IsEmpty())
		{
			CL(L"is empty");
		}

		// CL(this->initialized);

		if (!this->value->IsObject())
		{
			CL(L"object");
		}


		return (this->value->ToObject()->Get(ToStringV8(name)));

	}

	A2 toFunction()
	{

	}


	void set(const s3d::String &key, const v8::Local<v8::Value> &v) {


		this->value->ToObject()->Set(ToStringV8(key), v);


	}



	template <class ... Args>
	void operator()(const Args& ... args)
	{

		auto &&arguments = func(args...);


		// CL(arguments.size());

		// CL(*this->value);
			
		// GetCurrent: 現在のスレッドの Isolate を取得する
		// GetCurrentContext: 実行中のコンテキストを取得する

		v8::Local<v8::Context> context = Isolate::GetCurrent()->GetCurrentContext();
		
	
		// auto a = v8::Local<v8::Function>::Cast(this->value)->GetName()->ToString();

		

		// CL(*context, Isolate::GetCurrent());

		// auto context = this->context_ptr;

		// CL(context.IsEmpty());

		// v8::Context::Scope scope(context);

		//v8::Context::Scope context_scope(context);


		if (this->value.IsEmpty())
		{
			CL(L"IsEmpty");
		}


		auto func = v8::Local<v8::Function>::Cast(this->value);

		if (!func->IsFunction())
		{

			CL(L"not function");
		}


		func->Call(
			context,
			this->value,
			arguments.size(),
			arguments.data());

	}

	/*

	void operator()(v8::Local<v8::Context> ctx, int i, Local<Value> args[])
	{

		v8::Local<v8::Context> context = Isolate::GetCurrent()->GetCurrentContext();


		//v8::Local<v8::Context> ctx = Isolate::GetCurrent()->GetCurrentContext();

		//v8::Local<v8::Function>::Cast(main)->Call(context, js_update, 1, arggg);


		v8::Local<v8::Function>::Cast(this->value)->Call(ctx, this->value, i, args);

	}

	*/
};

/*

template<>
A2<v8::Object> A2<v8::Object>::operator[](s3d::String name) 
{


	return this->value->Get(ToStringV8(name));
}

*/