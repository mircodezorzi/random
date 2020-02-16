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

			Node(T value_, std::shared_ptr<Node<T>> next_)
				: next{next_}
				, value{value_} {}

			Node(std::shared_ptr<Node<T>> other)
				: next{other->next}
				, value{other->value} {}
		};

		/// \brief Base Container Class
		template <typename T>
		class Container
		{
		protected:
			std::shared_ptr<detail::Node<T>> head;
			std::shared_ptr<detail::Node<T>> tail;
			int count;

			_Pragma("GCC diagnostic push")
			_Pragma("GCC diagnostic ignored \"-Wreturn-type\"")
			_Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")

			virtual T _pop() = 0;
			virtual T _pop(int pos) {}

			virtual void _push(const T &val) = 0;

			virtual void deepcopy(std::shared_ptr<detail::Node<T>> n) = 0;

			_Pragma("GCC diagnostic pop")

		public:
			class Iterator;

			DZ_NODISCARD Iterator begin() const
			{
				return Iterator{head};
			}

			DZ_NODISCARD Iterator end() const
			{
				return Iterator{nullptr};
			}

			Container()
				: head{nullptr}
				, tail{nullptr}
				, count{0} {}

			Container(const Container &other)
			{
				deepcopy(other.head);
			}

			/// \brief Push an element into the container.
			void push(const T &val)
			{
				_push(val);
				count++;
			}

			/// \brief Push any amount of elements into the container.
			template <typename ... Args>
			void push(const T &val, const Args& ... args)
			{
				push(val);
				push(args...);
			}

			/// \brief Remove an element from the container.
			T pop()
			{
				T tmp{_pop()};
				count--;
				return tmp;
			}

			/// \brief Remove an element at position pos from the container.
			T pop(int pos)
			{
				if (pos >= size())
					throw OutOfRange{};
				T tmp{_pop(pos)};
				count--;
				return tmp;
			}

			bool empty() const
			{
				return count == 0;
			}

			int size() const
			{
				return count;
			}

			/// \brief Load container from file.
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
			void load(const std::string &filepath, const std::function<T(const std::string&)> &callback)
			{
				std::string s;
				std::ifstream f;
				f.open(filepath);
				while (std::getline(f, s))
					push(callback(s));
			}

			/// \brief Save container to file.
			auto save(const std::string &filepath) const
				-> decltype(has_operator_ostream<T>::value)
			{
				std::ifstream f;
				f.open(filepath);
				map([&f](const T &i) { f << i << '\n'; });
				f.close();
			}

			/// \brief Save container to file.
			void save(const std::string filepath, const std::function<T(const std::string&)> &callback) const
			{
				std::ifstream f;
				f.open(filepath);
				map([&f, &callback](const T &i) { f << callback(i) << '\n'; });
				f.close();
			}

			/// \brief Streams all container nodes to ostream.
			auto stream(std::ostream &os, const std::string &delim = " ") const
				-> decltype(has_operator_ostream<T>::value, void())
			{
				auto i = head.get();
				for (; i->next.get(); i = i->next.get())
					os << i->value << delim;
				os << i->value;
			}

			/// \brief Streams all container nodes to sstream.
			auto stream(std::stringstream &ss, const std::string &delim = " ") const
				-> decltype(has_operator_ostream<T>::value, void())
			{
				auto i = head.get();
				for (; i->next.get(); i = i->next.get())
					ss << i->value << delim;
				ss << i->value;
			}

			/// \brief Print container to stdout.
			void print() const
			{
				std::cout << "[";
				stream(std::cout, ", ");
				std::cout << "]\n";
			}

			/// \brief Map function to container.
			void map(const std::function<void(T&)> &f)
			{
				for (auto i = head.get(); i; i = i->next.get())
					f(i->value);
			}

			/// \brief Map function to container.
			void map(const std::function<void(const T&)> &f) const
			{
				for (auto i = head.get(); i; i = i->next.get())
					f(i->value);
			}

		};

		template <typename T>
		class Container<T>::Iterator
		{
		public:
			std::shared_ptr<Node<T>> n;
			Iterator(std::shared_ptr<Node<T>> n_) : n{n_} {}
			void operator++() { n = n->next; }
			bool operator!=(Iterator rhs) { return n.get() != rhs.n.get(); }
			T &operator*() { return n->value; }
		};

	} // namespace detail


} // namespace dz

#endif /* DZ_CONTAINERS_HPP_ */
