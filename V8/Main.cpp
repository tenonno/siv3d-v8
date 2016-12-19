
#include <Siv3D.hpp>
#include "JavaScript2.hpp"



void Main()
{

	JavaScript js;

	auto siv3d = JS::CreateSiv3D();

	js.define()[L"siv3d"] = siv3d;

	const auto source = TextReader(L"test.js").readAll();
	const auto result = js.compile(source);


	while (System::Update())
	{
	}

}
