#pragma once
#include "signal.h"
#include <initializer_list>
#include <math.h>
#define MAXQSIZE 100

namespace ztl
{
	template<typename T>
	class Queue
	{
	public:
		Queue()
			:frontp(0), backp(0) {}
		Queue(std::initializer_list<T> li)
		{
			if (li.size() >= MAXQSIZE)
				throw("数据过大");
			int i = 0;
			for (auto it = li.begin(); it != li.end(); it++, i++)
				Qarr[i] = *it;
			frontp = 0;
			backp = li.size();
		}
		Queue(const Queue& other)
		{
			backp = frontp = other.frontp;
			while (backp != other.backp)
			{
				Qarr[backp] = other.Qarr[backp];
				backp = (backp + 1) % MAXQSIZE;
			}
		}
		~Queue()
		{
			frontp = backp = 0;
		}

		bool empty()const
		{
			return frontp % MAXQSIZE == backp % MAXQSIZE;
		}
		int size()const
		{
			return abs(backp - frontp + MAXQSIZE) % MAXQSIZE;
		}
		int capacity()const
		{
			return MAXQSIZE - 1 - size();
		}
		status push(T elem)
		{
			if (!capacity())
				return OVERFLOW;
			Qarr[backp] = elem;
			backp = (backp + 1) % MAXQSIZE;
			return OK;
		}
		void pop()
		{
			if (empty())return;
			frontp = (frontp + 1) % MAXQSIZE;
		}
		const T& back()const
		{
			return Qarr[backp - 1];
		}
		const T& front()const
		{
			return Qarr[frontp];
		}
	protected:
		T Qarr[MAXQSIZE];
		int frontp = 0;
		int backp = 0;
	};
}
