#pragma once

namespace pbr
{
	template <typename Iterator>
	Iterator select_randomly(Iterator start, Iterator end){
		static std::random_device rd;
		static std::mt19937 gen(rd());
		const std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
		std::advance(start, dis(gen));
		return start;
	}
}
