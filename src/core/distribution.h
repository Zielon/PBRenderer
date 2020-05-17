#pragma once

#include <utility>
#include <vector>
#include <numeric>

#include "uniform_sampler.h"

namespace pbr
{
	template <typename T, typename U, typename V>
	T Clamp(T val, U low, V high){
		if (val < low)
			return low;
		if (val > high)
			return high;
		return val;
	}

	template <typename Predicate>
	int FindInterval(int size, const Predicate& pred){
		int first = 0, len = size;
		while (len > 0)
		{
			int half = len >> 1, middle = first + half;
			// Bisect range based on value of _pred_ at _middle_
			if (pred(middle))
			{
				first = middle + 1;
				len -= half + 1;
			}
			else
				len = half;
		}
		return Clamp(first - 1, 0, size - 2);
	}

	class Distribution1D final
	{
	public:
		Distribution1D() = default;

		explicit Distribution1D(const std::vector<float>& f): cdf(f.size() + 1){

			func = f;
			cdf[0] = 0;
			auto n = f.size();
			for (int i = 1; i < n + 1; ++i) cdf[i] = cdf[i - 1] + func[i - 1] / n;
			funcInt = cdf[n];
			if (funcInt == 0)
			{
				for (int i = 1; i < n + 1; ++i) cdf[i] = float(i) / float(n);
			}
			else
			{
				for (int i = 1; i < n + 1; ++i) cdf[i] /= funcInt;
			}
		}

		float sample(const float u, float* pdf, int* off = nullptr) const{

			int offset = FindInterval((int)cdf.size(), [&](int index){
				return cdf[index] <= u;
			});

			if (off) *off = offset;

			float du = u - cdf[offset];
			if ((cdf[offset + 1] - cdf[offset]) > 0)
			{
				du /= (cdf[offset + 1] - cdf[offset]);
			}

			if (pdf) *pdf = (funcInt > 0) ? func[offset] / funcInt : 0;
			return (float(offset) + du) / float(func.size());
		};

		float funcInt{};

		int count() const{
			return (int)func.size();
		}

		std::vector<float> cdf;
		std::vector<float> func;
	};

	/*
	 * The Piecewise-Constant Sampling Distribution for a High-Dynamic-Range Environment Map
	 */
	class Distribution2D final
	{
	public:
		explicit Distribution2D(const std::vector<std::vector<float>>& func){

			std::vector<float> marginalFunc;

			for (auto& f : func)
			{
				pConditionalV.emplace_back(new Distribution1D(f));
			}

			for (auto& c : pConditionalV)
			{
				marginalFunc.push_back(c->funcInt);
			}

			pMarginal.reset(new Distribution1D(marginalFunc));
		}

		glm::vec2 sample(const glm::vec2& u, float* pdf) const{

			float pdfs[2];
			int v;
			float d1 = pMarginal->sample(u[1], &pdfs[1], &v);
			float d0 = pConditionalV[v]->sample(u[0], &pdfs[0]);
			*pdf = pdfs[0] * pdfs[1];
			return glm::vec2(d0, d1);
		};

		float get_pdf(const glm::vec2& p) const{

			int iu = Clamp(int(p[0] * pConditionalV[0]->count()), 0,
			               pConditionalV[0]->count() - 1);
			int iv =
				Clamp(int(p[1] * pMarginal->count()), 0, pMarginal->count() - 1);
			return pConditionalV[iv]->func[iu] / pMarginal->funcInt;
		};

		std::shared_ptr<Distribution1D> pMarginal;
		std::vector<std::shared_ptr<Distribution1D>> pConditionalV;
	};
}
