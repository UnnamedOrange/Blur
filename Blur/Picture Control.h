#pragma once
#include "pch.h"
#include "resource.h"

class PictureControl : public TWindowChild
{
	virtual LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);

private:
	VOID ShowInfo(Gdiplus::Graphics& g, const std::wstring& str);
	HBITMAP Paint();
	VOID OnPaint(HWND hwnd);

public:
	PictureControl()
	{
		auto& param = AccessParam();
		param.strWindowName = L"Picture Control";
	}

private:
	HBITMAP hCrtBitmap = nullptr;
public:
	BOOL OpenPicture(const std::wstring& strPath);
	HBITMAP DuplicateCrtBitmap()
	{

	}
};