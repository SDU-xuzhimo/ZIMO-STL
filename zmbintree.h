#pragma once
#include "signal.h"
#include <initializer_list>
#include <iostream>

namespace ztl
{
	template<typename T>
	struct BTnode
	{
		T data;
		class BTnode<T>* lchild;
		class BTnode<T>* rchild;

		BTnode(T elem)
			:data(elem), lchild(nullptr), rchild(nullptr) {}
	};

	template<typename T>
	class BinTree
	{
	public:
		BinTree(std::initializer_list<T> li)
			:bli(li),size(li.size())
		{
			BTcreat(bt);
		}
		//~BinTree();
		BinTree(const BinTree& other)
			:size(other.size)
		{
			BTcopy(other.bt, bt);
		}
		BinTree& operator=(const BinTree& other)
		{
			BTcopy(other.bt, bt);
		}
		bool empty()const
		{
			return !bt;
		}


		void Travel(/*const char* fp(BTnode<T> bnode)*/)
		{
			Travel_DLR(bt/*, fp*/);
		}

	protected:
		BTnode<T>* bt;
		std::initializer_list<T> bli;
		int size;
		void BTcreat(BTnode<T>*& bp)//DLR
		{
			static auto it = bli.begin();
			if (it == bli.end())return;
			if (*it == 0)
			{
				bp = nullptr;
				it++;
			}
			else
			{
				bp = new BTnode<T>(*it);
				it++;
				BTcreat(bp->lchild);
				BTcreat(bp->rchild);
			}
		}
		void BTcopy(const BTnode<T>* bp, BTnode<T>*& bd)
		{
			if (bp == nullptr)
			{
				bd = nullptr;
				return;
			}
			bd = new BTnode<T>(bp->data);
			BTcopy(bp->lchild, bd->lchild);
			BTcopy(bp->rchild, bd->rchild);
		}

		void Travel_DLR(BTnode<T>* bp)//, void fp(BTnode<T> bnode))
		{
			if (bp == nullptr) return;
			std::cout << bp->data;
			Travel_DLR(bp->lchild/*, fp*/);
			Travel_DLR(bp->rchild/*, fp*/);
		}
		void Travel_LDR(BTnode<T>* bp, void* fp())
		{
			if (bp == nullptr)return;
			Travel_DLR(bt->lchild, fp);
			fp(bt);
			Travel_DLR(bt->rchild, fp);
		}
		void Travel_LRD(BTnode<T>* bp, void* fp())
		{
			if (bp == nullptr)return;
			Travel_DLR(bt->lchild, fp);
			Travel_DLR(bt->rchild, fp);
			fp(bt);
		}
	};
}
