#ifndef DZ_QUEUE_HPP_
#define DZ_QUEUE_HPP_

#include "container.hpp"

namespace dz
{

	template <typename T>
	class Queue : public detail::Container<T>
	{
		using Node = detail::Node<T>;

	protected:
		using detail::Container<T>::head;
		using detail::Container<T>::tail;
		using detail::Container<T>::count;

		void deepcopy(std::shared_ptr<Node> n) override
		{
			if(n) {
				push(n->value);
				deepcopy(n->next);
			}
		}

	public:
		using detail::Container<T>::pop;
		using detail::Container<T>::push;
		using detail::Container<T>::Container;

		T pop(int i) = delete;

		void _push(const T &i) override
		{
			if (this->empty()) {
				auto tmp = std::make_shared<Node>(i, head);
				head = tmp;
				tail = tmp;
			} else {
				auto tmp = std::make_shared<Node>(i, nullptr);
				tail->next = tmp;
				tail       = tmp;
			}
		}

		T top()
		{
			if (this->empty()) throw EmptyContainer{};
			return tail->value;
		}

		T _pop() override
		{
			if (this->empty()) throw EmptyContainer{};
			auto tmp = head;
			auto v   = tmp->value;
			head = head->next;
			if (this->empty()) tail = nullptr;
			return v;
		}

	};

} // namespace dz

#endif // DZ_QUEUE_HPP_
