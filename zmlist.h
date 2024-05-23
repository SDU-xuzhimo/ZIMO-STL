#pragma once

#include "signal.h"
#include <initializer_list>
//#include <iostream>

namespace ztl
{
	template<typename T>
	struct list_node
	{
		T data;
		struct list_node* next;

		list_node<T>()
			:next(nullptr) {}
		list_node<T>(T _data)
			: data(_data), next(nullptr) {}

	};
	template<typename T>
	class link_list
	{
		//friend std::ostream& operator<<(std::ostream& os, const link_list<T>& li);
	public:
		link_list()//默认构造
			:head(nullptr) {}
		link_list(const link_list<T>& other)//复制构造
		{
			if (other.empty()) head(nullptr);
			else
			{
				list_node<T>* tem1 = other;
				list_node<T>* tem2 = head = new list_node<T>(tem1->data);
				while (!tem1->next)
				{
					tem1 = tem1->next;
					tem2->next = new list_node<T>(tem1->data);
					tem2 = tem2->next;
				}
			}
		}
		link_list(std::initializer_list<T> init)
		{
			list_node<T>* temp = head = new list_node<T>(*init.begin());
			for (auto it = init.begin() + 1; it != init.end(); it++)
			{
				temp->next = new list_node<T>(*it);
				temp = temp->next;
			}
		}
		~link_list()
		{
			list_node<T>* temp = head;
			while (head)
			{
				head = head->next;
				delete temp;
				temp = head;

			}
		}

		bool empty()const
		{
			return head;
		}
		int size()const
		{
			list_node<T>* temp = head;
			int cnt = 0;
			while (temp != nullptr)
			{
				cnt++;
				temp = temp->next;
			}
			return cnt;
		}
		status add_back(T elem)
		{
			list_node<T>* temp = head;
			if (temp == nullptr)
			{
				temp = new list_node<T>(elem);
				if (temp == nullptr)
					return OVERFLOW;
				return OK;
			}
			else
			{
				while (temp->next != nullptr)
				{
					temp = temp->next;
				}
				temp->next = new list_node<T>(elem);
				if (temp->next->next == nullptr)
					return OVERFLOW;
				return OK;
			}
		}
		status erase(int index)
		{
			if (index > size())return ERROR;
			list_node<T>* temp = head;
			int cnt = 0;
			while (cnt < index)
			{
				cnt++;
				temp = temp->next;
			}
			list_node<T>* tem1 = temp->next;
			temp->next = tem1->next;
			delete tem1;
			return OK;
		}
		T& get(int index)
		{
			if (index > size())throw("索引超限");
			int cnt = 1;
			list_node<T>* tem1 = head;
			while (cnt < index)
			{
				tem1 = tem1->next;
				cnt++;
			}
			return tem1->data;
		}
		//int indexof(const T& elem)const;
		/*status travel()const
		{
			list_node<T>* tem = head;

		}*/

	protected:
		list_node<T>* head;

	};
	//template<typename T>
	//std::ostream& operator<<(std::ostream& os, const link_list<T>& li)
	//{
	//	list_node<T>* tem = li.head;
	//	while (li.tem)
	//	{
	//		os << tem->data;
	//		tem = tem->next;
	//	}
	//	return os;
	//}
}
