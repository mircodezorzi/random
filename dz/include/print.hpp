#include <iostream>
#include <map>
#include <vector>

#include "string.hpp"
#include "vector.hpp"

namespace dz
{

	namespace detail
	{

		template <typename T>
		void raw_print(const T &i)
		{
			std::cout << i;
		}

		template <typename T>
		void print(const T &i)
		{
			std::cout << i;
		}

		void print(const std::string &i)
		{
			std::cout << "\"" << i << "\"";
		}

		void print(const dz::String &i)
		{
			std::cout << "\"" << i << "\"";
		}

		template <typename T, typename ... Args>
		void print(const T &i, const Args& ... args)
		{
			detail::print(i);
			std::cout << " ";
			detail::print(args...);
		}

		template <typename T, typename U>
		void print(const std::map<T, U> &m)
		{
			auto it = m.begin();

			std::cout << "{";
			while (it != m.end()) {
				detail::print(it->first);
				std::cout << ": ";
				detail::print(it->second);
				if (++it != m.end()) std::cout << ", ";
			}
			std::cout << "}";
		}

		template <typename T>
		void print(const std::vector<T> &v)
		{
			if (!v.size()) {
				std::cout << "[]";
				return;
			}
			std::cout << "[";
			for (unsigned i = 0; i < v.size() - 1; i++) {
				detail::print(v[i]);
				std::cout << ", ";
			}
			detail::print(v[v.size() - 1]);
			std::cout << "]";
		}

		template <typename T>
		void print(const dz::Vector<T> &v)
		{
			if (!v.size()) {
				std::cout << "[]";
				return;
			}
			std::cout << "[";
			for (unsigned i = 0; i < v.size() - 1; i++) {
				detail::print(v[i]);
				std::cout << ", ";
			}
			detail::print(v[v.size() - 1]);
			std::cout << "]";
		}

	}

	template <typename ... Args>
	void print(const Args& ... args)
	{
		detail::print(args...);
		std::cout << std::endl;
	}

} // namespace dz
