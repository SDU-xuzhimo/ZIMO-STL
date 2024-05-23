#pragma once
#ifndef _STACK_
#define _STACK_
#include "deque"

template<typename _Ty, class _Container = deque<_Ty>>
class stack
{
public:
	using value_type = typename _Container::value_type;
	using reference = typename _Container::reference;
	using const_reference = typename _Container::const_reference;
	using size_type = typename _Container::size_type;
	using container_type = _Container;


	stack() = default;
	explicit stack(const _Container& container)
		:c(container){}
	explicit stack(_Container&& container)noexcept
		:c(std::move(container)){}
	bool empty()const
	{
		return c.empty();
	}
	size_type size()const
	{
		return c.size();
	}

	reference top()
	{
		return c.back();
	}
	const_reference top()const
	{
		return c.back();
	}
	void push(const value_type& _Val)
	{
		c.push_back(_Val);
	}
	void push(value_type&& _Val)noexcept
	{
		c.push_back(std::move(_Val));
	}
	void pop()
	{
		c.pop_back();
	}
	const _Container& _Getcontainer()
	{
		return c;
	}
protected:
	_Container c{};
};
#endif
