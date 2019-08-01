#include "pch.h"
#include "resource.h"
#include "Picture Control.h"

LRESULT PictureControl::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
	case WM_LBUTTONDBLCLK:
	{
		PostMessage(GetParent(hwnd), WM_COMMAND, IDM_OPEN, NULL);
		break;
	}
	default:
		return DefWindowProcW(hwnd, message, wParam, lParam);
	}
	return 0;
}
BOOL PictureControl::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	SetClassLongW(hwnd, GCL_STYLE, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
	return TRUE;
}

VOID PictureControl::ShowInfo(Gdiplus::Graphics& g, const std::wstring& str)
{
	using namespace Gdiplus;
	FontFamily ff(L"Segoe UI");
	StringFormat sf;
	sf.SetAlignment(StringAlignment::StringAlignmentCenter);
	sf.SetLineAlignment(StringAlignment::StringAlignmentCenter);
	g.DrawString(str.c_str(), str.length(), &Font(&ff, dpi(24.0), 0, Unit::UnitPixel),
		RectF(0, 0, iWidth, iHeight), &sf, &SolidBrush(Color::Black));
}
HBITMAP PictureControl::Paint()
{
	HDC hdc = GetDC(NULL);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);
	ReleaseDC(NULL, hdc);
	hdc = CreateCompatibleDC(NULL);
	DeleteObject(SelectObject(hdc, hBitmap));

	using namespace Gdiplus;
	Graphics g(hdc);
	g.SetCompositingQuality(CompositingQuality::CompositingQualityHighQuality);
	g.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAliasGridFit);
	g.FillRectangle(&SolidBrush(Color::White), Rect(-1, -1, iWidth + 1, iHeight + 1));

	if (!hCrtBitmap)
	{
		ShowInfo(g, L"没有打开图片");
	}
	else
	{
		int cxMargin = iWidth - dpi(32);
		int cyMargin = iHeight - dpi(32);
		BITMAP bitmap{};
		if (!GetObjectW(hCrtBitmap, sizeof(bitmap), &bitmap))
			throw std::runtime_error("fail to get the bitmap");
		if (!(bitmap.bmWidth * bitmap.bmHeight))
		{
			ShowInfo(g, L"图片的面积为 0");
		}
		else
		{
			int cxNow = 0;
			int cyNow = 0;
			if (bitmap.bmWidth <= cxMargin && bitmap.bmHeight <= cyMargin)
			{
				cxNow = bitmap.bmWidth;
				cyNow = bitmap.bmHeight;
			}
			if (bitmap.bmWidth >= bitmap.bmHeight)
			{
				cxNow = cxMargin;
				cyNow = MulDiv(bitmap.bmHeight, cxNow, bitmap.bmWidth);
			}
			else
			{
				cyNow = cyMargin;
				cxNow = MulDiv(bitmap.bmWidth, cyNow, bitmap.bmHeight);
			}
			HDC hdcMem = CreateCompatibleDC(hdc);
			DeleteObject(SelectObject(hdcMem, hCrtBitmap));
			SetStretchBltMode(hdc, COLORONCOLOR);
			SetBrushOrgEx(hdc, 0, 0, nullptr);
			StretchBlt(hdc, (iWidth - cxNow) / 2, (iHeight - cyNow) / 2, cxNow, cyNow,
				hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
			DeleteDC(hdcMem);
		}
	}

	DeleteDC(hdc);
	return hBitmap;
}
VOID PictureControl::OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	HBITMAP hBitmap = Paint();
	HDC hdcMem = CreateCompatibleDC(ps.hdc);
	DeleteObject(SelectObject(hdcMem, hBitmap));
	BitBlt(ps.hdc, 0, 0, iWidth, iHeight, hdcMem, 0, 0, SRCCOPY);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
	EndPaint(hwnd, &ps);
}

BOOL PictureControl::OpenPicture(const std::wstring& strPath)
{
	using namespace Gdiplus;
	Image image(strPath.c_str());
	if (!(image.GetWidth() * image.GetHeight()))
		return FALSE;
	HDC hdc = GetDC(NULL);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, image.GetWidth(), image.GetHeight());
	ReleaseDC(NULL, hdc);
	hdc = CreateCompatibleDC(NULL);
	DeleteObject(SelectObject(hdc, hBitmap));
	Graphics g(hdc);
	g.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
	DeleteDC(hdc);

	if (hCrtBitmap)
		DeleteObject(hCrtBitmap);
	hCrtBitmap = hBitmap;
	InvalidateRect(GetHwnd(), NULL, FALSE);
	return TRUE;
}
VOID PictureControl::SetPicture(HBITMAP hBitmap)
{
	DeleteObject(hCrtBitmap);
	hCrtBitmap = hBitmap;
	InvalidateRect(GetHwnd(), NULL, TRUE);
}
HBITMAP PictureControl::GetPicture()
{
	return hCrtBitmap;
}