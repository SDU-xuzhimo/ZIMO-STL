#pragma once
#include <iostream>
#include <utility>
template<typename vertype>
struct arc
{
	friend std::istream& operator>>(std::istream& in, arc<vertype>& a)
	{
		in >> a.start >> a.end >>a.weight;
		return in;
	}
	vertype start, end;
	int weight;
};
struct arcnode
{
	int index;
	arcnode* firstarc;
	int weight;

	arcnode(int _index, int _weight)
		:index(_index),firstarc(nullptr), weight(_weight){}
};
template<typename vertype>
struct vertex
{
	vertype data;
	arcnode* first;
};
template<typename vertype>
class Lgraph
{
public:
	Lgraph(int _vernum, int _arcnum)
		:vernum(_vernum), arcnum(_arcnum), ver(new vertex<vertype>[_vernum])
	{
		for (auto i = 0; i < vernum; i++)
		{
			std::cin >> ver[i].data;
			ver[i].first = nullptr;
		}
		for (auto i = 0; i < arcnum; i++)
		{
			arc<vertype> tem;
			std::cin >> tem;
			arcnode* p1 = ver[locate(tem).first].first;
			ver[locate(tem).first].first = new arcnode(locate(tem).second, tem.weight);
			ver[locate(tem).first].first->firstarc = p1;
			arcnode* p2 = ver[locate(tem).second].first;
			ver[locate(tem).second].first = new arcnode(locate(tem).first, tem.weight);
			ver[locate(tem).second].first->firstarc = p2;
		}
		//output();
	}
	void travel()
	{
		bool* visited = new bool[vernum];
		for (auto i = 0; i < vernum; i++)
			visited[i] = false;
		travel_DFS(visited, 0);
	}
protected:
	vertex<vertype>* ver;
	int vernum, arcnum;
	std::pair<int, int> locate(arc<vertype>& a)
	{
		std::pair<int, int> tem;
		for (auto i = 0; i < vernum; i++)
		{
			if (ver[i].data == a.start)
				tem.first = i;
			else if (ver[i].data == a.end)
				tem.second = i;
		}
		return tem;
	}

private:
	void output()const
	{
		vertype tem;
		std::cin >> tem;
		int index;
		for (auto i = 0; i < vernum; i++)
			if (ver[i].data == tem)
				index = i;
		const arcnode* p = ver[index].first;
		while (p != nullptr)
		{
			std::cout << ver[p -> index].data;
			p = p->firstarc;
		}
	}
	void travel_DFS(bool* visited, int index)
	{
		std::cout << ver[index].data;
		visited[index] = true;
		arcnode* p = ver[index].first;
		while (p != nullptr)
		{
			if (visited[p->index] == 0)
				travel_DFS(visited, p->index);
			else
				p = p->firstarc;
		}
	}
};
