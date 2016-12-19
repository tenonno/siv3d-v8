#pragma once

#include "include/libplatform/libplatform.h"
#include "include/v8.h"


inline v8::Local<v8::Value> to_v8_value(const double value)
{
	return ToV8::Number(value);
}


inline v8::Local<v8::Value> to_v8_value(const int value)
{
	return ToV8::Number(value);
}

inline v8::Local<v8::Value> to_v8_value(const s3d::String &value)
{
	return ToStringV8(value);

}

inline v8::Local<v8::Value> to_v8_value(const wchar *value)
{
	return ToStringV8(value);

}


inline v8::Local<v8::Value> to_v8_value(const bool value)
{
	return ToBooleanV8(value);
}


inline v8::Local<v8::Data> to_v8_value(const v8::Local<v8::FunctionTemplate> &value)
{
	return value;
}


class JS_Template;



struct JS_Template_Reference
{



	s3d::String _name;
	v8::Local<v8::ObjectTemplate> *ot;

	JS_Template_Reference(s3d::String n, v8::Local<v8::ObjectTemplate> *oot)
		: _name(n), ot(oot)
	{


	}


	template<class T>
	void operator=(const T &value)
	{


		//CL(this->_name);

		(*this->ot)->Set(

			ToV8::String(this->_name),
			ToV8::Value(value)
		);

		// this->TP->set(this->_name, to_v8_value(value));

	}



	template<>
	void operator=<JS::Function>(const JS::Function &value)
	{

		(*this->ot)->Set(

			ToV8::String(this->_name),
			value.toV8()

		);

	}


	template<>
	void operator=<JS::Object>(const jsiv8::js_object &value)
	{




		this->this_object_template = value.toV8();
		(*this->ot)->Set(
			ToStringV8(this->_name),
			value.toV8()
		);


	}

	v8::Local<v8::ObjectTemplate> this_object_template;

	template<>
	void operator=(const v8::Local<v8::ObjectTemplate> &value)
	{

		this->this_object_template = value;

		const auto isolate = v8::Isolate::GetCurrent();

		(*this->ot)->Set(

			ToStringV8(this->_name),
			value
		);

	}







	template< >
	void operator=<FunctionCallback>(const FunctionCallback &value)
	{

		const auto isolate = v8::Isolate::GetCurrent();

		(*this->ot)->Set(

			ToStringV8(this->_name),
			v8::FunctionTemplate::New(isolate, value)

		);

	}

	//JS_Template this_this;

	JS_Template_Reference operator[](const s3d::String &name)
	{

		if (this->this_object_template.IsEmpty())
		{
			CL(L"not child");
		}

		//this_this = JS_Template(this->this_object_template);

		return JS_Template_Reference(
			name,
			&this->this_object_template

		);

		//		this->_data->

	}


};



class JS_Template
{

	v8::Local<v8::ObjectTemplate> _data;

public:




	void set(s3d::String name, Local<Data> value)
	{

		this->_data->Set(ToStringV8(name), value);

	}


	JS_Template_Reference operator[](const s3d::String &name)
	{


		return JS_Template_Reference(
			name,

			&this->_data

		);

		//		this->_data->

	}



	void set(s3d::String name, FunctionCallback fn)
	{

		const auto isolate = v8::Isolate::GetCurrent();


		this->_data->Set(
			ToStringV8(name),
			v8::FunctionTemplate::New(isolate, fn)
		);

	}






	JS_Template(const v8::Local<v8::ObjectTemplate> &data)
	{


		this->_data = data;



	}


};