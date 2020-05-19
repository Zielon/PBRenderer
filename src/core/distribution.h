#pragma once

#include <vector>
#include <numeric>

#include "uniform_sampler.h"

namespace pbr
{
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

			int offset = std::clamp(
				int(std::distance(cdf.begin(), std::upper_bound(cdf.begin(), cdf.end(), u))) - 1,
				0, int(cdf.size()) - 2);

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
			return int(func.size());
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

			std::vector<float> marginal_func;

			for (auto& f : func)
			{
				pConditionalV.emplace_back(new Distribution1D(f));
			}

			for (auto& conditional : pConditionalV)
			{
				marginal_func.push_back(conditional->funcInt);
			}

			pMarginal.reset(new Distribution1D(marginal_func));
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

			int iu = std::clamp(int(p[0] * pConditionalV[0]->count()), 0, pConditionalV[0]->count() - 1);
			int iv = std::clamp(int(p[1] * pMarginal->count()), 0, pMarginal->count() - 1);
			return pConditionalV[iv]->func[iu] / pMarginal->funcInt;
		};

		std::shared_ptr<Distribution1D> pMarginal;
		std::vector<std::shared_ptr<Distribution1D>> pConditionalV;
	};
}
