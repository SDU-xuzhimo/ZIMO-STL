#pragma once
#include "Vector.h"
#include <initializer_list>
template<typename elemtype,template<typename elemtype,typename ALLOC = std::allocator<elemtype> > class container = std::vector>
class maxheap
{
public:
	maxheap()
		:heap(){}
	maxheap(const std::initializer_list<elemtype>& li)
	{
		for (auto it = li.begin(); it != li.end(); it++)
			push(*it);
	}
	maxheap(const container<elemtype>& _Mycontainer)//���ݸ�������ʽ����������
	{
		for (auto it = _Mycontainer.begin(); it != _Mycontainer.end(); it++)
			push(*it);
	}
	void push(const elemtype& _val)
	{
		int currentnode = heap.size();
		heap.push_back(_val);
		while (currentnode != 0 && heap[currentnode/2] < _val)//��鵽�����λ�õĸ��ڵ����С
		{
			heap[currentnode] = heap[currentnode / 2];
			currentnode /= 2;
		}
		heap[currentnode] = _val;
	}
	void pop()
	{
		if (heap.empty())throw("heap is empty");
		if (heap.size() == 1)
		{
			heap.pop_back();
			return;
		}
		auto temp = heap[heap.size() - 1];	//�����һ��Ԫ�ص�λ�ÿ������
		heap.pop_back();

		int currentNode = 0;//�������Ľڵ㣬��root��ʼ
		int child = 1;		//�����ĺ��ӽڵ�
		while (true)
		{
			if (child >= heap.size())
			{
				heap[currentNode] = temp;
				break;
			}
			else if (temp > heap[child] && temp > heap[child + 1])
			{
				heap[currentNode] = temp;
				break;
			}
			else
			{
				child = heap[child] > heap[child + 1] ? child : child + 1;
				heap[currentNode] = heap[child];	//�������нϴ��һ������currentNode
				currentNode = child;
				child *= 2;			//�Ƶ���һ�㿪ʼѰ��
			}
		}
	}

	elemtype& front()
	{
		if (heap.empty())
			throw("heap is empty");
		return heap[0];
	}
	const elemtype& front()const
	{
		if (heap.empty())
			throw("heap is empty");
		return heap[0];
	}

private:
	Vector<elemtype> heap;
};
