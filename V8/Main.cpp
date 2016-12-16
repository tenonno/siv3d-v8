
#include <Siv3D.hpp>

#include "JavaScript2.hpp"

void Main()
{



	Window::Resize(800, 600);
	Graphics::SetBackground(Color(0x333333));
	const Font font(12);


	JavaScript js;


	js.define()[L"siv3d"] = jsiv8::CreateSiv3D();



	const auto js_system = v8::ObjectTemplate::New(js.isolate);
	 

	auto obj = jsiv8::js_object();

	obj[L"test"] = 12.0;


	js.define()[L"t"] = obj;

	/*
	js.globalObject->Set(
		ToStringV8(L"system"),
		js_system);
	*/

	js.define()[L"system"] = js_system;


	js_system->Set(
		ToStringV8(L"update"),
		v8::FunctionTemplate::New(js.isolate, PPP));

	/*


	js.define()[L"test"] = 123;

	//js.define()[L"test2"] = true;


	// js.define()[L"test3"] = L"aaa";

	js.define()[L"func"] =



	*/
	/*
	int i = 0;

	js.define()[L"func"] = (FunctionCallback)([i](const v8::FunctionCallbackInfo<v8::Value> &args)
	{

		CL(L"1");

		args.GetReturnValue().SetNull();
	});

	*/






	const auto source = TextReader(L"test.js").readAll();
	const auto result = js.compile(source);



	js.global()[L"siv3d"][L"main"](L"test");


	Window::SetTitle(L"Siv3D App - V8 ", js.version);

	while (System::Update())
	{


		// ClearPrint();


		if (!pvv.IsEmpty() && !(System::FrameCount() % 10)) {


			const auto aa = v8::Local<v8::Promise::Resolver>::New(js.isolate, pvv);

			js.global()[L"siv3d"].set(L"frameCount", ToNumberV8(System::FrameCount()));

			aa->Resolve(ToBooleanV8(true));
			pvv.Reset();
			//v8::Undefined(js.isolate));

			ISOLATE->RunMicrotasks();

			//Println(L"Resolve", aa.IsEmpty());


			//
		}


		const auto rect = font(source).draw(140, 10);

		rect.drawFrame(1, 0, s3d::Color(255, 0, 0));


		// aaa(ToBooleanV8(true));

		const auto y = rect.y + rect.h + 5;

		Line(0, y, Window::Width(), y).draw();

		font(result).draw(140, rect.y + rect.h + 10);

	}



}
