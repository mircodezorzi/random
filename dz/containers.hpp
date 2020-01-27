#ifndef DZ_CONTAINERS_HPP_
#define DZ_CONTAINERS_HPP_

#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>

#include "sfinae.hpp"

namespace dz
{

	/// \brief Functional containers internally implemented as singly linked lists.
	namespace containers
	{

		/// \brief Thrown when trying to access empty container
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
				Node<T> *next;
				T value;

				Node(T value_, Node<T> *next_)
					: next{next_}
					, value{value_} {}

				Node(Node<T> *n_)
					: next{n_->next}
					, value{n_->value} {}
			};

			/// \brief Base Container Class
			template <typename T>
			class Container
			{
			protected:
				Node<T> *head;
				Node<T> *tail;
				int      count;

				_Pragma("GCC diagnostic push")
				_Pragma("GCC diagnostic ignored \"-Wreturn-type\"")

				virtual void _push(const T &i) = 0;
				virtual T _pop() = 0;
				virtual T _pop(int i) {}

				_Pragma("GCC diagnostic pop")

			public:
				class Iterator;

				Iterator begin() const { return Iterator{head}; }
				Iterator end()   const { return Iterator{tail}; }

				Container()
					: head{nullptr}
					, tail{nullptr}
					, count{0} {}

				virtual ~Container()
				{
					auto tmp = head;
					while (tmp) {
						auto next = tmp->next;
						if (tmp->next) {
							tmp->next = nullptr;
							delete tmp;
						}
						tmp = next;
					}
					head = nullptr;
					tail = nullptr;
					count = 0;
				}

				/// \brief Push an element into the container (policy is based on the
				/// container implementation).
				///
				/// Wrapper function to automatically keep track of the element count
				/// in the container.
				void push(const T &i)
				{
					_push(i);
					count++;
				}

				/// \brief Push any amount of elements into the container (policy is
				/// based on the container implementation).
				///
				/// Wrapper function to automatically keep track of the element count
				/// in the container.
				template <typename ... Args>
				void push(const T &i, const Args& ... args)
				{
					push(i);
					push(args...);
				}

				/// \brief Remove an element from the container (policy is based on the
				/// container implementation).
				///
				/// Wrapper function to automatically keep track of the element count
				/// in the container.
				T pop()
				{
					T tmp{_pop()};
					count--;
					return tmp;
				}

				/// \brief Remove an element at position i from the container (policy
				/// is based on the container implementation).
				///
				/// Wrapper function to automatically keep track of the element count
				/// in the container.
				T pop(int i)
				{
					assert(i <= size());
					T tmp{_pop(i)};
					count--;
					return tmp;
				}

				bool empty() const
				{
					return head == nullptr;
				}

				int size() const
				{
					return count;
				}

				/// \brief Load container from file.
				///
				/// Override for numerical types.
				template <typename T_ = T,
					typename std::enable_if<std::is_arithmetic<T_>::value, T_>::type* = nullptr>
				void load(const std::string &filepath)
				{
					std::string s;
					std::ifstream f;
					f.open(filepath);
					while (std::getline(f, s))
						push(std::stoi(s));
				}

				/// \brief Load container from file.
				///
				/// Override for strings.
				template <typename T_ = T,
					typename std::enable_if<std::is_same<T_, std::string>::value, T_>::type* = nullptr>
				void load(const std::string &filepath)
				{
					std::string s;
					std::ifstream f;
					f.open(filepath);
					while (std::getline(f, s))
						push(s);
				}

				/// \brief Load container from file.
				///
				/// Iterates though every line of the file, pushing the object returned
				/// by callback, called on each linen.
				void load(const std::string &filepath, const std::function<T(const std::string&)> &callback)
				{
					std::string s;
					std::ifstream f;
					f.open(filepath);
					while (std::getline(f, s))
						push(callback(s));
				}

				/// \brief Save container to file.
				///
				/// Type must implement operator<<.
				auto save(const std::string &filepath) const
					-> decltype(has_operator_ostream<T>::value)
				{
					std::ifstream f;
					f.open(filepath);
					map([&f](const T &i) { f << i << '\n'; });
					f.close();
				}

				/// \brief Save container to file.
				///
				/// Iterate though each node of the container, streaming the returned
				/// value from callback to the file.
				void save(const std::string filepath, const std::function<T(const std::string&)> &callback) const
				{
					std::ifstream f;
					f.open(filepath);
					map([&f, &callback](const T &i) { f << callback(i) << '\n'; });
					f.close();
				}

				/// \brief Streams all container nodes to ostream.
				auto stream(std::ostream &os, const std::string &delim = " ") const
					-> decltype(has_operator_ostream<T>::value)
				{
					auto i = head;
					for (; i->next; i = i->next)
						os << i->value << delim;
					os << i->value;
				}

				/// \brief Streams all container nodes to sstream.
				auto stream(std::stringstream &ss, const std::string &delim = " ") const
					-> decltype(has_operator_ostream<T>::value)
				{
					auto i = head;
					for (; i->next; i = i->next)
						ss << i->value << delim;
					ss << i->value;
				}

				void map(const std::function<void(T&)> &f)
				{
					for (auto i = head; i; i = i->next)
						f(i->value);
				}

				void map(const std::function<void(const T&)> &f) const
				{
					for (auto i = head; i; i = i->next)
						f(i->value);
				}

			};

			template <typename T>
			class Container<T>::Iterator
			{
			private:
				Node<T> *n;
			public:
				Iterator(Node<T>* n_) : n{n_} {}
				void operator++() { n = n->next; }
				bool operator!=(Iterator rhs) { return n != rhs.n->next; }
				T &operator*() { return n->value; }
			};

		} // namespace detail

		/// \brief Stack
		template <typename T>
		class Stack final : public detail::Container<T>
		{
		private:
			using detail::Container<T>::head;
			using detail::Container<T>::count;

			void deepcopy(detail::Node<T> *n)
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

			T pop(int i) = delete;

			void _push(const T &i) override
			{
				head = new detail::Node<T>{i, head};
			}

			T top()
			{
				if(this->empty()) throw Empty();
				return head->value;
			}

			T _pop() override
			{
				if(this->empty()) throw Empty();
				auto tmp = head;
				auto v = tmp->value;
				head = head->next;
				delete tmp;
				return v;
			}

		};

		/// \brief Queue
		template <typename T>
		class Queue : public detail::Container<T>
		{
		private:
			using detail::Container<T>::head;
			using detail::Container<T>::tail;
			using detail::Container<T>::count;

			void deepcopy(detail::Node<T> *n)
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
					auto tmp = new detail::Node<T>{i, head};
					head = tail = tmp;
				} else {
					auto tmp = new detail::Node<T>{i, nullptr};
					tail->next = tmp;
					tail = tmp;
				}
			}

			T top()
			{
				if(this->empty()) throw Empty();
				return tail->value;
			}

			T _pop() override
			{
				if(this->empty()) throw Empty();
				auto tmp = head;
				auto v = tmp->value;
				head = head->next;
				if (this->empty())
					tail = nullptr;
				delete tmp;
				return v;
			}

		};

		/// \brief Linked List
		template <typename T>
		class List : public detail::Container<T>
		{
		protected:
			using detail::Container<T>::head;
			using detail::Container<T>::count;

			void deepcopy(detail::Node<T> *n)
			{
				if(n) {
					push(n->value);
					deepcopy(n->next);
				}
			}

		public:
			using detail::Container<T>::push;
			using detail::Container<T>::Container;

			void _push(const T &i) override
			{
				head = new detail::Node<T>{i, head};
			}

			T _pop() override
			{
				return _pop(1);
			}

			T _pop(int pos) override
			{
				if(this->empty()) throw Empty();
				if (pos == 1) {
					auto tmp = head;
					auto v = head->value;
					head = head->next;
					delete tmp;
					return v;
				} else {
					auto cur = head;
					for (int i = 2; i < pos && cur->next; cur = cur->next) i++;
					if(this->empty()) throw Empty();
					auto tmp = cur->next;
					auto v = cur->next->value;
					cur->next = cur->next->next;
					delete tmp;
					return v;
				}
			}

		};

		/// \brief Ordered Linked List
		///
		/// By default uses a compare function to decide the order of the inserted elements.
		template <typename T>
		class OrderedList : public List<T>
		{
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

			template <typename T_ = T,
				typename std::enable_if<has_operator_greater<T>::value>::type* = nullptr>
			OrderedList() : List<T>{} {}

			OrderedList(std::function<bool(const T&, const T&)> compare_) : List<T>{} , compare{compare_} {}

			void _push(const T &i) override
			{
				if (this->empty() || !compare(i, head->value)) {
					head = new detail::Node<T>{i, head};
				} else {
					detail::Node<T> *cur = head;
					while (cur->next && compare(i, cur->next->value)) cur = cur->next;
					cur->next = new detail::Node<T>{i, cur->next};
				}
			}

		};

	} // namespace containers

} // namespace dz

#endif /* DZ_CONTAINERS_HPP_ */
