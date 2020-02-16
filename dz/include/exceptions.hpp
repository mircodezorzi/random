#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

#include <exception>

/// \brief Thrown when trying to access empty container.
struct EmptyContainer : public std::exception
{
	const char *what () const throw()
	{
		return "container is empty";
	}
};

/// \brief Throws when trying to access an element that doesn't exist.
struct OutOfRange : public std::exception
{
	const char *what () const throw()
	{
		return "index out of range";
	}
};

#endif // EXCEPTIONS_HPP_
