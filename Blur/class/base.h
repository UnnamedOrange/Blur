#pragma once
#include "../pch.h"

namespace Blur
{
	class Base
	{
	protected:
		std::unordered_map<std::wstring, TValue> dict;
	private:
		VOID AnalyzeArgu(const std::wstring& argu)
		{
			dict.clear();

			std::wstring switched;
			WCHAR last(-1);
			for (WCHAR ch : argu)
			{
				if (ch != L'-' || last != L'-')
					switched.push_back(ch);
				last = ch;
			}

			std::wstring buf;
			bool isInKey = false;
			std::wstring crtKey;
			auto _handle = [&]()
			{
				if (buf.empty())
					return;

				if (isInKey)
				{
					if (dict.count(buf))
					{
						throw std::logic_error("repeated key");
						return;
					}
					crtKey = buf;
					dict[crtKey] = TList();
				}
				else
				{
					if (crtKey.empty())
					{
						throw std::logic_error("no current key");
						return;
					}
					bool isNum = true;
					for (auto ch : buf)
						if (!std::isdigit(ch))
						{
							isNum = false;
							break;
						}
					if (isNum)
					{
						TInt t = 0;
						for (auto ch : buf)
							t = t * 10 + (ch - L'0');
						dict[crtKey].list().push_back(t);
					}
					else
						dict[crtKey].list().push_back(TStr(buf));
				}
			};

			for (WCHAR ch : switched)
			{
				if (ch == ' ')
				{
					_handle();
					isInKey = false;
					buf.clear();
					continue;
				}
				if (ch == '-')
				{
					isInKey = true;
					buf.clear();
					crtKey.clear();
					continue;
				}

				buf.push_back(ch);
			}
			_handle();
		}
		virtual VOID _Run() = 0;
		HWND hwndNotify{};
		UINT WM_BLURFINISH = TMessage::Register(L"WM_BLURFINISH");
	protected:
		HBITMAP origin{};
		HBITMAP result{};
	private:
		HANDLE hThread{};
		DWORD dwTime{};
		static DWORD WINAPI Thread(LPVOID lpThreadParameter)
		{
			Base& b = *((Base*)lpThreadParameter);
			b._Run();
			b.Finish();
			return 0;
		}
		VOID Finish()
		{
			FILETIME unused, time1, time2;
			GetThreadTimes(hThread, &unused, &unused, &time1, &time2);
			DWORD64 t1 = ((DWORD64)time1.dwHighDateTime << 32) + time1.dwLowDateTime;
			DWORD64 t2 = ((DWORD64)time2.dwHighDateTime << 32) + time2.dwLowDateTime;
			dwTime = DWORD((t1 + t2) / 10000);
			CloseHandle(hThread);
			hThread = nullptr;
			HBITMAP re = result;
			result = nullptr;
			PostMessageW(hwndNotify, WM_BLURFINISH, WPARAM(re), LPARAM(dwTime));
		}

	public:
		VOID Terminate()
		{
			if (hThread)
			{
				TerminateThread(hThread, 0);
				WaitForSingleObject(hThread, INFINITE);
				CloseHandle(hThread);
				hThread = nullptr;

				if (result)
				{
					DeleteObject(result);
					result = nullptr;
				}
			}
		}

	public:
		VOID Run(const std::wstring& argu, HWND hwndNotify, HBITMAP hOrigin)
		{
			if (hThread)
				return;
			if (!hOrigin)
				return;
			AnalyzeArgu(argu);
			this->hwndNotify = hwndNotify;
			this->origin = hOrigin;
			this->result = nullptr;
			hThread = CreateThread(nullptr, NULL, Thread, this, NULL, nullptr);
		}
		HBITMAP GetResult()
		{
			if (!result)
				throw std::logic_error("fail to get result");
			HBITMAP bRet = result;
			result = nullptr;
			return bRet;
		}
		virtual std::wstring GetDefaultArguments() const = 0;
		virtual std::wstring GetName() const = 0;
	};
}