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

// TCollection

#pragma once
#include "TStdInclude.hpp"

class TCollection final
{
	// e.g. "C:\\Users\\Some User\\AppData\\Local"
	std::wstring GetLocal()
	{
		PWSTR pointer = nullptr;
		SHGetKnownFolderPath(FOLDERID_LocalAppData, NULL, NULL, &pointer);
		std::wstring ret(pointer);
		CoTaskMemFree(pointer);
		return ret;
	}
	std::wstring strLocal = GetLocal();

	std::wstring strRoot;

public:
	TCollection() = default;
	explicit TCollection(const std::wstring& root)
	{
		SetRoot(root);
	}
	// e.g. "root"
	VOID SetRoot(const std::wstring& root)
	{
		strRoot = strLocal + L"\\" + root;
	}
	// e.g. "C:\\Users\\Some User\\AppData\\Local\\root"
	std::wstring GetRoot()
	{
		if (strRoot.empty())
			throw std::logic_error("(SetRoot) before (GetRoot).");

		return strRoot;
	}

	// e.g. "(\\)data\\1.dat" -> "C:\\Users\\Some User\\AppData\\Local\\root\\data\\1.dat"
	std::wstring GetPath(std::wstring end)
	{
		if (strRoot.empty())
			throw std::logic_error("(SetRoot) before (GetPath).");

		if (end.front() != L'\\')
			end = std::wstring(L"\\") + end;
		return strRoot + end;
	}

	// e.g. "(\\)data" -> "C:\\Users\\Some User\\AppData\\Local\\root\\data"
	VOID CreateDirectoryOnRoot(std::wstring end)
	{
		if (strRoot.empty())
			throw std::logic_error("(SetRoot) before (CreateDirectoryOnRoot).");

		if (end.front() != L'\\')
			end = std::wstring(L"\\") + end;
		if (end.back() != L'\\')
			end.push_back(L'\\');

		std::wstring temp;
		temp.reserve(end.size());
		for (auto ch : end)
		{
			if (ch == '\\')
			{
				if (!(CreateDirectoryW((strRoot + temp).c_str(), nullptr) ||
					GetLastError() == ERROR_ALREADY_EXISTS))
					throw std::runtime_error("fail to create path.");
			}
			temp.push_back(ch);
		}
	}
};