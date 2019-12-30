#ifndef DZ_CONTAINERS_HPP_
#define DZ_CONTAINERS_HPP_

#include <fstream>
#include <functional>
#include <iostream>
#include <array>

namespace dz
{

	/// \brief Functional containers internally implemented as singly linked lists.
	namespace containers
	{

		/// \brief Throws when trying to access empty container
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
			};

			template <typename T>
			class Container
			{
			protected:
				Node<T> *head;
				Node<T> *tail;
				int count;

			public:
				Container()
					: head{nullptr}
					, tail{nullptr}
					, count{0} {}

				virtual ~Container()
				{
					clear();
				}

				/// \brief Clear container.
				void clear()
				{
					auto tmp = head;
					while (tmp) {
						auto next = tmp->next;
						delete tmp;
						tmp = next;
					}
					head = nullptr;
					tail = nullptr;
					count = 0;
				}

				template <typename ... Args>
				void push(T i, const Args& ... args)
				{
					push(i);
					push(args...);
				}

				void push(const T &i)
				{
					_push(i);
					count++;
				}

				virtual void _push(const T &i) = 0;

				T pop()
				{
					T tmp{_pop()};
					count--;
					return tmp;
				}

				T pop(int i)
				{
					T tmp{_pop(i)};
					count--;
					return tmp;
				}

				virtual T _pop([[maybe_unused]] int i) {}
				virtual T _pop() {}

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
				/// Type must implement operator<<().
				template <typename T_ = T,
					typename std::enable_if<(sizeof(static_cast<std::ostream &(std::ostream::*)(T_)>(&std::ostream::operator<<)))>::type* = nullptr>
				void save(const std::string &filepath) const
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

				/// \brief Streams all container nodes to ostream
				template <typename T_ = T,
					typename std::enable_if<(sizeof(static_cast<std::ostream &(std::ostream::*)(T_)>(&std::ostream::operator<<)))>::type* = nullptr>
				void stream(std::ostream &os, const std::string &delim = " ") const
				{
					map([&os, &delim](const T &i) { os << i << delim; });
				}

				void map(const std::function<void(const T&)> &f) const
				{
					for (auto i = head; i; i = i->next)
						f(i->value);
				}

			};

		} // namespace detail

		template <typename T>
		class Stack final : public detail::Container<T>
		{
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

		template <typename T>
		class Queue : public detail::Container<T>
		{
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
					for (int i = 2; i < pos && cur->next; ++i, cur = cur->next);
					if(this->empty()) throw Empty();
					auto tmp = cur->next;
					auto v = cur->next->value;
					cur->next = cur->next->next;
					delete tmp;
					return v;
				}
			}

		};


		template <typename T>
		class OrderedList : public List<T>
		{
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
			using List<T>::List;

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
