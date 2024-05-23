#pragma once

#ifndef _QUEUE_
#define _QUEUE_
#include "deque"

template<typename _Ty, class _Container = deque<_Ty>>
class queue
{
public:
	using value_type = typename _Container::value_type;
	using reference = typename _Container::reference;
	using const_reference = typename _Container::const_reference;
	using size_type = typename _Container::size_type;
	using container_type = _Container;

	queue() = default;
	explicit queue(const _Container& container)
		:c(container){}
	explicit queue(_Container&& container)
		:c(std::move(container)){}

	bool empty()const
	{
		return c.empty();
	}
	size_type size()const
	{
		return c.size();
	}

	reference front()
	{
		return c.front();
	}
	const_reference front()const
	{
		return c.front();
	}
	reference back()
	{
		return c.back();
	}
	const_reference back()const
	{
		return c.back();
	}
	void pop()
	{
		c.pop_front();
	}
	void push(const value_type& _Val)
	{
		c.push_back(_Val);
	}
	void push(value_type&& _Val)
	{
		c.push_back(std::move(_Val));
	}

	const _Container& _Getcontainer()const
	{
		return c;
	}
protected:
	_Container c{};
};

template<typename _Ty, class _Container = Vector<_Ty>,class _Pre = std::less<_Ty>>
class priority_queue
{
public:
	using value_type = _Ty;
	using reference = _Ty&;
	using const_reference = const _Ty&;
	using size_type = size_t;
	using container_type = _Container;
	using compare = _Pre;

	priority_queue() = default;
	priority_queue(const container_type& cont)
		:c(cont)
	{
		make_heap();
	}
	void push(const value_type& _Val)
	{
		push_heap(_Val);
	}
	void pop()
	{
		pop_heap();
	}
	const_reference top()const
	{
		return c[0];
	}
	size_type size()const
	{
		return c.size();
	}
	bool empty()const
	{
		return !c.size();
	}
	const_reference back()const
	{
		return c[c.size()];
	}

	void test()
	{
		for (auto it = c.begin(); it != c.end(); it++)
			std::cout << *it << " ";
	}
protected:
	_Container c{};
	compare _prefunc;
private:
	void make_heap()
	{
		if (c.size()<=1)
			return;
		
		for (int current = (c.size()-1)/2; current >= 0; current--)
		{
			value_type cur = c[current];
			int child = current * 2;
			while (child < c.size())
			{
				if (child < c.size()-1 && _prefunc(c[child], c[child + 1]))
					child++;
				if (!_prefunc(cur, c[child]))
					break;
				c[child / 2] = c[child];
				child *= 2;
			}
			c[child/2] = cur;
		}
	}
	void push_heap(const value_type& _Val)
	{
		c.push_back(_Val);
		int heapsize = c.size()-1;
		int current = heapsize;
		while (current != 0 && !_prefunc(_Val, c[current / 2]))
		{
			c[current] = c[current / 2];
			current /= 2;
		}
		c[current] = _Val;
	}
	void pop_heap()
	{
		if (c.empty())
			return;
		if (c.size() == 1)
		{
			c.pop_back();
			return;
		}
		value_type last_elem = c[c.size()-1];
		//c[0] = last_elem;
		c.pop_back();
		int current = 0;
		int child = 1;
		while (child < c.size())
		{
			if (child < c.size()-1 && _prefunc(c[child], c[child + 1]))
				child++;
			if (!_prefunc(last_elem, c[child]))
				break;
			c[current] = c[child];
			current = child;
			child *= 2;
		}
		c[current] = last_elem;
	}
};
#endif
