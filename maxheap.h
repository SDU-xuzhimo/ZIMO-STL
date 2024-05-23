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
	maxheap(const container<elemtype>& _Mycontainer)//兼容各种序列式容器的最大堆
	{
		for (auto it = _Mycontainer.begin(); it != _Mycontainer.end(); it++)
			push(*it);
	}
	void push(const elemtype& _val)
	{
		int currentnode = heap.size();
		heap.push_back(_val);
		while (currentnode != 0 && heap[currentnode/2] < _val)//检查到其插入位置的父节点比其小
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
		auto temp = heap[heap.size() - 1];	//将最后一个元素的位置空余出来
		heap.pop_back();

		int currentNode = 0;//检索到的节点，从root开始
		int child = 1;		//检索的孩子节点
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
				heap[currentNode] = heap[child];	//将孩子中较大的一个置于currentNode
				currentNode = child;
				child *= 2;			//移到下一层开始寻找
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
