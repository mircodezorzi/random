#ifndef DZ_CONTAINERS_HPP_
#define DZ_CONTAINERS_HPP_

#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>

#include "exceptions.hpp"
#include "macros.hpp"
#include "sfinae.hpp"

namespace dz
{

	namespace detail
	{

		template <typename T>
		struct Node
		{
			std::shared_ptr<Node<T>> next;
			T value;

			explicit Node(T value_, std::shared_ptr<Node<T>> next_)
				: next{next_}
				, value{value_} {}

			explicit Node(std::shared_ptr<Node<T>> other)
				: next{other->next}
				, value{other->value} {}
		};

		/// \brief Base Container Class
		template <typename T>
		class Container
		{
		public:
			using node_type = Node<T>;
			using node_wrap = std::shared_ptr<Node<T>>;
			using size_type = unsigned long int;

		protected:
			node_wrap head;
			node_wrap tail;
			size_type count;

			_Pragma("GCC diagnostic push")
			_Pragma("GCC diagnostic ignored \"-Wreturn-type\"")
			_Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")

			virtual auto _pop() -> T = 0;
			virtual auto _pop(int pos) -> T{}
			virtual auto _push(const T &val) -> void = 0;
			virtual auto _deepcopy(node_wrap n) -> void = 0;

			_Pragma("GCC diagnostic pop")

			auto deepcopy(node_wrap n) -> void
			{
				_deepcopy(n);
			}

		public:
			class Iterator;

			auto begin() -> Iterator { return Iterator{head};    }
			auto end()   -> Iterator { return Iterator{nullptr}; }

			Container()
				: head{nullptr}
				, tail{nullptr}
				, count{0} {}

			Container(const Container &other) : count{0}
			{
				deepcopy(other.head);
			}

			/// \brief Push an element into the container.
			auto push(const T &val) -> void
			{
				_push(val);
				count++;
			}

			/// \brief Push any amount of elements into the container.
			template <typename ... Args>
			auto push(const T &val, const Args& ... args) -> void
			{
				push(val);
				push(args...);
			}

			/// \brief Remove an element from the container.
			auto pop() -> T
			{
				T tmp{_pop()};
				count--;
				return tmp;
			}

			/// \brief Remove an element at position pos from the container.
			auto pop(size_type pos) -> T
			{
				if (pos >= size()) {
					throw OutOfRange{};
				}
				T tmp{_pop(pos)};
				count--;
				return tmp;
			}

			auto empty() const -> bool
			{
				return count == 0;
			}

			auto size() const -> size_type
			{
				return count;
			}

			/// \brief Load container from file.
			template <typename T_ = T,
				typename std::enable_if<std::is_arithmetic<T_>::value, T_>::type* = nullptr>
			auto load(const std::string &filepath) -> void
			{
				std::string s;
				std::ifstream f;
				f.open(filepath);
				while (std::getline(f, s)) {
					push(std::stoi(s));
				}
			}

			/// \brief Load container from file.
			template <typename T_ = T,
				typename std::enable_if<std::is_same<T_, std::string>::value, T_>::type* = nullptr>
			auto load(const std::string &filepath) -> void
			{
				std::string s;
				std::ifstream f;
				f.open(filepath);
				while (std::getline(f, s)) {
					push(s);
				}
			}

			/// \brief Load container from file.
			auto load(const std::string &filepath, const std::function<T(const std::string&)> &callback) -> void
			{
				std::string s;
				std::ifstream f;
				f.open(filepath);
				while (std::getline(f, s)) {
					push(callback(s));
				}
			}

			/// \brief Save container to file.
			auto save(const std::string &filepath) const
				-> decltype(has_operator_ostream<T>::value, void())
			{
				std::ifstream f;
				f.open(filepath);
				map([&f](const T &i) { f << i << '\n'; });
				f.close();
			}

			/// \brief Save container to file.
			auto save(const std::string &filepath, const std::function<T(const std::string&)> &callback) const -> void
			{
				std::ifstream f;
				f.open(filepath);
				map([&f, &callback](const T &i) { f << callback(i) << '\n'; });
				f.close();
			}

			/// \brief Streams container nodes to ostream.
			auto stream(std::ostream &os, const std::string &delim = " ") const
				-> decltype(has_operator_ostream<T>::value, void())
			{
				auto i = head.get();
				for (; i->next.get(); i = i->next.get()) {
					os << i->value << delim;
				}
				os << i->value;
			}

			/// \brief Streams container nodes to sstream.
			auto stream(std::stringstream &ss, const std::string &delim = " ") const
				-> decltype(has_operator_ostream<T>::value, void())
			{
				auto i = head.get();
				for (; i->next.get(); i = i->next.get()) {
					ss << i->value << delim;
				}
				ss << i->value;
			}

			/// \brief Print container to stdout.
			auto print() const -> void
			{
				std::cout << "[";
				stream(std::cout, ", ");
				std::cout << "]\n";
			}

			/// \brief Map function to container.
			auto map(const std::function<void(T&)> &f) -> void
			{
				for (auto i = head.get(); i; i = i->next.get()) {
					f(i->value);
				}
			}

			/// \brief Map function to container.
			auto map(const std::function<void(const T&)> &f) const -> void
			{
				for (auto i = head.get(); i; i = i->next.get()) {
					f(i->value);
				}
			}

		};

		template <typename T>
		class Container<T>::Iterator
		{
		public:
			node_wrap n;
			explicit Iterator(node_wrap n_) : n{n_} {}
			auto operator++() -> void { n = n->next; }
			auto operator!=(Iterator rhs) const -> bool { return n.get() != rhs.n.get(); }
			auto operator*() -> T& { return n->value; }
		};

	} // namespace detail


} // namespace dz

#endif /* DZ_CONTAINERS_HPP_ */
