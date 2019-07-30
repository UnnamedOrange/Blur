#include "pch.h"
#include "resource.h"
#include "Host.h"

#include "Main Window.h"

LRESULT CALLBACK Host::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		return HANDLE_WM_CREATE(hwnd, wParam, lParam,
			[this](HWND hwnd, LPCREATESTRUCT lpCreateStruct)->BOOL
			{
				NewWindow();
				return TRUE;
			});
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
	{
		if (false) {}
		else if (message == WM_ACTIVATEHOST)
		{
			NewWindow();
		}
		else
			return DefWindowProcW(hwnd, message, wParam, lParam);
	}
	}
	return 0;
}

VOID Host::NewWindow()
{
	auto t = Push(new MainWindow);
	t->Create();
	ShowWindow(t->GetHwnd(), SW_SHOW);
	UpdateWindow(t->GetHwnd());
	SetForegroundWindow(t->GetHwnd());
	SetFocus(t->GetHwnd());
}