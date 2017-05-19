#include "algorithms_implementation.h"

using std::priority_queue;
using std::list;
using std::deque;

/**/
static void set_up_all_nodes(network & net)
{
	for(int idx = 0; idx < net.number_of_nodes; idx++)
	{
		net.nodes[idx].predeccesors.clear();
		net.nodes[idx].sigma    = 0;
		net.nodes[idx].distance = INT_MAX;
		net.nodes[idx].delta    = 0;
	}
}

/* 无权图最短路径BFS算法 
 * sigma(s, t)表示节点i到节点j的最短路径的总条数
 */
priority_queue<node> * bfs(unsigned short int source, network & net)
{
	list<unsigned short int> Q;
	priority_queue<node> * S = new priority_queue<node>();

	/* sigma(s, s) = 1, distance(s, s) = 0*/
	set_up_all_nodes(net);
	Q.push_back(source);
	net.nodes[source].sigma    = 1.0;
	net.nodes[source].distance = 0;

	while(!Q.empty())
	{
		unsigned short int v = Q.front();
		Q.pop_front();

		S->push(net.nodes[v]);

		set<unsigned short int> neighbors = net.nodes[v].neighbor_indexes;
		set<unsigned short int>::iterator it;
		for(it = neighbors.begin(); it != neighbors.end(); it++)
		{
			if(net.nodes[*it].status == infected) continue;
			if(net.nodes[*it].distance == INT_MAX)
			{
				net.nodes[*it].distance = net.nodes[v].distance + 1;
				net.distance[source][*it] = net.distance[source][v] + 1;
				Q.push_back(*it);
			}

			if(net.nodes[*it].distance == net.nodes[v].distance + 1)
			{
				net.nodes[*it].sigma += net.nodes[v].sigma;
				net.nodes[*it].predeccesors.insert(v);
			}
		}
	}
	return S;
}

/*
void bfs_from_vertex(network & net, unsigned short int s)
{
	int count = net.number_of_nodes;
	
	//辅助数组和队列
	bool * is_visited = new bool[count];
	deque<unsigned short int> q;

	//初始化
	q.clear();
	q.push_back(s);
	is_visited[s] = true;

	while(!q.empty())
	{
		int v = q.front();
		q.pop_front();

		for(int i = 0; i < count; i++)
		{
			if(net.adjacency_matrix[v][i] && !is_visited[i])
			{
				net.distance[s][i] = net.distance[s][v] + 1;
				q.push_back(i);
				is_visited[i] = true;
			}
		}
	}

	delete[] is_visited;
	is_visited = NULL;
}

void shortest_path_bfs(network & net)
{
	int count = net.number_of_nodes;

	for(int i = 0; i < count; i++)
	{
		if(net.nodes[i].status == infected) continue;
		bfs_from_vertex(net, i);
	}
}
*/


/* 无权图最短路径的广度优先搜索算法 */
void shortest_path_bfs(network & net)
{
	int count = net.number_of_nodes;

	//bfs辅助队列及辅助数组
	deque<unsigned short int> q;
	bool * is_visited = new bool[count];

	//对每一个节点进行bfs搜索，找出该节点与其他节点之间的最短路径长度
	for(int s = 0; s < count; s++)
	{
		if(net.nodes[s].status == infected) continue;
		for(int i = 0; i < count; i++)	is_visited[i] = false;
		
		//辅助数据结构的初始化
		q.clear();

		//开始bfs搜索
		q.push_back(s);
		is_visited[s] = true;

		while(!q.empty())
		{
			unsigned short int v = q.front();
			q.pop_front();

			for(int i = 0; i < count; i++)
			{
				if(!net.adjacency_matrix[v][i] || is_visited[i]) continue;
				net.distance[s][i] = net.distance[s][v] + 1;
				q.push_back(i);
				is_visited[i] = true;
			}
		}
	}

	delete[] is_visited;
	is_visited = NULL;
	return;
}