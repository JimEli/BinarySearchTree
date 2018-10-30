/*************************************************************************
* Title: Vector
* File: vector.h
* Author: James Eli
* Date: 10/26/2018
*
* Vector class with basic functionality (no iterators, removing items, etc.).
*
* Notes:
*  (1) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0 (32 & 64-bit), and with Eclipse
*      Oxygen.3a Release (4.7.3a), using CDT 9.4.3/MinGw32 gcc-g++ (6.3.0-1).
*************************************************************************
* Change Log:
*  10/26/2018: Initial release. JME
*************************************************************************/
#ifndef _MY_VECTOR_H_
#define _MY_VECTOR_H_

#include <memory> // unique pointer

template<typename T>
class Vector
{
	std::size_t count;         // Number of actually stored objects.
	std::size_t capacity;      // Allocated capacity.
	std::unique_ptr<T[]> data; // Data element.

public:
	// Default ctor.
	Vector() : count(0), capacity(0), data(nullptr) { };
	// Copy ctor.
	Vector(Vector const &rhs) : count(rhs.count), capacity(rhs.capacity), data(nullptr)
	{
		std::unique_ptr<T[]> data = std::make_unique<T[]>(capacity);

		for (std::size_t i = 0; i < count; i++)
			data[i] = rhs.data[i];
	};

	// Dtor.
	~Vector() { };
	
	// Clear.
	void clear() { data.release(); };

	// Provides memory management.
	Vector &operator= (Vector const &rhs)
	{
		std::unique_ptr<T[]> temp(static_cast<T*>(data.release()));

		count = rhs.count;
		capacity = rhs.capacity;
		data.reset(new T[capacity]);
		
		for (std::size_t i = 0; i < count; i++)
			data[i] = temp[i];

		return *this;
	};

	// Adds new value, and if needed allocates more space.
	void push_back(T const &d)
	{
		if (capacity == count)
			resize();
		data[count++] = d;
	};

	// Decrements count, doesn't actually remove value from array.
	void pop_back()
	{
		if (count == 0)
			return;
		count--;
	};

	// Size getter.
	size_t size() const { return count; };

	// Bracketed set/get.
	T const &operator[] (size_t i) const { return data[i]; };
	T &operator[] (size_t i) { return data[i]; };

private:
	// Allocates double old size.
	void resize()
	{
		capacity = capacity ? capacity*2 : 1;

		std::unique_ptr<T[]> temp(static_cast<T*>(data.release()));
		data.reset(new T[capacity]);
		// Copy old to new.
		for (std::size_t i = 0; i < count; i++)
			data[i] = temp[i];
	};
};

#endif
