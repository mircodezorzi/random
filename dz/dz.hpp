#ifndef DZ_HPP_
#define DZ_HPP_

// containers
#include "include/stack.hpp"
#include "include/queue.hpp"
#include "include/list.hpp"
#include "include/ordered_list.hpp"

// other containers
#include "include/tree.hpp"
#include "include/vector.hpp"

// misc
#include "include/string.hpp"
#include "include/print.hpp"

namespace dz
{

	inline auto input(const String &prompt) -> String
	{
		dz::detail::raw_print(prompt);
		String str;
		getline(std::cin, str);
		return str;
	}

} // namespace dz

#endif /* DZ_HPP_ */
