#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cstring>
#include <vector>

namespace dz
{

	template <typename T>
	class Vector
	{
	private:

		using size_type = unsigned long;
		static constexpr double growth_factor = 1.3;
		static constexpr int    initial_size  = 4;

		size_type count;
		size_type capacity;

		T *data;

	public:
		T *begin() { return &data[0];      }
		T *end()   { return &data[size()]; }

		Vector() : count{0} , capacity{initial_size}
		{
			data = (T*)malloc(sizeof(T) * capacity);
		}

		Vector(T def, size_type size) : count{size} , capacity{size}
		{
			data = (T*)malloc(sizeof(T) * capacity);
			memset(data, def, sizeof(T) * size);
		}

		Vector(const Vector<T> &other) : count{other.count} , capacity{other.capacity}
		{
			data = (T*)malloc(sizeof(T) * count);
			memcpy(data, other.data, sizeof (T) * count);
		}

		Vector(const std::vector<T> &other) : count{other.size()}, capacity{other.capacity}
		{
			for (const auto &i : other)
				push_back(i);
		}

		~Vector()
		{
			if (!data) {
				free(data);
				data = nullptr;
			}
		}

		T &operator[](int pos) const
		{
			assert(pos < size());
			if (pos < 0) {
				return data[size() + pos - 1];
			} else {
				return data[pos];
			}
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

		void remove(size_type pos)
		{
			assert(pos < size());
			count--;
			for (size_type k = pos ; k < size(); k++)
				data[k] = data[k + 1];
		}

		size_type index(const T &val)
		{
			size_type index = 0;
			for (const auto &i : *this) {
				if (i == val) {
					return index;
				}
				index++;
			}
			return -1;
		}

		size_type size() const
		{
			return count;
		}

	};

} // namespace dz

#endif /* VECTOR_HPP_ */
