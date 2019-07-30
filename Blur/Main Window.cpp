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

	pc.AccessParam().hWndParent = hwnd;
	pc.Create();
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
	case IDM_OPEN:
	{
		TOpenSaveDialog open;
		open.hwndParent = hwnd;
		open.strFilter = L"图片\0*.jpg;*.jpeg;*.png;*.bmp;*.gif\0所有文件\0*.*\0\0";
		auto t = open.OpenFile();
		if (t.first)
			pc.OpenPicture(t.second);
		break;
	}
	}
}
VOID MainWindow::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	int iPadWidth = pad.GetRecommandWidth();
	MoveWindow(pad.GetHwnd(), 0, 0, iPadWidth, cy, TRUE);
	MoveWindow(pc.GetHwnd(), iPadWidth, 0, cx - iPadWidth, cy, TRUE);
}

INT_PTR Pad::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hwnd, WM_SIZE, OnSize);
	default:
		return FALSE;
	}
	return TRUE;
}
BOOL Pad::OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hwndComb = GetDlgItem(hwnd, IDC_COMBO1);
	hwndButton_Apply = GetDlgItem(hwnd, IDC_BUTTON1);
	hwndText_Argu = GetDlgItem(hwnd, IDC_EDIT1);
	ComboBox_AddString(hwndComb, L"1. 高斯模糊");
	ComboBox_SetCurSel(hwndComb, 0);
	return TRUE;
}
VOID Pad::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	default:
		break;
	}
}
VOID Pad::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	RECT rect;
	GetWindowRect(hwndComb, &rect);
	SetWindowPos(hwndComb, nullptr, NULL, NULL,
		cx, rect.bottom - rect.top, SWP_NOMOVE);

	GetWindowRect(hwndButton_Apply, &rect);
	SetWindowPos(hwndButton_Apply, nullptr,
		cx - (rect.right - rect.left) - dpi(16),
		cy - (rect.bottom - rect.top) - dpi(16),
		NULL, NULL, SWP_NOSIZE);
	GetWindowRect(hwndButton_Apply, &rect);
	int iButtonTop = rect.top;

	GetWindowRect(hwndText_Argu, &rect);
	SetWindowPos(hwndText_Argu, nullptr, NULL, NULL,
		cx, cy - rect.top - (cy - iButtonTop) - dpi(16),
		SWP_NOMOVE);
}