#ifndef SFINAE_HPP_
#define SFINAE_HPP_

#include <type_traits>
#include <iostream>

namespace detail
{

	template <typename T, typename U>
	struct has_operator_greater_impl
	{
		template <typename T_ = T, typename U_ = U> // template parameters here to enable SFINAE
		static auto test(T_ &&t, U_ &&u) -> decltype(t > u, void(), std::true_type{});
		static auto test(...) -> std::false_type;
		using type = decltype(test(std::declval<T>(), std::declval<U>()));
	};

	template <typename T>
	struct has_operator_ostream_impl
	{
		template <typename T_ = T> // template parameters here to enable SFINAE
		static auto test(std::ostream &&os, T_ &&t) -> decltype(os << t, void(), std::true_type{});
		static auto test(...) -> std::false_type;
		using type = decltype(test(std::declval<std::ostream>(), std::declval<T>()));
	};

} // namespace detail

template <typename T, typename U = T>
struct has_operator_greater : detail::has_operator_greater_impl<T, U>::type {};

template <typename T>
struct has_operator_ostream : detail::has_operator_ostream_impl<T>::type {};

#endif /* SFINAE_HPP_ */
