#pragma once
#include "pch.h"
#include "resource.h"

#include "main.h"
#include "Picture Control.h"

class Pad : public TCreateDialog
{
	virtual INT_PTR WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	VOID OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	VOID OnSize(HWND hwnd, UINT state, int cx, int cy);

private:
	HWND hwndComb{};
	HWND hwndButton_Apply{};
	HWND hwndText_Argu{};

public:
	Pad()
	{
		auto& param = AccessParam();
		param.lpTemplateName = MAKEINTRESOURCEW(IDD_FORMVIEW);
	}

public:
	int GetRecommandWidth()
	{
		return dpi(250);
	}
};

class MainWindow : public TWindowPopup
{
	virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
	VOID OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	VOID OnSize(HWND hwnd, UINT state, int cx, int cy);

public:
	MainWindow()
	{
		auto& param = AccessParam();
		param.dwStyle = WS_OVERLAPPEDWINDOW;
		param.x = param.y = param.nWidth = param.nHeight = CW_USEDEFAULT;
		param.strWindowName = app.GetApplicationName();
		param.hMenu = LoadMenuW(HINST, MAKEINTRESOURCEW(IDR_MENU));
	}

private:
	Pad pad;
	PictureControl pc;
};