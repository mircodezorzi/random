#ifndef DZ_ORDERED_LIST_HPP_
#define DZ_ORDERED_LIST_HPP_

#include "list.hpp"

namespace dz
{

	template <typename T>
	class OrderedList : public List<T>
	{
	public:
		using node_type = typename detail::Container<T>::node_type;
		using node_wrap = typename detail::Container<T>::node_wrap;

	private:
		using List<T>::count;
		using List<T>::head;

		static auto build_compare() -> const std::function<bool(const T&, const T&)>
		{
			return [](const T &lhs, const T &rhs) -> bool { return lhs > rhs; };
		}

		static auto create_compare() -> const std::function<bool(const T&, const T&)> &
		{
			static const auto c = build_compare();
			return c;
		}

		const std::function<bool(const T&, const T&)> &compare = create_compare();

	public:
		using List<T>::empty;
		using List<T>::push;

		template <typename _ = decltype(has_operator_greater<T>::value && has_operator_equals<T>::value)>
		OrderedList()
			: List<T>{} {}

		OrderedList(std::function<bool(const T&, const T&)> compare_,
								std::function<bool(const T&, const T&)> equals_)
			: List<T>{equals_} , compare{compare_} {}

		auto _push(const T &val) -> void override
		{
			if (empty() || !compare(val, head->value)) {
				head = std::make_shared<node_type>(val, head);
			} else {
				auto cur = head;
				while (cur->next && compare(val, cur->next->value)) cur = cur->next;
				cur->next = std::make_shared<node_type>(val, cur->next);
			}
		}

	};

} // namespace dz

#endif // DZ_ORDERED_LIST_HPP_
