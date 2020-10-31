#ifndef DZ_HPP_
#define DZ_HPP_

// containers
#include "stack.hpp"
#include "queue.hpp"
#include "list.hpp"
#include "ordered_list.hpp"

// other cs
#include "tree.hpp"
#include "vector.hpp"

// misc
#include "string.hpp"
#include "print.hpp"

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
