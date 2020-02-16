#ifndef DZ_LIST_HPP_
#define DZ_LIST_HPP_

#include "container.hpp"

namespace dz
{

	template <typename T>
	class List : public detail::Container<T>
	{
		using Node = detail::Node<T>;

	protected:
		using detail::Container<T>::head;
		using detail::Container<T>::count;

		void deepcopy(std::shared_ptr<Node> n) override
		{
			if (n) {
				push(n->value);
				deepcopy(n->next);
			}
		}

	public:
		using detail::Container<T>::size;
		using detail::Container<T>::pop;
		using detail::Container<T>::push;
		using detail::Container<T>::Container;

		T &operator[](int index)
		{
			if (index >= size())
				throw OutOfRange{};
			auto i = head.get();
			for (; index; i = i->next.get());
			return i;
		}

		void _push(const T &i) override
		{
			head = std::make_shared<Node>(i, head);
		}

		T _pop() override
		{
			return _pop(1);
		}

		T _pop(int pos) override
		{
			if (this->empty()) throw EmptyContainer{};
			if (pos == 1) {
				auto tmp = head;
				auto v = head->value;
				head = head->next;
				return v;
			} else {
				auto cur = head;
				for (int i = 2; i < pos && cur->next; cur = cur->next) i++;
				if (this->empty()) throw EmptyContainer{};
				auto tmp = cur->next;
				auto v = cur->next->value;
				cur->next = cur->next->next;
				return v;
			}
		}

	};

} // namespace dz

#endif // DZ_LIST_HPP_
