#pragma once
#include "pch.h"
#include "resource.h"

class Host : public TWindowHost
{
	virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

public:
	Host()
	{
		auto& param = AccessParam();
		param.strWindowName = L"Blur Host";
	}

public:
	VOID NewWindow();
	VOID CloseAll()
	{
		for (const auto& t : container)
		{
			PostMessageW(t.first->GetHwnd(), WM_QUIT, NULL, NULL);
		}
	}
};