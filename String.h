#ifndef _ZIMOSTRING_
#define _ZIMOSTRING_
#include "iterator.h"
#include <memory>
#include <stdexcept>
#include <istream>
#include <ostream>
#include <utility>
#pragma warning(disable:4715)
class String
{
	friend String operator+(const String& s1,const String& s2);
	friend std::istream& operator>>(std::istream& in, String& s);
	friend std::ostream& operator<<(std::ostream& os, const String& s);
public:
	String()
		:bgPtr(nullptr), first_free(nullptr), endPtr(nullptr){}
	String(const char* s)
	{
		int s_len = strlen(s);
		bgPtr = alloc.allocate(s_len);
		char* temp = bgPtr;
		for (int i = 0; i < s_len; i++)
		{
			alloc.construct(temp++, s[i]);
		}
		first_free = endPtr = temp;
	}
	String(int n, char c)
	{
		bgPtr = alloc.allocate(n);
		char* temp = bgPtr;
		for (int i = 0; i < n; i++)
			alloc.construct(temp++, c);
		endPtr = first_free = temp;
	}
	String(const String& s)
	{
		char* tem1 = s.bgPtr;
		char* tem2 = s.first_free;

		bgPtr = alloc.allocate(s.size());
		char* tem = bgPtr;
		for (int i = 0; i < s.size(); i++)
		{
			alloc.construct(tem++, *tem1++);
		}
		first_free = endPtr = tem;
	}
	String(String&& s)noexcept
		:bgPtr(s.bgPtr), first_free(s.first_free), endPtr(s.endPtr)
	{
		s.bgPtr = s.first_free = s.endPtr = nullptr;
	}
	~String()
	{
		free();
	}

	char& operator[](int n)
	{
		if (n >= size())
			throw std::range_error("下标越界");
		else
		{
			auto temp = bgPtr;
			return *(temp + n);
		}
	}
	char operator[](int n)const
	{
		if (n >= size())
			throw std::range_error("下标越界");
		else
		{
			auto temp = bgPtr;
			return *(temp + n);
		}
	}

	String& operator=(const String& s)
	{
		free();
		auto temp = bgPtr = alloc.allocate(s.size());
		for (auto i = 0; i != s.size(); i++)
			alloc.construct(temp++, s[i]);
		first_free = endPtr = bgPtr + s.size();
		return *this;
	}
	String& operator=(char* c)
	{
		free();
		auto temp = bgPtr = alloc.allocate(strlen(c));
		for (int i = 0; i != strlen(c); i++)
			alloc.construct(temp++, c[i]);
		first_free = endPtr = bgPtr + strlen(c);
		return *this;
	}

	constexpr int size()const
	{
		return first_free - bgPtr;
	}
	constexpr int capacity()const
	{
		return endPtr - bgPtr;
	}
	bool empty()const
	{
		if (size())
			return false;
		else
			return true;
	}

	String& operator+=(const String& other)
	{
		check(other.size());
		for (auto i = 0; i < other.size(); i++)
			alloc.construct(first_free++, other[i]);
		return *this;
	}
	bool operator==(const String& other)const
	{
		int count = 0;
		if (size() != other.size())
			return false;
		else
		{
			for (auto i = 0; i < size(); i++)
			{
				if (( * this)[i] == other[i])
					count++;
				else
					continue;
			}
			if (count != size())
				return false;
			else
				return true;
		}
	}

	void push_back(const char _ch)
	{
		check();
		alloc.construct(first_free++, _ch);
	}
	void pop_back()
	{
		if (empty())
			return;
		else
			alloc.destroy(--first_free);
	}
	void shrink_to_fit()
	{
		char* tem, *tem1;
		tem1 = tem = alloc.allocate(size());
		for (auto i = 0; i < size(); i++)
			*tem++ = std::move((*this)[i]);
		free();
		bgPtr = tem1;
		first_free = endPtr = tem;
	}
	const char* c_str()
	{
		push_back('\0');
		return bgPtr;
	}

	class iterator
	{
	public:
		typedef random_access_iterator_tag iterator_category;
		typedef char value_type;
		typedef ptrdiff_t difference_type;
		typedef char* pointer;
		typedef char& reference;

		iterator(char* _ptr)
			:ptr(_ptr) {}

		reference operator*()const { return *ptr; }
		pointer operator->()const { return ptr; }

		iterator operator+(difference_type off)const { return iterator(ptr + off); }
		iterator& operator++() { ++ptr; return *this; }
		iterator operator++(int) { iterator temp = *this; ++(*this); return temp; }
		iterator& operator--() { --ptr; return *this; }
		iterator operator--(int) { iterator tem = *this; --(*this); return tem; }
		iterator operator-(difference_type off) { return iterator(ptr - off); }
		difference_type operator-(const iterator& rhs)const { return ptr - rhs.ptr; }

		bool operator<(const iterator& rhs)const { return ptr < rhs.ptr; }
		bool operator>(const iterator& rhs)const { return ptr > rhs.ptr; }
		bool operator==(const iterator& rhs)const { return ptr == rhs.ptr; }
		bool operator!=(const iterator& rhs)const { return ptr != rhs.ptr; }
	private:
		char* ptr;
	};
	class const_iterator
	{
	public:
		typedef random_access_iterator_tag iterator_category;
		typedef const char value_type;
		typedef ptrdiff_t difference_type;
		typedef const char* pointer;
		typedef const char& reference;

		const_iterator(const char* _ptr)
			:ptr(_ptr) {}

		reference operator*()const { return *ptr; }
		pointer operator->()const { return ptr; }

		const_iterator operator+(difference_type off)const { return const_iterator(ptr + off); }
		const_iterator& operator++() { ++ptr; return *this; }
		const_iterator operator++(int) { const_iterator temp = *this; ++(*this); return temp; }
		const_iterator& operator--() { --ptr; return *this; }
		const_iterator operator--(int) { const_iterator temp = *this; --(*this); return temp; }
		const_iterator operator-(difference_type off)const { return const_iterator(ptr - off); }
		difference_type operator-(const const_iterator& rhs)const { return ptr - rhs.ptr; }

		bool operator<(const const_iterator& rhs)const { return ptr < rhs.ptr; }
		bool operator>(const const_iterator& rhs)const { return ptr > rhs.ptr; }
		bool operator==(const const_iterator& rhs)const { return ptr == rhs.ptr; }
		bool operator!=(const const_iterator& rhs)const { return ptr != rhs.ptr; }
	private:
		const char* ptr;
	};
	iterator begin() { return iterator(bgPtr); }
	iterator end() { return iterator(first_free); }
	const_iterator begin()const { return const_iterator(bgPtr); }
	const_iterator end()const { return const_iterator(first_free); }
private:
	std::allocator<char> alloc;
	char* bgPtr;
	char* first_free;
	char* endPtr;

	//these are tool method
	void check()
	{
		if (size() == capacity())
			reallocate();
	}
	void check(int etc_size)
	{
		if (capacity() <size() + etc_size)
			reallocate();
	}
	void reallocate()
	{
		auto newcapacity = size() ? size() * 2 : 1;
		auto newbegin = alloc.allocate(newcapacity);
		auto temp = newbegin;
		auto oldtemp = bgPtr;
		for (int i = 0; i < size(); i++)
			alloc.construct(temp++, std::move(*oldtemp++));
		free();

		bgPtr = newbegin;
		first_free = temp;
		endPtr = bgPtr + newcapacity;
	}
	void free()
	{
		for (; first_free != bgPtr;)
			alloc.destroy(--first_free);
		alloc.deallocate(bgPtr, capacity());
		bgPtr = first_free = endPtr = nullptr;
	}
};

bool operator<(const String& s1,const String& s2)
{
	if (s1 == s2)
		return 0;
	else
	{

		if (s1.size() > s2.size())
			return 0;
		else if (s1.size() == s2.size())
		{
			int i = 0;
			while (i < s1.size())
			{
				if (s1[i] < s2[i])
				{
					return 1;
				}
				else if (s1[i] == s2[i])
				{
					i++;
					continue;
				}
				else
					return 0;
				i++;
			}
		}
		else
			return 1;
	}
}
String operator+(const String& s1, const String& s2)
{
	char* tems = new char[s1.size() + s2.size()+1];
	for (auto i = 0; i < s1.size(); i++)
		tems[i] = s1[i];
	for (auto i = 0; i < s2.size(); i++)
		tems[i + s1.size()] = s2[i];
	tems[s1.size() + s2.size()] = '\0';
	return String(tems);
}

std::ostream& operator<<(std::ostream& os, const String& s)
{
	for (auto i = 0; i < s.size(); i++)
		os << s[i];
	return os;
}
std::istream& operator>>(std::istream& in, String& s)
{
	s.free();
	int ch;
	while ((ch = in.get()) != EOF&&ch!='\n')
		s.push_back(ch);
	return in;
}


#endif 