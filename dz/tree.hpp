#ifndef TREE_HPP_
#define TREE_HPP_

#include <functional>
#include <fstream>
#include <iostream>

#include "containers.hpp"

namespace dz
{

	/// \brief Throws when trying to access empty tree
	struct Empty : public std::exception
	{
		const char *what () const throw()
		{
			return "container is empty";
		}
	};

	namespace detail
	{

		template <typename T>
		struct Node
		{
			Node<T> *sx;
			Node<T> *dx;
			T value;

			Node(T value_, Node<T> *sx_, Node<T> *dx_) : sx{sx_}, dx{dx_} , value{value_} {}
		};

	} // namespace detail

	enum class DisplayMethod { PreOrder, PostOrder, InOrder };

	template <typename T>
	class Tree
	{
	public:

	private:
		detail::Node<T> *head;
		int count;

		void stream(std::ostream &os, detail::Node<T> *h, DisplayMethod method) const
		{
			switch (method) {
			case DisplayMethod::PreOrder:
				os << h->value << " ";
				if (h->sx) stream(os, h->sx, method);
				if (h->dx) stream(os, h->dx, method);
				break;
			case DisplayMethod::PostOrder:
				if (h->sx) stream(os, h->sx, method);
				if (h->dx) stream(os, h->dx, method);
				os << h->value << " ";
				break;
			case DisplayMethod::InOrder:
				if (h->sx) stream(os, h->sx, method);
				os << h->value << " ";
				if (h->dx) stream(os, h->dx, method);
				break;
			}
		}

		/// \brief Return node with biggest value in subtree
		detail::Node<T> *max(detail::Node<T> *h)
		{
			auto cur = h;
			while (cur->dx != nullptr)
				cur = cur->dx;
			return cur;
		}

	public:
		class Iterator;

		Tree() : head{nullptr}, count{0} {}

		~Tree()
		{
			while (!empty())
				pop();
		}

		void erase(T i)
		{
			erase(head, i);
		}

		T pop()
		{
			return erase(head, top())->value;
		}

		T top() const
		{
			return head->value;
		}

		detail::Node<T> *erase(detail::Node<T> *h, T i)
		{
			if (h == nullptr) return h;

			if (i < h->value) {
				h->sx = erase(h->sx, i);
			} else if (i > h->value) {
				h->dx = erase(h->dx, i);
			} else {
				if(h->dx == nullptr && h->sx == nullptr) {
					delete h;
					return h = nullptr;
				} else if (h->sx == nullptr) {
					auto tmp = h->dx;
					delete h;
					return tmp;
				} else if (h->dx == nullptr) {
					auto tmp = h->sx;
					delete h;
					return tmp;
				} else {
					auto tmp = max(h->sx);
					h->value = tmp->value;
					h->sx = erase(h->sx, tmp->value);
				}
			}
			return h;
		}

		bool empty() const
		{
			return !(head->sx && head->dx);
		}

		template <typename ... Args>
		void push(T i, const Args& ... args)
		{
			push(i);
			push(args...);
		}

		void push(const T &i)
		{
			if (head == nullptr) {
				head = new detail::Node<T>{i, nullptr, nullptr};
				count++;
				return;
			}

			auto h = &head;
			while (*h != nullptr) {
				if (i < (*h)->value)
					h = &(*h)->sx;
				else if (i > (*h)->value)
					h = &(*h)->dx;
				else break;
			}
			*h = new detail::Node<T>{i, nullptr, nullptr};
			count++;
		}

		void stream(std::ostream &os, DisplayMethod method = DisplayMethod::PreOrder) const
		{
			if (head)
				stream(os, head, method);
		}

		Iterator iterator() const
		{
			return Iterator{head};
		}

		void map(const std::function<void(T)> &f) const
		{
			auto it = iterator();
			while (it.has_next()) f(it.next());
		}

		void map(const std::function<void(T&)> &f)
		{
			auto it = iterator();
			while (it.has_next()) f(it.next());
		}

		void map(const std::function<void(const T&)> &f) const
		{
			auto it = iterator();
			while (it.has_next()) f(it.next());
		}

		void sort()
		{
			auto it = iterator();
			Tree<T> t{};
			while (it.has_next()) t.push(it.next());
			*this = t;
		}

	};


	template <typename T>
	class Tree<T>::Iterator
	{
	private:
		detail::Node<T> *h;
		detail::Node<T> *head;
		containers::Stack<detail::Node<T>*> backtrack;

	public:
		Iterator(detail::Node<T> *h_)
			: h{h_}
			, head{h_}
			, backtrack{}
		{
			backtrack.push(h);
		}

		void reset()
		{
			h = head;
			backtrack.clear();
			backtrack.push(h);
		}

		T &next()
		{
			backtrack.push(h);
			if (h->sx) {
				h = h->sx;
				return h->value;
			}
			while (h) {
				if (h->dx) {
					h = h->dx;
					return h->value;
				}
				h = backtrack.pop();
			}
		}

		bool has_next()
		{
			return h->sx || h->dx || backtrack.top()->sx;
		}

	};

} // namespace dz

#endif /* TREE_HPP_ */
