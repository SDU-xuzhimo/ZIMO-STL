#ifndef _VECTOR_BODY_
#define _VECTOR_BODY_

#include "iterator.h"
//#include <iterator>
#include <memory>
#include <utility>
#include <initializer_list>
#include <algorithm>

template<typename _Ty,typename _pointer,typename _reference>
class _vector_iterator
{
public:
	typedef random_access_iterator_tag iterator_category;
	typedef _Ty value_type;
	typedef ptrdiff_t difference_type;
	typedef _pointer pointer;
	typedef _reference reference;
	typedef _vector_iterator self;


	_vector_iterator(pointer _ptr)
		:ptr(_ptr) {}

	reference operator*()const { return *ptr; }
	pointer operator->()const { return ptr; }

	self operator+(difference_type off)const { return self(ptr + off); }
	self& operator++() { ++ptr; return *this; }
	self operator++(int) { self temp = *this; ++(*this); return temp; }
	self& operator--() { --ptr; return *this; }
	self operator--(int) { self tem = *this; --(*this); return tem; }
	self operator-(difference_type off) { return self(ptr - off); }
	difference_type operator-(const self& rhs)const { return ptr - rhs.ptr; }

	bool operator<(const self& rhs)const { return ptr < rhs.ptr; }
	bool operator>(const self& rhs)const { return ptr > rhs.ptr; }
	bool operator==(const self& rhs)const { return ptr == rhs.ptr; }
	bool operator!=(const self& rhs)const { return ptr != rhs.ptr; }
protected:
	pointer ptr;
};
template<typename T>
class Vector
{
public:
	using value_type = T;
	using reference = T&;
	using pointer = T*;
	using difference_type = ptrdiff_t;
	using size_type = size_t;
	using const_reference = const T&;
	using iterator = _vector_iterator<T,T*,T&>;
	using const_iterator = _vector_iterator<T, const T*, const T&>;

	Vector<T>()
		:begPtr(nullptr), first_free(nullptr),endPtr(nullptr){}

	Vector<T>(const std::initializer_list<T>& _InitList)
	{
		first_free = begPtr = alloc.allocate(_InitList.size()+1);
		for (auto it = _InitList.begin(); it != _InitList.end(); it++)
			alloc.construct(first_free++, *it);
		endPtr = first_free;
	}

	Vector<T>(const Vector& Vec)
	{
		std::pair< T*,T*> _Mypair = allocate_n_copy(Vec.begin(), Vec.end());
		begPtr = _Mypair.first;
		endPtr = first_free = _Mypair.second;
	}

	explicit Vector<T>(Vector&& vec)noexcept
		:begPtr(vec.begPtr),first_free(vec.first_free),endPtr(vec.endPtr)
	{
		vec.begPtr = vec.first_free = vec.endPtr = nullptr;
	}

	~Vector()
	{
		free();
	}

	bool empty()const
	{
		return begPtr == first_free;
	}
	void push_back(const T& val)
	{
		check();
		alloc.construct(first_free++, val);
	}
	void pop_back()
	{
		if (empty())
			throw("vector is empty!");
		alloc.destroy(--first_free);
	}
	int size()const { return first_free - begPtr; }
	int capacity()const { return endPtr - begPtr; }

	iterator begin() { return iterator(begPtr); }
	iterator end() { return iterator(first_free); }
	const_iterator begin()const { return const_iterator(begPtr); }
	const_iterator end()const { return const_iterator(first_free); }

	reference operator[](int pos) { return *(begPtr + (difference_type)pos); }
	const_reference operator[](int pos)const { return *(begPtr + (difference_type)pos); }
	reference front()
	{
		return *begPtr;
	}
	const_reference front()const
	{
		return *begPtr;
	}
	reference back()
	{
		return *(first_free - 1);
	}
	const_reference back()const
	{
		return *(first_free - 1);
	}
private:
	std::allocator<T> alloc;
	T* begPtr;
	T* first_free;
	T* endPtr;

	void check() { if (capacity()==size()) reallocate(); }

	std::pair<T*, T*>
		allocate_n_copy(const_iterator _First, const_iterator _Last)
	{
		auto data = alloc.allocate(_Last - _First);
		return { data, std::uninitialized_copy(_First , _Last, data) };
	}

	void reallocate()
	{
		auto newcapacity = size()==0 ? 1 : size() * 2;
		auto temp = alloc.allocate(newcapacity);
		auto temp_new = temp;
		auto temp_old = begPtr;
		for (auto i = 0; i != size(); i++)
			alloc.construct(temp_new++, std::move(*temp_old++));

		begPtr = temp;
		first_free = temp_new;
		endPtr = begPtr + newcapacity;
	}
	void free()
	{
		if (begPtr)
		{
			for (int i = 0; i < size(); i++)
			{
				alloc.destroy(--first_free);
			}
			alloc.deallocate(begPtr,capacity());
		}
	}
};

#endif