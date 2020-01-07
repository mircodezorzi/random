#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cstring>

namespace dz
{

	template <typename T>
	class Range;

	template <typename T>
	class Vector
	{
	private:
		static constexpr double growth_factor = 1.3;
		static constexpr int    initial_size  = 4;

		T  *data;
		int count;
		int capacity;

	public:
		T *begin() { return &data[0];      }
		T *end()   { return &data[size()]; }

		Vector()
			: count{0}
			, capacity{initial_size}
		{
			data = (T*)malloc(sizeof(T) * capacity);
		}

		Vector(T def, int size)
			: count{size}
			, capacity{size}
		{
			data = (T*)malloc(sizeof(T) * capacity);
			memset(data, def, sizeof(T) * size);
		}

		Vector(const Vector<T> &other)
			: count{other.count}
			, capacity{other.capacity}
		{
			data = (T*)malloc(sizeof(T) * count);
			memcpy(data, other.data, sizeof (T) * count);
		}

		~Vector()
		{
			if (!data) {
				free(data);
				data = nullptr;
			}
		}

		T &operator[](int i)
		{
			return data[i];
		}

		void push_back(const T &i)
		{
			if (size() + 1 >= capacity) {
				capacity *= growth_factor;
				auto ptr = (T*)realloc(data, sizeof(T) * capacity);
				data = ptr;
			}
			data[count] = i;
			count++;
		}

		void remove(int i)
		{
			count--;
			for(int k = i; k < size(); k++)
				data[k] = data[k + 1];
		}

		int size() const
		{
			return count;
		}

		Range<T> iter() const
		{
			return Range<T>{*this};
		}

	};

} // namespace dz

#endif /* VECTOR_HPP_ */
