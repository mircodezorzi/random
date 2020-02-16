#ifndef DZ_ORDERED_LIST_HPP_
#define DZ_ORDERED_LIST_HPP_

#include "list.hpp"

namespace dz
{

	template <typename T>
	class OrderedList : public List<T>
	{
		using Node = detail::Node<T>;

	private:
		using List<T>::head;
		using List<T>::count;

		static const std::function<bool(const T&, const T&)> build_compare()
		{
			return [](const T &lhs, const T &rhs) -> bool { return lhs > rhs; };
		}

		static const std::function<bool(const T&, const T&)> &create_compare()
		{
			static const auto c = build_compare();
			return c;
		}

		const std::function<bool(const T&, const T&)> &compare = create_compare();

	public:
		using List<T>::push;


		template <typename _ = decltype(has_operator_greater<T>::value)>
		OrderedList() : List<T>{} {}

		OrderedList(std::function<bool(const T&, const T&)> compare_) : List<T>{} , compare{compare_} {}

		void _push(const T &i) override
		{
			if (this->empty() || !compare(i, head->value)) {
				head = std::make_shared<Node>(i, head);
			} else {
				auto cur = head;
				while (cur->next && compare(i, cur->next->value)) cur = cur->next;
				cur->next = std::make_shared<Node>(i, cur->next);
			}
		}

	};

} // namespace dz

#endif // DZ_ORDERED_LIST_HPP_
