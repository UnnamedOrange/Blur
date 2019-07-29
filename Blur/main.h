#pragma once
#include "pch.h"
#include "resource.h"

#include "Host.h"

class Demo : public TApplication
{
	TGdiplus __unused;

public:
	Demo() : TApplication(L"Blur", L"41D69ADB-6D53-459A-A6F8-E0C5219FE820") {}

public:
	Host host;

public:
	virtual INT Execute() override;
};

extern Demo app;