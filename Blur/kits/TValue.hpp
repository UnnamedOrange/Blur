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

// TValue

#pragma once

#include "TStdInclude.hpp"

#include "TBlock.hpp"

// ÉùÃ÷
class TValue;
std::ostream& operator<< (std::ostream& out, const TValue& val);
std::wostream& operator<< (std::wostream& out, const TValue& val);

class __TValueBase
{

};
enum TValueType
{
	tNull,
	tInt,
	tStr,
	tList,
};

class TInt : __TValueBase
{
	INT64 data;

public:
	TInt() : data() {}
	TInt(INT64 data) : data(data) {}
	operator INT64& () { return data; }
	operator const INT64& () const { return data; }

	friend std::ostream& operator<< (std::ostream& out, const TInt& val)
	{
		out << val.data;
		return out;
	}
	friend std::wostream& operator<< (std::wostream& out, const TInt& val)
	{
		out << val.data;
		return out;
	}
};
class TStr : __TValueBase, public std::wstring
{
public:
	using std::wstring::wstring;
	TStr(const std::wstring& val) { std::wstring::operator=(val); }

	friend std::wostream& operator<< (std::wostream& out, const TStr& val)
	{
		out << L"\"" << val.c_str() << L"\"";
		return out;
	}
};
class TList : __TValueBase, public std::vector<TValue>
{
public:
	using std::vector<TValue>::vector;

	friend std::ostream& operator<< (std::ostream& out, const TList& val)
	{
		if (!val.size())
			out << "[]";
		else
		{
			out << "[";
			bool bPrinted = false;
			for (const auto& t : val)
			{
				if (bPrinted) out << ", ";
				out << t;
				bPrinted = true;
			}
			out << "]";
		}
		return out;
	}
	friend std::wostream& operator<< (std::wostream& out, const TList& val)
	{
		if (!val.size())
			out << L"[]";
		else
		{
			out << L"[";
			bool bPrinted = false;
			for (const auto& t : val)
			{
				if (bPrinted) out << L", ";
				out << t;
				bPrinted = true;
			}
			out << L"]";
		}
		return out;
	}
};

class TValue
{
	TValueType type{ TValueType::tNull };
public:
	TValueType GetType() const { return type; }

public:
	TValue() = default;

	TValue(TInt val)
	{
		type = TValueType::tInt;
		obj = val;
	}
	TValue(INT64 val)
	{
		type = TValueType::tInt;
		obj = TInt(val);
	}

	TValue(const TList& val)
	{
		type = TValueType::tList;
		obj = val;
	}

	TValue(const TStr& val)
	{
		type = TValueType::tStr;
		obj = val;
	}

private:
	TBlock obj;

protected:
	VOID verifyInt() const
	{
		if (type != TValueType::tInt)
			throw std::runtime_error("type tInt is expected.");
	}
public:
	TInt& interger() { verifyInt(); return *((TInt*)obj.data()); }
	const TInt& interger() const { verifyInt(); return *((TInt*)obj.data()); }

protected:
	VOID verifyStr() const
	{
		if (type != TValueType::tStr)
			throw std::runtime_error("type tStr is expected.");
	}
public:
	TStr& str() { verifyStr(); return *((TStr*)obj.data()); }
	const TStr& str() const { verifyStr(); return *((TStr*)obj.data()); }

protected:
	VOID verifyList() const
	{
		if (type != TValueType::tList)
			throw std::runtime_error("type tList is expected.");
	}
public:
	TList& list() { verifyList(); return *((TList*)obj.data()); }
	const TList& list() const { verifyList(); return *((TList*)obj.data()); }

public:
	friend std::ostream& operator<< (std::ostream& out, const TValue& val)
	{
		switch (val.GetType())
		{
		case TValueType::tInt:
		{
			out << val.interger();
			break;
		}
		case TValueType::tList:
		{
			out << val.list();
			break;
		}
		default:
		{
			throw std::runtime_error("no matched function. wostream is recommanded.");
			break;
		}
		}
		return out;
	}
	friend std::wostream& operator<< (std::wostream& out, const TValue& val)
	{
		switch (val.GetType())
		{
		case TValueType::tInt:
		{
			out << val.interger();
			break;
		}
		case TValueType::tStr:
		{
			out << val.str();
			break;
		}
		case TValueType::tList:
		{
			out << val.list();
			break;
		}
		default:
		{
			throw std::runtime_error("no matched function.");
			break;
		}
		}
		return out;
	}

public:
	bool operator== (const TValue& b) const
	{
		if (type != b.type)
			return false;
		switch (type)
		{
		case tNull:
			return true;
		case tInt:
			return interger() == b.interger();
		case tStr:
			return str() == b.str();
		case tList:
			return list() == b.list();
		}
	}
};