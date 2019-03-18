// Testing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <sstream>
#include <string>

struct full_vector : std::logic_error {
	using std::logic_error::logic_error;
};

template <typename T, std::size_t Capacity>
class static_vector {
private:
	typename std::aligned_storage< sizeof(T), alignof(T)>::type _data[ Capacity ];
	std::size_t _size;
public:

	using value_type	  = T;
	using reference		  = T & ;
	using const_reference = const T &;
	using iterator		  = T * ;
	using const_iterator  = const T *;

	static_vector() = default;
	static_vector( std::size_t count ) {
	
	}
	static_vector(std::initializer_list<T> list)
		: _size(list.size())
	{ 
		_check_size(list.size());
		int temp = 0;
		for (T item : list) {
			*reinterpret_cast< T* >(_data + temp) = item;
			++temp;
		}	
	}
	
	iterator begin() noexcept { return reinterpret_cast< T* >(_data); }
	const_iterator begin() const noexcept { return reinterpret_cast<T*>(_data); }

	iterator end() noexcept { return begin() + _size; }
	const_iterator end() const noexcept { return begin() + _size; }

	friend std::ostream& operator<<(std::ostream& os, const static_vector& vec) {
		for (int i = 0; i < vec._size; ++i) {
			os << *reinterpret_cast<const T*>(vec._data + i);
		}
		return os;
	}

private:
	void _check_size( std::size_t count ) {
		if (Capacity < count) {
			std::stringstream ss;
			ss << "Could not allocate " << count << " items in static_vector of size: " <<  Capacity << "\n";
			throw full_vector(ss.str().c_str());
		}
	}
};

int main()
{
	try {
		static_vector <int, 4> my_vec{ 1, 2, 3, 4 };
	}
	catch (const full_vector& full) {
		std::cout << full.what() << std::endl;
	}


	return 0;
}
