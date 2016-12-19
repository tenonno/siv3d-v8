
#include <Siv3D.hpp>
#include "JavaScript.hpp"


// 呼ばれたら Promise を返す Update 関数
v8::Persistent<v8::Promise::Resolver> persistentResolver;
void Update(const JS::Args &args)
{
	const auto isolate = args.GetIsolate();
	auto resolver = v8::Promise::Resolver::New(isolate);
	persistentResolver.Reset(isolate, resolver);
	args.GetReturnValue().Set(resolver->GetPromise());
}


#define __L__ 1


void Main()
{

	using namespace JS;


	JavaScript js;

	auto siv3d = CreateSiv3D();

	// Update を登録する
	siv3d[L"update"] = Function(Update);

	/*
	siv3d[L"setBackgroundHue"] = Function([](const JS::Args &args)
	{
		Graphics::SetBackground(HSV(args[0]->ToNumber()->Value(), 1.0, 1.0));
	});

	*/






	siv3d[L"setBackgroundHue"] = Function([](const Args &args)
	{


		// auto lambda = [&](double value, String str)

		auto lambda = [&](double value)
		{

			Graphics::SetBackground(HSV(value, 1.0, 1.0));

			// return true;

		};

		auto v = args[1];


		lambda(args[0]->ToNumber()->Value());

	});








	js.define()[L"siv3d"] = siv3d;

	//js.define()[L"function"] = Function




	const auto source = TextReader(L"test.js").readAll();
	const auto result = js.compile(source);


	// JavaScript の siv3d.main 関数を呼ぶ
	js.global()[L"siv3d"][L"main"](L"日本語, 絵文字 ``");


	while (System::Update())
	{

		// siv3d.frameCount を更新する
		js.global()[L"siv3d"].set(L"frameCount", JS::ToV8::Number(System::FrameCount()));


		// Update 関数が呼ばれていたら
		if (!persistentResolver.IsEmpty())
		{
			// JavaScript のメインループを許可する
			const auto resolver = v8::Local<v8::Promise::Resolver>::New(ISOLATE, persistentResolver);
			resolver->Resolve(JS::ToV8::Boolean(true));
			persistentResolver.Reset();
			ISOLATE->RunMicrotasks();
		}

	}

}

