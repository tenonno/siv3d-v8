#pragma once

#include "include/libplatform/libplatform.h"
#include "include/v8.h"




inline v8::Local<v8::Value> to_v8_value(const double value)
{
	return ToNumberV8(value);
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

	JS_Template *TP;


	s3d::String _name;
	v8::Local<v8::ObjectTemplate> *ot;

	JS_Template_Reference(JS_Template *t, s3d::String n, v8::Local<v8::ObjectTemplate> *oot)
		: TP(t), _name(n), ot(oot)
	{


	}


	template<class T>
	void operator=(const T &value)
	{

		this->TP->set(this->_name, to_v8_value(value));

	}

	template<class T>
	typename std::enable_if<std::is_floating_point<T>::value, T>::type
		operator=(const T &value)
	{
		std::cout << "foo1: float\n";
		return t;
	}





		template<>
	void operator=<jsiv8::js_object>(const jsiv8::js_object &value)
	{

		(*this->ot)->Set(
			ToStringV8(this->_name),
			value.toV8()
		);


	}

	template<>
	void operator=(const v8::Local<v8::ObjectTemplate> &value)
	{

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

			this,
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