#ifndef DZ_LIST_HPP_
#define DZ_LIST_HPP_

#include "container.hpp"

namespace dz
{

	template <typename T>
	class List : public detail::Container<T>
	{
	public:
		using node_type = typename detail::Container<T>::node_type;
		using node_wrap = typename detail::Container<T>::node_wrap;
		using size_type = typename detail::Container<T>::size_type;

		static constexpr const size_type npos = -1;

	protected:
		using detail::Container<T>::head;
		using detail::Container<T>::count;

		auto _deepcopy(node_wrap n) -> void override
		{
			if (n) {
				push(n->value);
				_deepcopy(n->next);
			}
		}

		static auto build_equals() -> const std::function<bool(const T&, const T&)>
		{
			return [](const T &lhs, const T &rhs) -> bool { return lhs == rhs; };
		}

		static auto create_equals() -> const std::function<bool(const T&, const T&)>&
		{
			static const auto e = build_equals();
			return e;
		}

		const std::function<bool(const T&, const T&)> &equals = create_equals();

	public:
		using detail::Container<T>::push;
		using detail::Container<T>::pop;
		using detail::Container<T>::size;

		template <typename _ = decltype(has_operator_greater<T>::value)>
		List()
			: detail::Container<T>{} {}

		List(std::function<bool(const T&, const T&)> equals_)
			: detail::Container<T>{}
			, equals{equals_} {}

		auto operator[](size_type index) -> T&
		{
			if (index >= size()) {
				throw OutOfRange{};
			}
			auto i = head.get();
			for (; index; i = i->next.get());
			return i;
		}

		/// \brief Return index of val, npos if not present
		auto index(const T &val) -> size_type
		{
			int index = 0;
			for (const auto &i : *this) {
				if (equals(val, i))
					return
				index++;
			}
			return npos;
		}

		auto _push(const T &val) -> void override
		{
			head = std::make_shared<node_type>(val, head);
		}

		auto _pop() -> T override
		{
			return _pop(1);
		}

		auto _pop(int pos) -> T override
		{
			if (this->empty()) {
				throw EmptyContainer{};
			}
			if (pos == 1) {
				auto tmp = head;
				auto v = head->value;
				head = head->next;
				return v;
			} else {
				auto cur = head;
				for (int i = 2; i < pos && cur->next; cur = cur->next) i++;
				if (this->empty()) {
					throw EmptyContainer{};
				}
				auto tmp = cur->next;
				auto v = cur->next->value;
				cur->next = cur->next->next;
				return v;
			}
		}

	};

} // namespace dz

#endif // DZ_LIST_HPP_
