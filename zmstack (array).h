#pragma once

#include "signal.h"
#include <initializer_list>

namespace ztl
{
#define MAXSSIZE 100

	template<typename T>
	class Stack
	{
	public:
		Stack()
			:size(0) {}
		Stack(std::initializer_list<T> li)
			:size(li.size())
		{
			int i = 0;
			for (auto it = li.begin(); it != li.end(); it++, i++)
				sarr[i] = *it;
		}
		Stack(const Stack& other)
			:size(other.size)
		{
			for (int i = 0; i < size; i++)
				sarr[i] = other.sarr[i];
		}
		~Stack()
		{
			size = 0;
		}

		bool empty()const
		{
			return !size;
		}
		int capacity()const
		{
			return MAXSSIZE - size;
		}

		status push(T elem)
		{
			if (!capacity())
				return OVERFLOW;
			sarr[size] = elem;
			size++;
			return OK;
		}
		const T& top()const
		{
			return sarr[size - 1];
		}
		void pop()
		{
			if (empty())
				return;
			size--;
		}
	protected:
		T sarr[MAXSSIZE];
		int size;
	};
}