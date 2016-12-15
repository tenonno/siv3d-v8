#pragma once

namespace jsiv8
{


	struct js_object_base
	{

		js_object_base *parent = nullptr;


		bool isUndefined = true;

		s3d::String name;

		js_object_base()
		{

		}

		js_object_base(const s3d::String &name, js_object_base *parent)
			: name(name), parent(parent)
		{

		}





		template<class T>
		T operator=( T value)
		{

			if (this->parent == nullptr)
			{
				CL(L"e‚ª‘¶Ý‚µ‚Ü‚¹‚ñ: js_object_base - ", this->name);

			}

			this->parent->set(
				this->name,
				to_v8_value(value)
			);
			
			// to_v8_value

			return value;
		}


		virtual void set(s3d::String name, Local<Data> value)
		{

		};



	};






	class js_object : public js_object_base
	{

		v8::Local<v8::ObjectTemplate> object;

	public:


		v8::Local<v8::ObjectTemplate> toV8() const
		{
			return this->object;
		}

		js_object(v8::Isolate *isolate = v8::Isolate::GetCurrent())
		{

			auto obj = v8::ObjectTemplate::New(isolate);

			this->object = obj;

		}

		js_object_base operator[](const s3d::String &name)
		{
			return js_object_base(name, this);
		}



		void set(s3d::String name, Local<Data> value) override
		{


			this->object->Set(

				ToStringV8(name),
				value

			);


		};



	};


}