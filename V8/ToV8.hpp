#pragma once


namespace JS
{

	namespace ToV8
	{


		inline auto Number(const double value)
		{
			return v8::Number::New(ISOLATE, value);
		}

		inline auto Boolean(const bool value)
		{
			return v8::Boolean::New(ISOLATE, value);
		}

		inline auto String(const s3d::String &value)
		{
			return ToStringV8(value);
		}


		template<class T>
		inline auto Value(const T &value)
		{
			return to_v8_value(value);
		}


	}

}