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

			Node(T value_, Node<T> *sx_, Node<T> *dx_)
				: sx{sx_}
				, dx{dx_}
				, value{value_} {}
		};

	} // namespace detail

	enum class DisplayMethod { PreOrder, PostOrder, InOrder };

	template <typename T>
	class Tree
	{
	private:
		detail::Node<T> *head;
		int              count;

		void stream(std::ostream &os, detail::Node<T> *n, const std::string &delim, DisplayMethod method) const
		{
			switch (method) {
			case DisplayMethod::PreOrder:
				os << n->value << delim;
				if (n->sx) stream(os, n->sx, delim, method);
				if (n->dx) stream(os, n->dx, delim, method);
				break;
			case DisplayMethod::PostOrder:
				if (n->sx) stream(os, n->sx, delim, method);
				if (n->dx) stream(os, n->dx, delim, method);
				os << n->value << delim;
				break;
			case DisplayMethod::InOrder:
				if (n->sx) stream(os, n->sx, delim, method);
				os << n->value << delim;
				if (n->dx) stream(os, n->dx, delim, method);
				break;
			}
		}

		/// \brief Remove node with element i in tree.
		detail::Node<T> *erase(detail::Node<T> *n, T i) {
			if (n == nullptr) {
				return n;
			} else if (i < n->value) {
				n->sx = erase(n->sx, i);
			} else if (i > n->value) {
				n->dx = erase(n->dx, i);
			} else {
				if (n->sx == nullptr) {
					return n->dx;
				} else if (n->dx == nullptr) {
					return n->sx;
				} else {
					n->value = n->sx->value;
					n->sx = erase(n->sx, n->value);
				}
			}
			return n;
		}

	public:
		class Iterator;

		Iterator begin() const { return Iterator{head}; }
		Iterator end()  const { return Iterator{head}; }

		Tree()
			: head{nullptr}
			, count{0} {}

		/// \brief Push an element into the tree.
		void push(const T &i)
		{
			if (head == nullptr) {
				head = new detail::Node<T>{i, nullptr, nullptr};
				count++;
				return;
			}

			auto n = &head;
			while (*n != nullptr) {
				if (i < (*n)->value)
					n = &(*n)->sx;
				else if (i > (*n)->value)
					n = &(*n)->dx;
				else break;
			}
			*n = new detail::Node<T>{i, nullptr, nullptr};
			count++;
		}

		/// \brief Push any amount of elements into the tree.
		template <typename ... Args>
		void push(T i, const Args& ... args)
		{
			push(i);
			push(args...);
		}

		/// \brief Returns the root of the tree.
		T top() const
		{
			return head->value;
		}

		/// \brief Removes and returns the root of the tree.
		T pop()
		{
			return erase(head, top())->value;
		}

		/// \brief Erases the element in the tree with value i.
		void erase(T i)
		{
			erase(head, i);
		}

		/// \brief Erases as many element in the tree with values...
		template <typename ... Args>
		void erase(T i, const Args& ... args)
		{
			erase(i);
			erase(args...);
		}

		bool empty() const
		{
			return !(head->sx && head->dx);
		}

		/// \brief Save container to file.
		///
		/// Type must implement operator<<.
		template <typename T_ = T,
			typename std::enable_if<has_operator_ostream<T>::value>::type* = nullptr>
		void stream(std::ostream &os, const std::string &delim = " ", DisplayMethod method = DisplayMethod::PreOrder) const
		{
			for (auto &i : *this)
				os << i << delim;
			//if (head)
				//stream(os, head, delim, method);
		}

		void map(const std::function<void(T&)> &f)
		{
			for (auto &i : *this)
				f(i);
		}

		void map(const std::function<void(const T&)> &f) const
		{
			for (const auto &i : *this)
				f(i);
		}

		/// \brief If a tree gets mutated by a function applied to
		/// it, this function will sort it.
		void sort()
		{
			Tree<T> t{};
			for (const auto &i : *this)
				t.push(i);
			*this = t;
		}

	};

	template <typename T>
	class Tree<T>::Iterator
	{
	private:
		detail::Node<T> *n;
		T               *value;

		containers::Stack<detail::Node<T>*> backtrack;

		/// used to botch operator!=
		bool next;
		bool prev;

	public:
		Iterator(detail::Node<T> *n_)
			: n{n_}
			, value{&n_->value}
			, backtrack{}
			, next{true}
			, prev{true}
		{
			backtrack.push(n);
		}

		void operator++()
		{
			backtrack.push(n);
			if (n->sx) {
				n = n->sx;
				value = &n->value;
				return;
			}
			while (n) {
				if (n->dx) {
					n = n->dx;
					value = &n->value;
					return;
				}
				n = backtrack.pop();
			}
		}


		bool operator!=(Iterator _)
		{
			prev = next;
			next = n->sx || n->dx || backtrack.top()->sx;
			return prev;
		}

		T &operator*() { return *value; }

	};

} // namespace dz

#endif /* TREE_HPP_ */
