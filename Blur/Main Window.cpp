#include "pch.h"
#include "resource.h"
#include "Main Window.h"

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hwnd, WM_SIZE, OnSize);
	default:
	{
		if (false) {}
		else
			return DefWindowProcW(hwnd, message, wParam, lParam);
		break;
	}
	}
	return 0;
}
BOOL MainWindow::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	pad.AccessParam().hWndParent = hwnd;
	pad.Create();
	SetWindowLongW(pad.GetHwnd(), GWL_STYLE, WS_CHILD | WS_VISIBLE);

	return TRUE;
}
VOID MainWindow::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDM_NEWWINDOW:
	{
		app.host.NewWindow();
		break;
	}
	case IDM_CLOSE:
	{
		PostMessageW(hwnd, WM_CLOSE, NULL, NULL);
		break;
	}
	case IDM_EXIT:
	{
		app.host.CloseAll();
		break;
	}
	}
}
VOID MainWindow::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	MoveWindow(pad.GetHwnd(), 0, 0, dpi(200), cy, TRUE);
}

INT_PTR Pad::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{

		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
}