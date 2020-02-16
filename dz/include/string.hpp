#ifndef DZ_STRING_HPP_
#define DZ_STRING_HPP_

#include <string>
#include <vector>

#include "macros.hpp"

namespace dz
{

	class String final : public std::string
	{
		static constexpr const char* ws = " \t\n\r\f\v";

	public:
		String()                       : std::string{""}  {}
		String(const char *str)        : std::string{str} {}
		String(const std::string &str) : std::string{str} {}

		DZ_NODISCARD auto split(const String &delims = " ") -> std::vector<String>
		{
			std::vector<String> output{};

			int first = 0;
			while (first < this->size()) {
				const auto second = this->find_first_of(delims, first);
				if (first != second)
					output.emplace_back(this->substr(first, second - first));
				if (second == std::string::npos) break;
				first = second + 1;
			}
			return output;
		}

		DZ_NODISCARD auto replace(const String &from, const String &to) -> String
		{
			std::string str = *this;
			std::string::size_type n = 0;
			while ((n = str.find(from, n)) != std::string::npos) {
					str.replace(n, from.size(), to);
					n += to.size();
			}
			return str;
		}

		auto rtrim(const char* t = ws) -> void
		{
				this->erase(this->find_last_not_of(t) + 1);
		}

		auto ltrim(const char* t = ws) -> void
		{
				this->erase(0, this->find_first_not_of(t));
		}

		auto trim(const char* t = ws) -> void
		{
			ltrim(t);
			rtrim(t);
		}

	};

	DZ_NODISCARD inline auto join (const String &delim, const std::vector<String> &v) -> String
	{
		if (!v.size()) return "";
		String s;
		for (int i = 0; i < v.size() - 1; i++)
			s+= v[i] + delim;
		s += v[v.size() - 1];
		return s;
	}

} // namespace dz

#endif // DZ_STRING_HPP_
