#ifndef RANGE_HPP_
#define RANGE_HPP_

#include <cstring>
#include <functional>

#include "vector.hpp"
#include "zip.hpp"

namespace dz
{

	template <typename T>
	class Range
	{
	private:

		Vector<T> data;

		int to_take;
		int to_drop;

	public:
		Range(const Vector<T> &v)
			: data{v}
			, to_drop{0}
			, to_take{0} {}

		Vector<T> to_vector()
		{
			return data;
		}

		Range<T> &transform(const std::function<void(T&)> &callback)
		{
			for (int i = to_drop; i < data.size() - to_take; i++)
				callback(data[i]);
			return *this;
		}

		Range<T> &filter(const std::function<bool(const T&)> &callback)
		{
			for (int i = to_drop; i < data.size() - to_take; i++)
				if (!callback(data[i]))
					data.remove(i);
			return *this;
		}

		Range<T> &drop(int i)
		{
			to_drop += i;
			return *this;
		}

		Range<T> &take(int i)
		{
			to_take = i - to_drop;
			return *this;
		}

	};

		// vector.iter() -> returns range object
		// range object has a copy of the vector

		// drop(n) -> skip n elements
		// take(n) -> take first n elements
		// filter(f) -> forward only if f return true
		// transform(f) -> apply function
		// iota(m, M) -> creates range (m, M)
		// reverse -> reverse
		// to_vector -> return vector
		// sort
		// unique
		// slice
		// join
		// split


} // namespace dz

#endif /* RANGE_HPP_ */
