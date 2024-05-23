#pragma once
#include "signal.h"
#include <initializer_list>

namespace ztl
{
	template<typename T>
	struct Qlistnode
	{
		T data;
		struct Qlistnode<T>* next;

		Qlistnode()
			:next(nullptr) {}
		Qlistnode(T elem)
			:data(elem), next(nullptr) {}
	};
	template<typename T>
	class Queue
	{
	public:
		Queue()
			:head(nullptr) {}
		Queue(std::initializer_list<T> li)
		{
			if (!li.size())return;
			Qlistnode<T>* temp = head = new Qlistnode<T>(*li.begin());

			for (auto it = li.begin() + 1; it != li.end(); it++)
			{
				temp->next = new Qlistnode<T>(*it);
				temp = temp->next;
			}
		}
		Queue(const Queue<T>& other)
		{
			free();
			if (other.empty())return;
			Qlistnode<T>* tem1 = other.head;
			Qlistnode<T>* tem2 = head = new Qlistnode<T>(*tem1);
			while (tem1->next != nullptr)
			{
				tem1 = tem1->next;
				tem2->next = new Qlistnode<T>(tem1->data);
				tem2 = tem2->next;
			}
		}
		~Queue()
		{
			free();
		}

		Queue& operator=(const Queue<T>& other)
		{
			free();
			if (other.empty())return;
			Qlistnode<T>* tem1 = other.head;
			Qlistnode<T>* tem2 = head = new Qlistnode<T>(*tem1);
			while (tem1->next != nullptr)
			{
				tem1 = tem1->next;
				tem2->next = new Qlistnode<T>(tem1->data);
				tem2 = tem2->next;
			}
			return *this;
		}
		bool empty()const
		{
			return !head;
		}
		int size()const
		{
			if (empty())return 0;
			int cnt = 1;
			Qlistnode<T>* tem = head;
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
				head = new Qlistnode<T>(elem);
				return OK;
			}
			Qlistnode<T>* tem = head;
			while (tem->next != nullptr)
				tem = tem->next;
			tem->next = new Qlistnode<T>(elem);
			if (!tem->next)return OVERFLOW;
			return OK;
		}
		const T& back()const
		{
			if (empty())throw("队列内无元素");
			Qlistnode<T>* tem = head;
			while (tem->next != nullptr)
				tem = tem->next;
			return tem->data;
		}
		const T& front()const
		{
			if (empty())throw("队列内无元素");
			return head->data;
		}
		void pop()
		{
			Qlistnode<T>* tem = head;
			head = head->next;
			delete tem;
		}
		void swap(const Queue& other)
		{
			Qlistnode<T>* tem = head;
			head = other.head;
			other.head = tem;
		}

	protected:
		Qlistnode<T>* head;
		void free()
		{
			Qlistnode<T>* tem = head;
			while (head != nullptr)
			{
				head = head->next;
				delete tem;
				tem = head;
			}
		}
	};
}