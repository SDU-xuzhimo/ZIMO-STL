#pragma once
#include "signal.h"
#include <initializer_list>

namespace ztl
{
	template<typename T>
	struct Slistnode
	{
		T data;
		struct Slistnode<T>* next;

		Slistnode()
			:next(nullptr) {}
		Slistnode(T elem)
			:data(elem), next(nullptr) {}

	};

	template<typename T>
	class stack
	{
	public:
		stack()
			:head(nullptr) {}
		stack(std::initializer_list<T> li)
		{
			if (!li.size())
				return;
			Slistnode<T>* temp = head = new Slistnode<T>(*li.begin());

			for (auto it = li.begin() + 1; it != li.end(); it++)
			{
				temp->next = new Slistnode<T>(*it);
				temp = temp->next;
			}
		}
		stack(const stack& other)
		{
			free();
			if (other.empty())
				return;
			Slistnode<T>* tem1 = other.head;
			Slistnode<T>* tem2 = head = new Slistnode<T>(*tem1);

			while (tem1->next != nullptr)
			{
				tem1 = tem1->next;
				tem2->next = new Slistnode<T>(tem1->data);
				tem2 = tem2->next;
			}
		}
		~stack()
		{
			free();
		}

		bool empty()const
		{
			return !head;
		}
		int size()const
		{
			if (empty())return 0;
			int cnt = 1;
			Slistnode<T>* tem = head;
			while (tem->next != nullptr)
			{
				cnt++;
				tem = tem->next;
			}

			return cnt;
		}

		status push(T elem)
		{
			if (empty())
			{
				head = new Slistnode<T>(elem);
				return OK;
			}
			Slistnode<T>* tem = head;
			while (tem->next != nullptr)
				tem = tem->next;
			tem->next = new Slistnode<T>(elem);
			if (!tem->next)return OVERFLOW;
			return OK;
		}
		void pop()
		{
			if (empty())
				return;
			Slistnode<T>* tem = head;
			while (tem->next->next != nullptr)
				tem = tem->next;
			delete tem->next;
			tem->next = nullptr;
		}
		const T& top()const
		{
			if (empty())throw("Õ»ÄÚÎÞÔªËØ");
			Slistnode<T>* tem = head;
			while (tem->next != nullptr)
				tem = tem->next;
			return tem->data;
		}
		void swap(stack<T>& other)
		{
			Slistnode<T>* tem = head;
			head = other.head;
			other.head = tem;
		}
	protected:
		Slistnode<T>* head;
		void free()
		{
			Slistnode<T>* temp = head;
			while (head != nullptr)
			{
				head = head->next;
				delete temp;
				temp = head;
			}
			head = nullptr;
		}
	};
}
