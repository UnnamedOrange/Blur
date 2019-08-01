// Copyright (c) 2018-2019 Orange Software
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// TOpenSaveDialog

#pragma once

#include "TStdInclude.hpp"

class TOpenSaveDialog : protected OPENFILENAMEW
{
	std::vector<WCHAR> buffer;

public:
	TOpenSaveDialog()
	{
		lStructSize = sizeof(OPENFILENAMEW);	// 结构大小
		hwndOwner = NULL;						// 对话框拥有者，可为空
		hInstance = NULL;						// （始终为 NULL）使用系统对话框保持为 NULL
		lpstrFilter = L"所有文件\0*.*\0\0";		// 筛选器，可为空，为空时快捷方式不指向。结尾为两个 \0，各项用 \0 分隔。注意不能用 std::wstring
		lpstrCustomFilter = nullptr;			// （始终为 nullptr）保持空，用户自定义过滤器
		nMaxCustFilter = NULL;					// （始终为 NULL）因 lpstrCustomFilter 为 NULL 而保持 NULL
		nFilterIndex = 1;						// 过滤器索引，从 1 开始，0 表示用户自定义过滤器
		lpstrFile = nullptr;					// 获取的文件名的缓冲，多个文件用 \0(OFN_EXPLORER) 或者空格 (!OFN_EXPLORER) 分割，若缓冲区太小，前两个字节保存了需要的缓冲区大小
		nMaxFile = NULL;						// 缓冲区长度，至少 256
		lpstrFileTitle = nullptr;				// 仅包含文件名或扩展名，不含路径名的缓冲，可为 nullptr
		nMaxFileTitle = NULL;					// 缓冲区长度
		lpstrInitialDir = nullptr;				// 初始目录
		lpstrTitle = nullptr;					// 对话框标题，可为空，默认为保存，打开
		Flags = NULL;							// 标记
		// OFN_ALLOWMULTISELECT	多文件
		// OFN_CREATEPROMPT		提醒是否新建
		// OFN_OVERWRITEPROMPT	提醒是否覆盖
		// OFN_ENABLESIZING		允许调整大小，仅限OFN_EXPLORER
		// OFN_EXPLORER			打开和另存为默认风格
		// OFN_FILEMUSTEXIST	文件必须存在
		nFileOffset = NULL;						// 文件名相对路径名的开始下标，对于多文件对话框，其值为第一个路径的对应值
		nFileExtension = NULL;					// 扩展名相对路径名的开始下标，不含.，同上
		lpstrDefExt = NULL;						// 默认扩展名，不加.，仅前三个字符有效
		lCustData = NULL;						// （始终为 NULL）用于钩子程序，不用
		lpfnHook = nullptr;						// （始终为 nullptr）用于钩子程序，不用
		lpTemplateName = nullptr;				// （始终为 nullptr）用于自定义对话框，不用
		pvReserved = NULL;						// 保留
		dwReserved = NULL;						// 保留
		FlagsEx = NULL;							// 扩展标记
	}

public:
	HWND hwndParent = nullptr;
	LPCWSTR& strFilter = lpstrFilter;
	std::wstring strTitle = L"";

public:
	std::pair<BOOL, std::wstring> OpenFile(DWORD nBufferMax = 256)
	{
		buffer.resize(nBufferMax);
		lpstrFile = buffer.data();
		nMaxFile = (DWORD)buffer.size();

		hwndOwner = hwndParent;
		lpstrTitle = strTitle.c_str();
		Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_FILEMUSTEXIST;

		BOOL ret = GetOpenFileNameW(this);
		return std::make_pair(ret, std::wstring(lpstrFile));
	}
	std::pair<BOOL, std::wstring> SaveFile(DWORD nBufferMax = 256)
	{
		buffer.resize(nBufferMax);
		lpstrFile = buffer.data();
		nMaxFile = (DWORD)buffer.size();

		hwndOwner = hwndParent;
		lpstrTitle = strTitle.c_str();
		Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;

		BOOL ret = GetSaveFileNameW(this);
		return std::make_pair(ret, std::wstring(lpstrFile));
	}
};