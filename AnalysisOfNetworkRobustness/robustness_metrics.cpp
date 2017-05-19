#include "robustness_metrics.h"
#include <algorithm>

using std::deque;

/* 升序比较器 */
static bool asc_comp(const int & a, const int & b)
{
	return a > b;
}

/* 计算网络的平均路径长度 */
double get_apl(network & net)
{
	int count = net.number_of_nodes;
	double totalPath = 0;
	double sum = 0;

	for(int i = 0; i < count; i++)
	{
		if(net.nodes[i].status == infected) continue;
		for(int j = i + 1; j < count; j++)
		{
			if(net.distance[i][j] >= USHRT_MAX ) continue;
			totalPath++;
			sum += net.distance[i][j];
		}
	}
	return sum == 0 ? 0 : (sum  / totalPath);
}

/* 计算网络直径 */
double get_diameter(network & net)
{
	double max_dist = 0;
	int count = net.number_of_nodes;
	for(int i = 0; i < count; i++)
	{
		for(int j = 0; j < count; j++)
		{
			if(net.nodes[i].status == infected || 
			   net.nodes[j].status == infected ||
			   net.distance[i][j]  >= USHRT_MAX ) continue;
			if(net.distance[i][j] > max_dist)
			{
				max_dist = net.distance[i][j];
			}
		}
	}
	return max_dist;
}

/* 计算最大连通分量中节点数目 */
double get_giant_component_count(network & net)
{
	int count = net.number_of_nodes;
	bool * is_visited = new bool[count];
	int nodes_in_component = 0;

	//记录所有连通分量中节点的数目
	vector<int> v;

	//BFS辅助队列
	deque<int> q;

	v.push_back(0);
	//初始化布尔数组
	for(int i = 0; i < count; i++) is_visited[i] = false;

	for(int idx = 0; idx < count; idx++)
	{
		//如果节点已经失效，或节点被访问过，则跳过
		if(net.nodes[idx].status == infected || is_visited[idx]) continue;
		q.clear();

		//未被感染的节点且未被访问过，将其插入队列中，该节点所在的连通分量的节点数记为1
		q.push_back(idx);
		is_visited[idx] = true;
		nodes_in_component = 1;

		//对该节点进行bfs遍历，找到它所在的连通分量
		while(!q.empty())
		{
			int w = q.front();
			q.pop_front();

			set<unsigned short int>::iterator it;
			for(it = net.nodes[w].neighbor_indexes.begin(); it != net.nodes[w].neighbor_indexes.end(); it++)
			{
				if(net.nodes[*it].status == infected || is_visited[*it]) continue;
				q.push_back(*it);
				nodes_in_component++;
				is_visited[*it] = true;
			}
		}

		/* 找到一个新的连通子图，将该连通分量中节点数目存起来 */
		v.push_back(nodes_in_component);
	}

	//对找到的所有连通分量的节点数目从打到小进行排序
	std::sort(v.begin(), v.end(), asc_comp);
	delete[] is_visited;
	is_visited = NULL;

	//返回最大连通分量节点数
	return v.at(0);
}

/* 计算失效节点的比列 */
double get_failed_node_fraction(network & net)
{
	int failed_node_num = 0, count = net.number_of_nodes;
	for(int i = 0; i < count; i++)
	{
		if(net.nodes[i].status == infected) failed_node_num++;
	}

	return failed_node_num / (double)count;
}