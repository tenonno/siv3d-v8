#pragma once


namespace JS
{


	class Function
	{

		v8::Local<v8::FunctionTemplate> functionTemplate;

	public:

		Function(v8::FunctionCallback function)
		{

			this->functionTemplate = v8::FunctionTemplate::New(
				ISOLATE,
				function
			);

		}


		v8::Local<v8::FunctionTemplate> toV8() const
		{
			return this->functionTemplate;
		}


	};



}