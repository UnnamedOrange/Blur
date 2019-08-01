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
		else if (message == WM_BLURFINISH)
		{
			pc.SetPicture((HBITMAP)wParam);
			WCHAR buf[256];
			swprintf_s(buf, L"在 %u ms 内搞完了！", (DWORD)lParam);
			MessageBoxW(hwnd, buf, L"提示", MB_ICONINFORMATION);
		}
		else if (message == WM_GETPICTURE)
		{
			return (LRESULT)pc.GetPicture();
		}
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

INT_PTR CALLBACK Pad::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hwnd, WM_SIZE, OnSize);
		HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
	default:
	{
		if (false) {}
		else if (message == WM_BLURFINISH)
		{
			PostMessageW(GetParent(hwnd), WM_BLURFINISH, wParam, lParam);
		}
		else
			return FALSE;
	}
	}
	return TRUE;
}
BOOL Pad::OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hwndComb = GetDlgItem(hwnd, IDC_COMBO1);
	hwndButton_Apply = GetDlgItem(hwnd, IDC_BUTTON1);
	hwndText_Argu = GetDlgItem(hwnd, IDC_EDIT1);

	for (int i = 0; i < collection.size(); i++)
	{
		WCHAR buf[10];
		swprintf_s(buf, L"%d. ", i + 1);
		ComboBox_AddString(hwndComb, (std::wstring(buf) + collection[i].GetName()).c_str());
	}
	ComboBox_SetCurSel(hwndComb, 0);
	SetWindowTextW(hwndText_Argu,
		collection[0].GetDefaultArguments().c_str());
	return TRUE;
}
VOID Pad::OnDestroy(HWND hwnd)
{
	collection.Terminate();
}
VOID Pad::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_COMBO1:
	{
		if (codeNotify == LBN_SELCHANGE)
		{
			SetWindowTextW(hwndText_Argu,
				collection[ComboBox_GetCurSel(hwndComb)].GetDefaultArguments().c_str());
		}
		break;
	}
	case IDC_BUTTON1:
	{
		if (codeNotify == BN_CLICKED)
		{
			collection.Terminate();

			std::vector<WCHAR> buf((size_t)GetWindowTextLengthW(hwndText_Argu) + 1);
			GetWindowTextW(hwndText_Argu, buf.data(), buf.size());
			collection[ComboBox_GetCurSel(hwndComb)].Run(buf.data(), hwnd,
				(HBITMAP)SendMessageW(GetParent(hwnd), WM_GETPICTURE, NULL, NULL));
		}
		break;
	}
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