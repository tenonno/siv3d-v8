
# include <Siv3D.hpp>

#pragma comment(lib,"v8.dll.lib")
#pragma comment(lib,"v8_libbase.dll.lib")
#pragma comment(lib,"v8_libplatform.dll.lib")


#include "include/v8.h"


#include "CV8Initializer.hpp"

#include <stdio.h>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>


#include "include/libplatform/libplatform.h"
#include "include/v8.h"

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator
{
public:
	virtual void* Allocate(size_t length) {
		void* data = AllocateUninitialized(length);
		return data == NULL ? data : memset(data, 0, length);
	}
	virtual void* AllocateUninitialized(size_t length) { return new char[length]; }
	virtual void Free(void* data, size_t) { delete[] data; }
};

using namespace v8;


/*

Handle<v8::String> ReadFile(s3d::String fileName) {



	TextReader reader(fileName);

	const s3d::String s = reader.readAll();


	FILE* file = fopen(name, "r");
	if (file == NULL) return Handle<v8::String>();

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	char* chars = new char[size + 1];
	chars[size] = '\0';
	for (int i = 0; i < size;) {
		int read = fread(&chars[i], 1, size - i, file);
		i += read;
	}

	fclose(file);


		Handle<v8::String>::New(

	Handle<v8::String> result = v8::String::NewFromUtf8(chars, size);
	delete[] chars;
	return result;
}

*/


template <class ... Args>
inline void CL(const Args& ... args)
{
	MessageBox::Show(Format(args...));
}

class MallocArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
public:
	virtual void* Allocate(size_t length) { return malloc(length); }
	virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
	virtual void Free(void* data, size_t length) { free(data); }
};



#include <thread>


namespace g
{
	Isolate *isolate;
}

inline v8::Local<v8::Number> ToNumberV8(const double value)
{
	const auto isolate = g::isolate; //Isolate::GetCurrent();

	return v8::Number::New(isolate, value);
}

inline v8::Local<v8::Boolean> ToBooleanV8(const bool value)
{
	const auto isolate = g::isolate; //Isolate::GetCurrent();

	return v8::Boolean::New(isolate, value);
}



inline v8::Local<v8::String> ToStringV8(const s3d::String &string)
{

	const auto isolate = v8::Isolate::GetCurrent();


	return v8::String::NewFromTwoByte(
		isolate,
		reinterpret_cast<const uint16 *>(string.c_str()));

}

#define _CRT_SECURE_NO_WARNINGS



#include <windows.h>

#undef MessageBox

/*
Local<String> CreateString(Isolate* isolate, wchar_t* pValue)
{
	return String::NewFromTwoByte(isolate, reinterpret_cast<const uint16_t*>(pValue));
}
*/


/*

int _tmain(int argc, _TCHAR* argv[])
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope handle_scope(isolate);
	Handle<Context> context = Context::New(isolate);
	Context::Scope context_scope(context);
	CString temp = "'你'+'好'";
	String::Utf8Value  mergeResult(ExecuteJs(isolate, CreateString(temp.GetBuffer())));
	wchar_t* finalResult = ConvertAnsiToUnicode(*mergeResult);
	cout << finalResult << endl;
	return 0;
}


*/




#pragma warning(disable : 4996)




std::string UTF8toSjis(std::string srcUTF8) {
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJISへ変換後の文字列長を得る
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけShiftJIS文字列のバッファを確保
	char* bufShiftJis = new char[lengthSJis];

	//UnicodeからShiftJISへ変換
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	delete bufUnicode;
	delete bufShiftJis;

	return strSJis;
}



inline s3d::String FromV8UTF8(const v8::String::Utf8Value &value)
{
	return Widen(UTF8toSjis(*value));
}

inline s3d::String FromV8UTF8(const v8::Local<v8::String> &string)
{
	return FromV8UTF8(v8::String::Utf8Value(string));
}



struct Text
{
	s3d::String text;
	s3d::Color color;
};


s3d::String FormatV8(const v8::Local<v8::Value> &value)
{

	if (value->IsUndefined())
	{
		return L"undefined";
	}

	if (value->IsBoolean())
	{
		return value->ToBoolean()->Value() ? L"true" : L"false";
	}

	if (value->IsString())
	{
		return FromV8UTF8(value->ToString());
	}

	if (value->IsNumber())
	{

		return Format(value->ToNumber()->Value());

	}


	if (value->IsObject())
	{

		return FromV8UTF8(value->ToObject()->ObjectProtoToString());

	}


	return L"?";

}


void test(const v8::FunctionCallbackInfo<v8::Value> &args)
{

	int size = args.Length();

	s3d::String result;

	for (int i = 0; i < size; ++i)
	{


		result += FormatV8(args[i]);

	}



	// MessageBox::Show(result);

	Println(result);


	args.GetReturnValue().SetUndefined();

}


namespace jsiv8
{

	bool GET = false;


	class JS_Object
	{
		std::unordered_map<s3d::String, JS_Object *> properties;


		JS_Object *parent;

	public:


		virtual JS_Object *toPointer()
		{


		}


	};



	/*
	class JS_Value
	{

		s3d::Array<JS_Value *> properties;

	public:

		JS_Object operator[](const s3d::String &name)
		{

			const auto obj = JS_Object();
			return obj;
		}

	};




	class JS_Object : public JS_Value
	{




	public:

		v8::Local<v8::ObjectTemplate> toJS()
		{

		}





	};

	*/



	v8::Local<v8::Context> context;


	struct CompileResult
	{



	};







}

#include "Test.hpp"




/*
class C_B
{
	v8::Context::Scope scope;
public:
	C_B(v8::Local<v8::Context> isolate) : scope(isolate)
	{

	}
};
*/

#include <future>

/*
v8::Local<v8::Promise::Resolver> PR;

std::future<void> UCB;

bool ucb = false;

void BBB(const v8::FunctionCallbackInfo<v8::Value> &args)
{


	const auto isolate = args.GetIsolate();

	auto PR = Promise::Resolver::New(args.GetIsolate());

	Local < Object > o;
	// Sleep(1);

	UCB = std::async(std::launch::deferred, [isolate, &PR] {


		// v8::Isolate::Scope isolateScope(isolate);


		PR->Resolve(ToNumberV8(Random() * 20.0));

	});
	ucb = true;

	args.GetReturnValue().Set(PR->GetPromise());

}
*/

#include <sstream>



template<class T>
class Instance
{

	T instance;


public:

	template<class ...Types>
	Instance(Types&&... args) : instance(std::forward<Types>(args)...)
	{

	}

};



#include "JavaScript.hpp"











v8::Persistent<v8::Promise::Resolver> pvv;

void PPP(const v8::FunctionCallbackInfo<v8::Value> &args)
{

	const auto isolate = args.GetIsolate();

	auto resolver = Promise::Resolver::New(isolate);
	
	pvv.Reset(isolate, resolver);

	v8::Local<v8::Promise::Resolver> local = v8::Local<v8::Promise::Resolver>::New(isolate, pvv);


	args.GetReturnValue().Set(local->GetPromise());

}



void Main()
{



	Window::Resize(800, 600);
	Graphics::SetBackground(Color(0x333333));
	const Font font(12);


	JavaScript js;

	Window::SetTitle(L"Siv3D App - V8 ", js.version);


	//	js.root()[L"siv3d"] = jsiv8::JS_Object();

	const auto js_system = v8::ObjectTemplate::New(js.isolate);

	js.globalObject->Set(
		ToStringV8(L"system"),
		js_system);





	js_system->Set(
		ToStringV8(L"update"),
		v8::FunctionTemplate::New(js.isolate, PPP));



	const auto source = TextReader(L"test.js").readAll();
	const auto result = js.compile(source);




	// v8::V8::TerminateExecution(js.isolate);
	// auto func = v8::Local<v8::Promise>::Cast(js.global()[L"system"][L"update"].value);
	// CL(js.global()[L"system"][L"update"].value->IsPromise());
	// func->Resolve(ToBooleanV8(false));
	// PR->Resolve(ToBooleanV8(true));




	js.global()[L"siv3d"][L"main"](L"test");



	while (System::Update())
	{


		// ClearPrint();


		if (!pvv.IsEmpty() && !(System::FrameCount() % 10)) {


			const auto aa = v8::Local<v8::Promise::Resolver>::New(js.isolate, pvv);

			js.global()[L"siv3d"].set(L"frameCount", ToNumberV8(System::FrameCount()));

			aa->Resolve(ToBooleanV8(true));

			//v8::Undefined(js.isolate));
			Isolate::GetCurrent()->RunMicrotasks();

			//Println(L"Resolve", aa.IsEmpty());


			//pvv.Reset();
		}


		const auto rect = font(source).draw(140, 10);

		rect.drawFrame(1, 0, s3d::Color(255, 0, 0));


		// aaa(ToBooleanV8(true));

		const auto y = rect.y + rect.h + 5;

		Line(0, y, Window::Width(), y).draw();

		font(result).draw(140, rect.y + rect.h + 10);

	}



}
