#pragma once

#include "include/libplatform/libplatform.h"
using namespace v8;
class CV8Initializer
{
public:
	
	CV8Initializer()
	{
		V8::InitializeICU();
		V8::InitializeExternalStartupData("./");
		m_pPlatform = platform::CreateDefaultPlatform();
		V8::InitializePlatform(m_pPlatform);
		V8::Initialize();
	}

	~CV8Initializer()
	{
		V8::Dispose();
		V8::ShutdownPlatform();
		delete m_pPlatform;
	}


private:
	v8::Platform* m_pPlatform;
};