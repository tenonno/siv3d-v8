
#include <Siv3D.hpp>
#include "JavaScript2.hpp"


// 呼ばれたら Promise を返す Update 関数
v8::Persistent<v8::Promise::Resolver> persistentResolver;
void Update(const JS::Args &args)
{
	const auto isolate = args.GetIsolate();
	auto resolver = v8::Promise::Resolver::New(isolate);
	persistentResolver.Reset(isolate, resolver);
	args.GetReturnValue().Set(resolver->GetPromise());
}



void Main()
{

	JavaScript js;

	auto siv3d = JS::CreateSiv3D();

	// Update を登録する
	siv3d[L"update"] = JS::Function(Update);

	// 色相から背景色を変える関数を登録する
	siv3d[L"setBackgroundHue"] = JS::Function([](const JS::Args &args)
	{
		Graphics::SetBackground(HSV(args[0]->ToNumber()->Value(), 1.0, 1.0));
	});


	js.define()[L"siv3d"] = siv3d;

	//js.define()[L"function"] = Function




	const auto source = TextReader(L"test.js").readAll();
	const auto result = js.compile(source);


	// JavaScript の siv3d.main 関数を呼ぶ
	js.global()[L"siv3d"][L"main"]();


	while (System::Update())
	{

		// siv3d.frameCount を更新する
		js.global()[L"siv3d"].set(L"frameCount", ToV8::Number(System::FrameCount()));


		// Update 関数が呼ばれていたら
		if (!persistentResolver.IsEmpty())
		{
			// JavaScript のメインループを許可する
			const auto resolver = v8::Local<v8::Promise::Resolver>::New(ISOLATE, persistentResolver);
			resolver->Resolve(ToV8::Boolean(true));
			persistentResolver.Reset();
			ISOLATE->RunMicrotasks();
		}

	}

}

