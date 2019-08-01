#pragma once
#include "base.h"

namespace Blur
{
	class GaussBlur : public Base
	{
		virtual VOID _Run() override
		{
			BITMAP bitmapOrigin;
			GetObjectW(origin, sizeof(BITMAP), &bitmapOrigin);
			std::vector<UINT32> bits((size_t)bitmapOrigin.bmWidth * bitmapOrigin.bmHeight);
			GetBitmapBits(origin, bitmapOrigin.bmWidthBytes * bitmapOrigin.bmHeight, bits.data());

			int r = dict[L"radius"].list()[0].interger();

			// 对于边缘，用平铺的方式解决
			int cx = bitmapOrigin.bmWidth;
			int cy = bitmapOrigin.bmHeight;
			auto locate = [&](int x, int y)
			{
				if (x < 0)
					x += cx;
				else if (x >= cx)
					x -= cx;
				if (y < 0)
					y += cy;
				else if (y >= cy)
					y -= cy;
				return bitmapOrigin.bmWidth * y + x;
			};

			// 填充
			std::vector<COLORREF> bits1(bits.size());
			std::vector<double> Gauss(r * 3 + 1); // 3-sigma principle
			constexpr double sqrt2pi = 2.5066282746310002;
			const double mul = 1 / (sqrt2pi * r);
			double powmul = -1 / (2.0 * r * r);
			for (int i = 0; i <= r * 3; i++)
				Gauss[i] = mul * std::exp(powmul * i * i);

			// RGB
			auto GetR = [](COLORREF color) -> double { return color & 0xff; };
			auto GetG = [](COLORREF color) -> double { return (color >> 8) & 0xff; };
			auto GetB = [](COLORREF color) -> double { return (color >> 16) & 0xff; };
			auto GetRGB = [](BYTE R, BYTE G, BYTE B) { return (COLORREF)((int)R | ((int)G << 8) | ((int)B << 16) | (0xff << 24)); };

			// 横向
			for (int y = 0; y < cy; y++)
			{
				for (int x = 0; x < cx; x++)
				{
					int idx = locate(x, y);
					COLORREF t = bits[idx];
					double mul = Gauss[0];
					double R = GetR(t) * mul;
					double G = GetG(t) * mul;
					double B = GetB(t) * mul;
					for (int i = 1; i <= r * 3; i++)
					{
						int idx1 = locate(x - i, y);
						int idx2 = locate(x + i, y);
						mul = Gauss[i];
						R += (GetR(bits[idx1]) + GetR(bits[idx2])) * mul;
						G += (GetG(bits[idx1]) + GetG(bits[idx2])) * mul;
						B += (GetB(bits[idx1]) + GetB(bits[idx2])) * mul;
					}
					bits1[idx] = GetRGB(R, G, B);
				}
			}

			// 纵向
			for (int x = 0; x < cx; x++)
			{
				for (int y = 0; y < cy; y++)
				{
					int idx = locate(x, y);
					COLORREF t = bits1[idx];
					double mul = Gauss[0];
					double R = GetR(t) * mul;
					double G = GetG(t) * mul;
					double B = GetB(t) * mul;
					for (int i = 1; i <= r * 3; i++)
					{
						int idx1 = locate(x, y - i);
						int idx2 = locate(x, y + i);
						mul = Gauss[i];
						R += (GetR(bits1[idx1]) + GetR(bits1[idx2])) * mul;
						G += (GetG(bits1[idx1]) + GetG(bits1[idx2])) * mul;
						B += (GetB(bits1[idx1]) + GetB(bits1[idx2])) * mul;
					}
					bits[idx] = GetRGB(R, G, B);
				}
			}

			result = CreateBitmap(bitmapOrigin.bmWidth, bitmapOrigin.bmHeight,
				bitmapOrigin.bmPlanes, 32, bits.data());
		}
		virtual std::wstring GetDefaultArguments() const override
		{
			return L"-radius 5";
		}
		virtual std::wstring GetName() const override
		{
			return L"高斯模糊";
		}
	};
}