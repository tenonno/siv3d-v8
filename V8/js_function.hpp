#pragma once





template <typename R, typename... Args> auto getResultType(R(*)(Args...))->R;
template <typename F, typename R, typename... Args> auto getResultType(R(F::*)(Args...))->R;
template <typename F, typename R, typename... Args> auto getResultType(R(F::*)(Args...) const)->R;
template <typename F, typename R = decltype(getResultType(&F::operator()))> auto getResultType(F)->R;
#define Result_Type(f) decltype(getResultType(f))

template <std::size_t I, typename R, typename... Args> auto getArgsType(R(*)(Args...)) -> typename std::tuple_element<I, typename std::conditional<std::is_same<std::tuple<Args...>, std::tuple<>>::value, std::tuple<void>, std::tuple<Args...>>::type>::type;
template <std::size_t I, typename F, typename R, typename... Args> auto getArgsType(R(F::*)(Args...)) -> typename std::tuple_element<I, typename std::conditional<std::is_same<std::tuple<Args...>, std::tuple<>>::value, std::tuple<void>, std::tuple<Args...>>::type>::type;
template <std::size_t I, typename F, typename R, typename... Args> auto getArgsType(R(F::*)(Args...) const) -> typename std::tuple_element<I, typename std::conditional<std::is_same<std::tuple<Args...>, std::tuple<>>::value, std::tuple<void>, std::tuple<Args...>>::type>::type;
template <std::size_t I, typename F, typename R = decltype(getArgsType<I>(&F::operator()))> auto getArgsType(F)->R;
#define Args_Type(i,f) decltype(getArgsType<i>(f))




namespace JS
{


	class js_function
	{

	public:


		template<class T>
		js_function(T callback)
		{



		}


		void we(const v8::FunctionCallbackInfo<v8::Value> &args)
		{

		}





		v8::Local<v8::FunctionTemplate> toV8()
		{

			v8::FunctionTemplate::New(

				v8::Isolate::GetCurrent(),

				[](const v8::FunctionCallbackInfo<v8::Value> &args)
			{



			});

		}




	};



}
