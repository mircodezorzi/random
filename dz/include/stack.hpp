#ifndef DZ_STACK_HPP_
#define DZ_STACK_HPP_

#include "container.hpp"

namespace dz
{

	template <typename T>
	class Stack : public detail::Container<T>
	{
		using Node = detail::Node<T>;

	protected:
		using detail::Container<T>::head;
		using detail::Container<T>::count;

		void deepcopy(std::shared_ptr<Node> n) override
		{
			if(n) {
				deepcopy(n->next);
				push(n->value);
			}
		}

	public:
		using detail::Container<T>::pop;
		using detail::Container<T>::push;
		using detail::Container<T>::Container;

		T pop(int val) = delete;

		void _push(const T &val) override
		{
			head = std::make_shared<Node>(val, head);
		}

		T top()
		{
			if (this->empty()) throw EmptyContainer{};
			return head->value;
		}

		T _pop() override
		{
			if (this->empty()) throw EmptyContainer{};
			auto tmp = head;
			auto v = tmp->value;
			head = head->next;
			return v;
		}

	};

} // namespace dz

#endif // DZ_STACK_HPP_
