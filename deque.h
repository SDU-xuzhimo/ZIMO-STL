#pragma once

#ifndef _DEQUE_
#define _DEQUE_
#include <memory>
#include <iterator>
#include <locale>
#include <initializer_list>
#include <algorithm>
//#define __ZTL__
//#define __TEST__

template<typename _Ty, typename _reference, typename _pointer, size_t Buf_Size = 0>
struct _deque_iterator
{
	static size_t buf_size()//give the size of buffer
	{
		return 512;
	}
	using iterator_category = random_access_iterator_tag;
	using value_type = _Ty;
	using reference = _reference;
	using pointer = _pointer;
	using difference_type = ptrdiff_t;
	using map_pointer = pointer*;
	using self = _deque_iterator;
	using iterator = _deque_iterator<_Ty, _Ty&, _Ty*>;
	using const_iterator = _deque_iterator<_Ty, const _Ty&, const _Ty*>;

	value_type* cur;//指向当前元素的指针
	value_type* first;//指向当前缓冲区头
	value_type* last;//指向当前缓冲区的尾后
	map_pointer node;//当前缓冲区

public:
	_deque_iterator(pointer _ptr, map_pointer _node)
		:cur(_ptr), first(*_node), last(first + difference_type(buf_size())), node(_node) {}
	_deque_iterator()
		:cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
	_deque_iterator(const _deque_iterator& rhs)
		:cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {}

	reference operator*()const { return *cur; }
	pointer operator->()const { return cur; }

	difference_type operator-(const _deque_iterator& rhs)const
	{
		return difference_type(buf_size()) * (node - rhs.node - 1) + (cur - first) + (rhs.last - rhs.cur);
	}
	self& operator++()
	{
		++cur;
		if (cur == last)
		{
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}
	self operator++(int)
	{
		self temp = *this;
		++*this;
		return temp;
	}
	self& operator--()
	{
		if (cur == first)
		{
			set_node(node - 1);
			cur = last;
		}
		cur--;
		return *this;
	}
	self operator--(int)
	{
		self temp = *this;
		--* this;
		return temp;
	}
	self& operator+=(difference_type off)
	{
		difference_type off_set = off + (cur - first);
		if (off_set >= 0 && off_set < buf_size())//在同一缓冲区内
			cur += off;
		else
		{
			difference_type node_offset = off_set > 0 ?
				off_set / difference_type(buf_size()) : -(difference_type(-off_set - 1) / difference_type(buf_size()) - 1);
			set_node(node + node_offset);
			cur = first+off_set - (node_offset * difference_type(buf_size()));
		}
		return *this;
	}
	self& operator-=(difference_type off)
	{
		return *this += -off;
	}
	self operator+(difference_type off)const
	{
		self temp = *this;
		return temp += off;
	}
	self operator-(difference_type off)const
	{
		self temp = *this;
		return temp -= off;
	}
	
	bool operator==(const _deque_iterator& rhs)const { return cur == rhs.cur; }
	bool operator!=(const _deque_iterator& rhs)const { return cur != rhs.cur; }
	bool operator<(const _deque_iterator& rhs)const { return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node); }
	void set_node(map_pointer new_node)//实现在不同缓冲区之间跳转
	{
		first = *new_node;
		last = first + difference_type(buf_size());
		node = new_node;
	}
};
template<typename T,size_t Buf_Size = 0>
class deque
{
	static size_t buf_size()
	{
		return 512;
	}
public:
	using value_type = T;
	using pointer = T*;
	using difference_type = ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using size_type = size_t;
	using iterator = _deque_iterator<value_type, reference, pointer, Buf_Size>;
	using const_iterator =  _deque_iterator<value_type, const T&, const T*, Buf_Size>;
	
	deque(size_t _map_size = 8)
	{
		create_map_and_nodes();
	}
	//deque(const std::initializer_list<value_type> init_list);
	bool empty()const
	{
		return start.cur == finish.cur;
	}
	size_t size()const
	{
		return finish - start;
	}
	
	void push_back(const T& _Val)
	{
#ifdef __DEBUG__
		if (empty())
		{

#ifdef __ZTL__
			map_pointer nstart = start.node;
			*nstart = allocate_node();
			start.set_node(nstart);
			start.cur = start.first;
#endif
#ifdef __TEST__
			
			start.node = allocate_node();
			start.cur = start.first = *start.node;
			start.last = *start.node+difference_type(buf_size());
			
#endif
			dataAllocator.construct(start.cur++, _Val);
			finish.set_node(nstart);
			finish.cur = start.cur + 1;
		}
#endif
		if (finish.cur != finish.last - 1)
			dataAllocator.construct(finish.cur++, _Val);
		else
			push_back_aux(_Val);
	}
	void push_front(const T& _Val)
	{
		if (start.cur != start.first)
			dataAllocator.construct(--start.cur, _Val);
		else
			push_front_aux(_Val);
	}
	void pop_back()
	{
		if (empty())
			return;
		
		if (finish.cur != finish.first)
			--finish.cur;
		else
			pop_back_aux();
	}
	void pop_front()
	{
		if (empty())
			return;
		if (start.cur != start.last - 1)
			++start.cur;
		else
			pop_front_aux();
	}
	void clear()
	{
		while (!empty())
			pop_back();
		mapAllocator.deallocate(map,map_size);
		create_map_and_nodes();
	}
	reference operator[](int pos)
	{
		return *(start.cur + difference_type(pos));
	}
	const_reference operator[](int pos)const
	{
		return *(start.cur + difference_type(pos));
	}
	reference front()
	{
		return *start.cur;
	}
	const_reference front()const
	{
		return *start.cur;
	}
	reference back()
	{
		return *(finish.cur - 1);
	}
	const_reference back()const
	{
		return *(finish.cur - 1);
	}

	iterator begin()
	{
		return start;
	}
	const_iterator begin()const
	{
		return start;
	}
	iterator end()
	{
		return finish;
	}
	const_iterator end()const
	{
		return finish;
	}
private:
	void push_back_aux(const T& _Val)
	{
		reserve_map_at_back();
		*(finish.node + 1) = allocate_node();
		dataAllocator.construct(finish.cur, _Val);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}
	void push_front_aux(const T& _Val)
	{
		reserve_map_at_front();
		*(start.node - 1) = allocate_node();
		start.set_node(start.node - 1);
		start.cur = start.last;
		dataAllocator.construct(--start.cur, _Val);
	}
	void pop_back_aux()
	{
		deallocate_node(finish.node);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last-1;
	}
	void pop_front_aux()
	{
		deallocate_node(start.node);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}
protected:
	/*using mapAllocator = std::allocator<pointer>;
	using dataAllocator = std::allocator<value_type>;*/
	std::allocator<pointer> mapAllocator;
	std::allocator<value_type> dataAllocator;
	using map_pointer = pointer*;

private:
	map_pointer map;//中控器
	iterator start;	//store the information of the first cathe
	iterator finish;//store the information of the last cathe  start和finish之间必须是充满元素的
	size_t map_size;

	//tool fuctions

	pointer allocate_node()//在map还有余量时创建新的缓冲区
	{
		return dataAllocator.allocate(difference_type(buf_size()));
	}
	void deallocate_node(map_pointer node)
	{
		dataAllocator.deallocate(*node,buf_size());
	}
	void create_map_and_nodes(size_type num_element=0)//产生并安排好deque的结构
	{
		size_type num_node = num_element / buf_size() + 1;
		map_size = num_node+2>8?num_node:8;//8是map最少管理的节点数
		map = mapAllocator.allocate(map_size);//分配中控器
		
		//优先使用中间部分，方便后续扩充
		map_pointer nstart = map + (map_size - num_node) / 2;
		map_pointer nfinish = nstart + num_node - 1;

		map_pointer cur;
		for (cur = nstart; cur <= nfinish; cur++)
			*cur = allocate_node();
		//维护start和finish
		start.set_node(nstart);
		finish.set_node(nfinish);//正式为start和finish赋值
		start.cur = start.first;
		finish.cur = finish.first + num_element % buf_size();
	}

	//map的某一端到顶了,用来判断在map的前端还是后端扩充
	void reserve_map_at_back(size_type nodes_add = 1)
	{
		if (nodes_add + 1 > map_size - (finish.node - map))
			_reallocate_map(nodes_add, false);
	}
	void reserve_map_at_front(size_type nodes_add = 1)
	{
		if (nodes_add > start.node - map)
			_reallocate_map(nodes_add, true);
	}
	void _reallocate_map(size_type nodes_add, bool add_at_front)//map重分配的主函数
	{
		size_type old_code_size = finish.node - start.node + 1;
		size_type new_code_size = old_code_size + nodes_add;
		map_pointer nstart;//重新分配的起点
		if (map_size > 2 * new_code_size)//还有超过一半的空间没有使用，只需重新组织一下即可
		{
			nstart = map + (map_size - new_code_size) / 2 + (add_at_front ? nodes_add : 0);
			if (nstart < start.node)			//说明需要向前分配
				std::copy(start.node, finish.node + 1, nstart);
			else
				std::copy_backward(start.node, finish.node + 1, nstart + old_code_size);
		}
		else						//map需要重新分配
		{
			size_type new_map_size = map_size + std::max(map_size, nodes_add)+2;
			map_pointer new_map = mapAllocator.allocate(new_map_size);
			nstart = new_map + (new_map_size - new_code_size) / 2 + (add_at_front ? nodes_add : 0);
			std::copy(start.node, finish.node + 1, nstart);
			mapAllocator.deallocate(map, map_size);
			map_size = new_map_size;
			map = new_map;
			//维护start和finish
			start.set_node(nstart);
			finish.set_node(nstart + old_code_size - 1);
		}
	}
	//对map的维护是deque类的中心，其他操作均由迭代器负责
};
#endif