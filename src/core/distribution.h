#pragma once

#include <utility>
#include <vector>
#include <numeric>

#include "uniform_sampler.h"

namespace pbr
{
	class Distribution1D final
	{
	public:
		Distribution1D() = default;

		explicit Distribution1D(const std::vector<float>& pdf){

			cdf.resize(pdf.size());
			std::partial_sum(pdf.begin(), pdf.end(), cdf.begin());
			count = cdf.back();
		}

		int sample(const float u) const{

			const auto end = std::upper_bound(cdf.begin(), cdf.end(), u * count);
			return std::distance(cdf.begin(), end);
		};

		float get_pdf(const int x) const{

			return (x == 0 ? cdf[0] : cdf[x] - cdf[x - 1]) / count;
		};

		float count{};

	private:
		std::vector<float> cdf;
	};

	/*
	 * The Piecewise-Constant Sampling Distribution for a High-Dynamic-Range Environment Map
	 */
	class Distribution2D final
	{
	public:
		explicit Distribution2D(const std::vector<std::vector<float>>& pdfs){

			std::vector<float> column_sums;

			for (const auto& pdf : pdfs)
			{
				// Compute conditional sampling distribution for v
				row_samplers.emplace_back(pdf);

				// Compute marginal sampling distribution u
				column_sums.push_back(row_samplers.back().count);
			}

			column_sampler = Distribution1D(column_sums);
		}

		/*
		 * First the marginal 1D distribution is used to select a v value, giving a columns of the image to sample. 
		 * Columns with bright pixels are more likely to be sampled. 
		 * Then, given a column, a value u is sampled from that column’s 1D distribution.
		 */
		std::pair<int, int> sample(const glm::vec2& u) const{

			auto column = column_sampler.sample(u.x);
			return std::make_pair(column, row_samplers[column].sample(u.y));
		};

		float get_pdf(const int x, const int y) const{

			return column_sampler.get_pdf(x) * row_samplers[x].get_pdf(y);
		};

	private:
		Distribution1D column_sampler{};
		std::vector<Distribution1D> row_samplers;
	};
}
