#ifndef DZ_QUEUE_HPP_
#define DZ_QUEUE_HPP_

#include "container.hpp"

namespace dz
{

	template <typename T>
	class Queue final : public detail::Container<T>
	{
	public:
		using node_type = typename detail::Container<T>::node_type;
		using node_wrap = typename detail::Container<T>::node_wrap;

	private:
		using detail::Container<T>::count;
		using detail::Container<T>::head;
		using detail::Container<T>::tail;

		auto _deepcopy(node_wrap n) -> void override
		{
			if(n) {
				push(n->value);
				_deepcopy(n->next);
			}
		}

		auto _push(const T &val) -> void override
		{
			if (empty()) {
				auto tmp = std::make_shared<node_type>(val, head);
				head = tmp;
				tail = tmp;
			} else {
				auto tmp = std::make_shared<node_type>(val, nullptr);
				tail->next = tmp;
				tail       = tmp;
			}
		}

		auto _pop() -> T override
		{
			if (empty()) throw EmptyContainer{};
			auto tmp = head;
			auto v   = tmp->value;
			head = head->next;
			if (empty()) tail = nullptr;
			return v;
		}

	public:
		using detail::Container<T>::Container;
		using detail::Container<T>::empty;
		using detail::Container<T>::pop;
		using detail::Container<T>::push;

		auto pop(int index) -> T& = delete;

		auto top() -> T
		{
			if (empty()) throw EmptyContainer{};
			return tail->value;
		}

	};

} // namespace dz

#endif // DZ_QUEUE_HPP_
