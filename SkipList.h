#ifndef __ZIMOSKIPLIST__
#define __ZIMOSKPLIST__

#include <iterator>
#include <memory>
#include <random>
#include <iostream>
#include <assert.h>
#define __VERSION_2__
constexpr unsigned int BRANCHING = 4;
template<typename _Ty>
struct node
{
	explicit node(const _Ty& __key)
		:key(__key) {}
	explicit node(const _Ty& __key,const int _height)	//version_2
		:key(__key),_next(new node* [_height]),_prev(new node* [_height]),height(_height)
	{
		for (int i = 0; i < _height; i++)
		{
			_next[i] = nullptr;
			_prev[i] = nullptr;
		}
	}
	node* Next(int _level)const
	{
		assert(_level >= 0);
		return _next[_level];
	}

	void set_next(int _level, node* _source)//在第_level层插入_source节点
	{
		assert(_level >= 0);
		_next[_level] = _source;
	}
#ifdef __VERSION_2__
	node* Prev(int _level)const
	{
		assert(_level >= 0);
		return _prev[_level];
	}

	void set_prev(int _level, node* _source)
	{
		//assert(_level >= 0);
		_prev[_level] = _source;
	}
#endif
	_Ty key;
	int height;
private:
#ifdef __VERSION_2__
	node** _next;//存储当前节点在某一层的下一节点
	node** _prev;//存储当前节点在某一层的上一节点
#endif
};
template<typename _Ty, typename _pointer, typename _reference>
class _skiplist_iterator
{
public:
	using iterator_category = std::bidirectional_iterator_tag();
	using value_type = _Ty;
	using reference = _reference;
	using const_reference = const _reference;
	using pointer = _pointer;
	using difference_type = ptrdiff_t;
	using self = _skiplist_iterator;

	_skiplist_iterator(pointer _ptr)
		:ptr(_ptr){}
	_skiplist_iterator()
		:ptr(nullptr){}
	_skiplist_iterator(const _skiplist_iterator& r)
		:ptr(r.ptr){}
	
	reference operator*() const{ return ptr->key; }
	//const_reference operator*()const { return ptr->key; }
	pointer operator->() const{ return ptr; }

	self& operator++()
	{
		ptr = ptr->Next(0);
		return *this;
	}
	self& operator--()
	{
		ptr = ptr->Prev(0);
		return *this;
	}
	self operator++(int)
	{
		auto temp = *this;
		++* this;
		return temp;
	}
	self operator--(int)
	{
		auto temp = *this;
		--* this;
		return temp;
	}
	bool operator==(const self& r)const { return ptr == r.ptr; }
	bool operator!=(const self& r)const { return ptr != r.ptr; }
	self& operator=(const self& r)
	{
		ptr = r.ptr;
		return *this;
	}
	node<value_type>* ptr;
};
template<class _skiplist_iterator,typename _pointer,typename _reference>
class _reverse_skiplist_iterator
{
public:
	_reverse_skiplist_iterator(_skiplist_iterator it)
		:cur(it) {}
	_reverse_skiplist_iterator()
		:cur(_skiplist_iterator(nullptr)) {}
	_reverse_skiplist_iterator& operator++()
	{
		--cur;
		return *this;
	}
	_reverse_skiplist_iterator operator++(int)
	{
		auto temp = *this;
		++* this;
		return temp;
	}
	_reverse_skiplist_iterator& operator--()
	{
		++cur;
		return *this;
	}
	_reverse_skiplist_iterator operator--(int)
	{
		auto temp = *this;
		--* this;
		return temp;
	}
	_reference operator*()
	{
		return *cur;
	}
	_pointer operator->()
	{
		return cur;
	}

	bool operator==(const _reverse_skiplist_iterator& r)
	{
		return cur == r.cur;
	}
	bool operator!=(const _reverse_skiplist_iterator& r)
	{
		return cur != r.cur;
	}
private:
	_skiplist_iterator cur;
};
template<typename _Ty,typename _Alloc = std::allocator<_Ty>,typename _Comp = std::greater<_Ty>>//type,allocator,谓词
class SkipList
{
	friend _skiplist_iterator<_Ty,node<_Ty>*,_Ty&>;
	SkipList(const SkipList& s) = delete;
	SkipList& operator=(const SkipList& s) = delete;//禁用复制与拷贝函数
public:
	using value_type = _Ty;
	using pointer = node<_Ty>*;
	using difference_type = ptrdiff_t;
	using reference = _Ty&;
	using const_reference = const _Ty&;
	using iterator = _skiplist_iterator<_Ty, node<_Ty>*, _Ty&>;
	using const_iterator = _skiplist_iterator<_Ty, const node<_Ty>*, const _Ty&>;
	using reverse_iterator = _reverse_skiplist_iterator<iterator, node<_Ty>*, _Ty&>;
	using const_reverse_iterator = _reverse_skiplist_iterator<const_iterator, const node<_Ty>*, const _Ty&>;


	//Exposed interface functions
	iterator begin()
	{
		return iterator(_head->Next(0));
	}
	iterator end()
	{
		return iterator();
	}
	const_iterator begin()const
	{
		return const_iterator(_head->Next(0));
	}
	const_iterator end()const
	{
		return const_iterator();
	}
	reverse_iterator rbegin()
	{
		return reverse_iterator(_tail);
	}
	reverse_iterator rend()
	{
		return reverse_iterator(_head);
	}
	const_reverse_iterator rbegin()const
	{
		return const_reverse_iterator(_tail);
	}
	const_reverse_iterator rend()const
	{
		return const_reverse_iterator(_head);
	}
	SkipList()
		:_max_height(1), _head(_new_node(value_type(), _k_max_height)),_tail(_head)
	{
		for (int i = 0; i < _k_max_height; i++)
		{
			_head->set_next(i, nullptr);
			_head->set_prev(i, nullptr);//version 2
		}
	}
	iterator insert(const _Ty& key)
	{
		pointer* prev = new pointer [_k_max_height];
		for (int i = 0; i < _k_max_height; i++)
			prev[i] = nullptr;
		_find_GreaterorEqual(key, prev);

		int height = _random_height();//生成一个随机高度
		if(height>_max_height)
		{
			for (int i = _get_max_height(); i < height; i++)//如果随机高度超过最大高度，用_head填充
				prev[i] = _head;
			_max_height = height;
		}

		pointer x = _new_node(key, height);//创建这个节点，这个节点将占据[0:height-1]层
		//连接链表,从下至上
		std::cout << height << std::endl;
		for (int i = 0; i < height; i++)
		{
			x->set_prev(i, prev[i]);		//version_2
			x->set_next(i, prev[i]->Next(i));	
			prev[i]->set_next(i, x);
			if (x->Next(i) != nullptr)
				x->Next(i)->set_prev(i, x);	//version_2
		}
		if (!x->Next(0))
			_tail = x;
		return iterator(x);
	}
	iterator contain(const _Ty& key)
	{
		//pointer p = _find_GreaterorEqual(key, nullptr);//没有必要存下路径
		//if (p && p->key == key)
		//	return true;
		//else
		//	return false;
		pointer p = _head;
		int level = _get_max_height() - 1;
		while (level>=0)
		{
			pointer next = p->Next(level);
			if (next && _comp(key, next->key))
				p = next;
			else
			{
				if (next && next->key == key)
					return iterator(next);
				level--;
			}
		}
		return iterator();
	}
	iterator erase(const _Ty& key)
	{
		//不使用_find_GreatororEqual函数，以提高删除效率
		pointer p = _head;
		int level = _get_max_height() - 1;
		iterator _it;
		pointer temp = nullptr;
		while (true)
		{
			pointer next = p->Next(level);
			if (next && _comp(key, next->key))//还没找到位置
				p = next;
			else
			{
				if (next && next->key == key)
				{
					temp = next;
					if (next->Next(level) != nullptr)
						next->Next(level)->set_prev(level, p);
					p->set_next(level, next->Next(level));
					_it = iterator(next->Next(level));
				}
				if (level == 0)
				{
					delete temp;
					return _it;
				}
				else
					level--;
			}
		}
		return _it;
	}
	iterator erase(const iterator& pos)//This method is not recommended unless you must use an iterator
	{
#ifdef __VERSION_1__
		pointer* prev = new pointer[_get_max_height()];
		pointer p = _find_GreaterorEqual(*pos, prev);
		pointer t = p->Next(0);
		if (!p->Next(0))return iterator();//删除对象不存在
		if (p->Next(0)->key == *pos)
		{
			int i;
			for (i = 0; i < _get_max_height(); i++)
			{
				if (iterator(prev[i]->Next(i))!=pos)
					break;
				prev[i]->set_next(i, p->Next(i)->Next(i));
			}
			_max_height = i;
			delete t;
			return iterator(p->Next(0));
		}
		else
			return iterator();
#endif
#ifdef __VERSION_2__
		pointer p = pos.ptr;
		for (int i = 0; i < p->height; i++)
		{
			p->Prev(i)->set_next(i, p->Next(i));
			if (p->Next(i))
				p->Next(i)->set_prev(i,p->Prev(i));
		}
		if (!p->Next(0))
			_tail = p->Prev(0);
		_max_height = p->height;
		auto t = p->Next(0);
		delete p;
		return iterator(t);
#endif
	}
#ifdef __VERSION_2__
	void pop_back()
	{
		for (int i = 0; i < _tail->height; i++)
			_tail->Prev(i)->set_next(i, nullptr);
		_max_height = _tail->height;
		auto p = _tail;
		_tail = _tail->Prev(0);
		delete p;
	}
	void pop_front()
	{
		auto p = _head->Next(0);
		if (p)
		{
			for (int i = 0; i < p->height; i++)
			{
				_head->set_next(i, p->Next(i));
				if (p->Next(i))
				{
					p->Next(i)->set_prev(i, _head);
				}
			}
		}
		if (!p->Next(0))
			_tail = _head;
		_max_height = p->height;
	}
#endif
private:
	static const int _k_max_height = 12;
	int _max_height;
	
	pointer const _head;
	pointer _tail;
	_Comp _comp;
	_Alloc _alloc;

	//Tool function
	const int _get_max_height()const { return _max_height; }
	pointer _new_node(const _Ty& _key, int height)//给出新建节点的值和要插入的层数
	{
		//char* node_memory = (char*)_alloc.allocate(sizeof(node<value_type>) + sizeof(pointer) * (height - 1));
		//return new (node_memory)node<value_type>(_key);

#ifdef __VERSION_2__
		return new node<value_type>(_key,height);
#endif
	}
	pointer _find_GreaterorEqual(const _Ty& _key, pointer* prev)const//prev负责储存寻找过程中在_key之前的节点
	{
		pointer p = _head;
		int level = _get_max_height()-1;
		while (true)
		{
			pointer next = p->Next(level);
			if (next && _comp(_key, next->key))
				p = next;
			else
			{
				if (prev)prev[level] = p;//如果有记录路径的需求，则将其存储起来
				if (!level)return p;
				else
					level--;
			}
		}
	}
	int _random_height()const
	{
		int height = 1;
		std::default_random_engine e;
		while (height < _k_max_height && (rand() % BRANCHING == 0))//生成的随机height一定是小于最大height
			height++;
		return height;
	}
};
#endif
