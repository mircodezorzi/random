#ifndef DZ_MACROS_HPP_
#define DZ_MACROS_HPP_

#if __has_cpp_attribute(nodiscard)
	#define DZ_NODISCARD [[nodiscard]]
#else
	#define DZ_NODISCARD
#endif

#endif // DZ_MACROS_HPP_
