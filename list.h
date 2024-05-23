#pragma once
#ifndef __LIST__
#define __LIST__
#include <iterator>
template<typename _Ty>
class _list_node
{
public:
	using value_type = _Ty;
	value_type _Mydata;
	class _list_node* _prev;
	class _list_node* _next;
};

template<typename _Ty,typename _reference,typename _pointer>
class _list_iterator
{
public:
	using iterator_category = bidirectional_iterator_tag;
	using value_type = _Ty;
	using reference = _reference;
	using pointer = _pointer;
	using const_reference = const _Ty&;
	using const_pointer = const _Ty*;
	using self = _list_iterator;

	reference operator*()const { return node->_Mydata; }
	
	self& operator++()
	{
		return node = node->_next;
	}
	self operator++(int)
	{
		_list_iterator temp = *this;
		++(*this);
		return temp;
	}
	self& operator--()
	{
		return node = node->_prev;
	}
	self operator--(int)
	{
		_list_iterator temp = *this;
		--(*this);
		return temp;
	}

	bool operator==(const _list_iterator& rhs)const
	{
		return node == rhs.node;
	}
	bool operator!=(const _list_iterator& rhs)const
	{
		return node != rhs.node;
	}
protected:
	_list_node<_Ty>* node;
};

template<typename _Ty>
class list
{
public:
	using value_type = _Ty;
	using reference = _Ty&;
	using const_reference = const _Ty&;
	using pointer = _Ty*;
	using const_pointer = const _Ty*;
	using iterator = _list_iterator<_Ty, reference, pointer>;
	using const_iterator = _list_iterator<_Ty, const _Ty&, const _Ty*>;

protected:
	_list_node* node;
};
#endif
