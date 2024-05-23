#pragma once
#include <istream>
#include <utility>
#include <iostream>
template<typename vertype>
struct arc
{
	friend std::istream& operator>>(std::istream& in, arc<vertype>& a)
	{
		in >> a.start >> a.end >> a.weight;
		return in;
	}
	vertype start, end;
	int weight;
};


template<typename vertype>
class Mgraph
{
public:
	Mgraph(int _vexnum, int _arcnum)
		:vexnum(_vexnum), arcnum(_arcnum), vertex(new vertype[vexnum]), arcset(new int*[_vexnum])
	{
		for (auto i = 0; i < vexnum; i++)
			arcset[i] = new int[vexnum];
		for (auto i = 0; i < vexnum; i++)
			for (auto j = 0; j < vexnum; j++)
				arcset[i][j] = 0;
		for (auto i = 0; i < vexnum; i++)
			std::cin >> vertex[i];
		
		for (auto i = 0; i < arcnum; i++)
		{
			arc<vertype> tem;
			std::cin >> tem;
			arcset[locate(tem).first][locate(tem).second] = tem.weight;
			arcset[locate(tem).second][locate(tem).first] = tem.weight;
		}

		//output();
	}
	void travel()
	{
		bool* visited = new bool[vexnum];
		for (auto i = 0; i < vexnum; i++)
			visited[i] = 0;
		travel_DFS(visited, 0);
	}
	
protected:
	vertype* vertex; //µã¼¯
	int** arcset;	  //ÁÚ½Ó¾ØÕó
	int vexnum, arcnum;
	std::pair<int, int> locate(arc<vertype>& a)
	{
		std::pair<int, int> tem;
		for (auto i = 0; i < vexnum; i++)
		{
			if (vertex[i] == a.start)
				tem.first = i;
			else if(vertex[i] == a.end)
				tem.second = i;
		}

		return tem;
	}
private:	//these are tool fuction
	void output()const
	{
		for (auto i = 0; i < vexnum; i++)
		{
			for (auto j = 0; j < vexnum; j++)
				std::cout << arcset[i][j] << " "<<'\t';
			std::cout << std::endl;
		}
	}
	void travel_DFS(bool* _visited, int index)	//O(n^2)
	{
		std::cout << vertex[index];
		_visited[index] = true;
		for (auto j = 0; j < vexnum; j++)
		{
			if (arcset[index][j] != 0 && _visited[j] == 0)
				travel_DFS(_visited, j);
		}
	}
};