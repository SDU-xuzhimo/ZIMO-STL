#pragma once
#include <locale>
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag :public input_iterator_tag {};
struct bidirectional_iterator_tag :public forward_iterator_tag {};
struct random_access_iterator_tag :public bidirectional_iterator_tag {};
//不同类型迭代器标志

template<typename _Category,typename _Tp,typename _Distance = ptrdiff_t,
typename _Pointer = _Tp*,typename _Reference = _Tp&>
struct iterator		//迭代器空基类
{
	typedef typename _Category iterator_category;
	typedef typename _Tp value_type;
	typedef typename _Distance difference_type;
	typedef typename _Pointer pointer;
	typedef typename _Reference reference;
};

template<typename _Iterator>
struct iterator_traits
{
	typedef typename _Iterator::iterator_category iterator_category;
	typedef typename _Iterator::value_type value_type;
	typedef typename _Iterator::difference_type difference_type;
	typedef typename _Iterator::pointer pointer;
	typedef typename _Iterator::reference reference;
};
template<typename _Tp>
struct iterator_traits<_Tp*>		//原生指针特例化
{
	typedef typename random_access_iterator_tag iterator_category;
	typedef typename _Tp valuetype;
	typedef typename ptrdiff_t difference_type;
	typedef typename _Tp* pointer;
	typedef typename _Tp& reference;
};
template<typename _Tp>
struct iterator_traits<const _Tp*>
{
	typedef typename random_access_iterator_tag iterator_category;
	typedef typename const _Tp valuetype;
	typedef typename ptrdiff_t difference_type;
	typedef typename const _Tp* pointer;
	typedef typename const _Tp& reference;
};

template <class _Iter>
inline typename iterator_traits<_Iter>::iterator_category
__iterator_category(const _Iter&)
{
	typedef typename iterator_traits<_Iter>::iterator_category _Category;
	return _Category();
}

template <class _Iter>
inline typename iterator_traits<_Iter>::iterator_category
iterator_category(const _Iter& __i) { return __iterator_category(__i); }

template<class InputIterator, class _Diff>
inline void _advance(InputIterator& _where, _Diff _Off,input_iterator_tag)
{
	while (_Off--)++_where;
}

template<class OutputIterator, class _Diff>
inline void _advance(OutputIterator& _where, _Diff _Off,output_iterator_tag)
{
	while (_Off--)++_where;
}

template<class ForwardIterator, class _Diff>
inline void _advance(ForwardIterator& _where, _Diff _Off, forward_iterator_tag)
{
	_advance(_where, _Off, input_iterator_tag);
}

template<class BidirectIterator, class _Diff>
inline void _advance(BidirectIterator& _where, _Diff _Off, bidirectional_iterator_tag)
{
	if (_Off >= 0)
		while (_Off--)
			_where++;
	else
		while (_Off++)
			_where--;
}

template<class RandomAccessIterator, class _Diff>
inline void _advance(RandomAccessIterator& _where, _Diff _Off, random_access_iterator_tag)
{
	_where += _Off;
}

template<class InputIterator,class _Diff>
void advance(InputIterator& _where, _Diff _Off)
{
	typedef typename iterator_traits<InputIterator>::iterator_category category;
	_advance(_where, _Off, category());
}