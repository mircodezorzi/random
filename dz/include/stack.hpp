#ifndef DZ_STACK_HPP_
#define DZ_STACK_HPP_

#include "container.hpp"

namespace dz
{

	template <typename T>
	class Stack final : public detail::Container<T>
	{
	public:
		using node_type = typename detail::Container<T>::node_type;
		using node_wrap = typename detail::Container<T>::node_wrap;

	protected:
		using detail::Container<T>::count;
		using detail::Container<T>::head;

		auto _deepcopy(node_wrap n) -> void override
		{
			if(n) {
				_deepcopy(n->next);
				push(n->value);
			}
		}

	public:
		using detail::Container<T>::Container;
		using detail::Container<T>::empty;
		using detail::Container<T>::pop;
		using detail::Container<T>::push;

		auto pop(int pos) -> T = delete;

		auto _push(const T &val) -> void override
		{
			head = std::make_shared<node_type>(val, head);
		}

		auto top() -> T
		{
			if (empty()) {
				throw EmptyContainer{};
			}
			return head->value;
		}

		auto _pop() -> T override
		{
			if (empty()) {
				throw EmptyContainer{};
			}
			auto tmp = head;
			auto v = tmp->value;
			head = head->next;
			return v;
		}

	};

} // namespace dz

#endif // DZ_STACK_HPP_
