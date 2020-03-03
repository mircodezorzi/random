#ifndef DZ_STRING_HPP_
#define DZ_STRING_HPP_

#include <string>
#include <vector>

namespace dz
{

	class String final : public std::string
	{
		static constexpr const char* ws = " \t\n\r\f\v";
		using std::string::replace;

	public:
		String()                       : std::string{""}  {}
		String(const char *str)        : std::string{str} {}
		String(const std::string &str) : std::string{str} {}

		auto split(const String &delims = " ") const -> std::vector<String>
		{
			std::vector<String> output{};

			std::string::size_type first = 0;
			while (first < this->size()) {
				const auto second = this->find_first_of(delims, first);
				if (first != second)
					output.push_back(this->substr(first, second - first));
				if (second == std::string::npos) break;
				first = second + 1;
			}
			return output;
		}

		auto replace(const String &from, const String &to) const -> String
		{
			dz::String str = *this;
			std::string::size_type n = 0;
			while ((n = str.find(from, n)) != std::string::npos) {
					str.replace(n, from.size(), to);
					n += to.size();
			}
			return str;
		}

		auto rtrim(const char* t = ws) const -> String
		{
			auto s = *this;
			s.erase(s.find_last_not_of(t) + 1);
			return s;
		}

		auto ltrim(const char* t = ws) const -> String
		{
			auto s = *this;
			s.erase(0, s.find_first_not_of(t));
			return s;
		}

		auto trim(const char* t = ws) const -> String
		{
			auto s = *this;
			return s.ltrim(t).rtrim(t);
		}

	};

	inline auto join (const String &delim, const std::vector<String> &v) -> String
	{
		if (!v.size()) return "";
		String s;
		for (unsigned i = 0; i < v.size() - 1; i++)
			s+= v[i] + delim;
		s += v[v.size() - 1];
		return s;
	}

} // namespace dz

#endif // DZ_STRING_HPP_
