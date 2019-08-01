#pragma once
#include "../pch.h"

#include "base.h"
#include "Gauss Blur.h"

namespace Blur
{
	class BlurCollection : protected std::vector<Base*>
	{
		GaussBlur b1;

	public:
		BlurCollection() : std::vector<Base*>({ &b1 }) {}
		size_t size() const
		{
			return std::vector<Base*>::size();
		}
		Base& operator[](size_t idx)
		{
			return *std::vector<Base*>::at(idx);
		}
		const Base& operator[](size_t idx) const
		{
			return *std::vector<Base*>::at(idx);
		}

		VOID Terminate()
		{
			for (auto t : *this)
				t->Terminate();
		}
	};
}