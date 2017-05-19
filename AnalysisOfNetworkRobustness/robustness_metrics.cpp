#include "robustness_metrics.h"
#include <algorithm>

using std::deque;

/* ����Ƚ��� */
static bool asc_comp(const int & a, const int & b)
{
	return a > b;
}

/* ���������ƽ��·������ */
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

/* ��������ֱ�� */
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

/* ���������ͨ�����нڵ���Ŀ */
double get_giant_component_count(network & net)
{
	int count = net.number_of_nodes;
	bool * is_visited = new bool[count];
	int nodes_in_component = 0;

	//��¼������ͨ�����нڵ����Ŀ
	vector<int> v;

	//BFS��������
	deque<int> q;

	v.push_back(0);
	//��ʼ����������
	for(int i = 0; i < count; i++) is_visited[i] = false;

	for(int idx = 0; idx < count; idx++)
	{
		//����ڵ��Ѿ�ʧЧ����ڵ㱻���ʹ���������
		if(net.nodes[idx].status == infected || is_visited[idx]) continue;
		q.clear();

		//δ����Ⱦ�Ľڵ���δ�����ʹ��������������У��ýڵ����ڵ���ͨ�����Ľڵ�����Ϊ1
		q.push_back(idx);
		is_visited[idx] = true;
		nodes_in_component = 1;

		//�Ըýڵ����bfs�������ҵ������ڵ���ͨ����
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

		/* �ҵ�һ���µ���ͨ��ͼ��������ͨ�����нڵ���Ŀ������ */
		v.push_back(nodes_in_component);
	}

	//���ҵ���������ͨ�����Ľڵ���Ŀ�Ӵ�С��������
	std::sort(v.begin(), v.end(), asc_comp);
	delete[] is_visited;
	is_visited = NULL;

	//���������ͨ�����ڵ���
	return v.at(0);
}

/* ����ʧЧ�ڵ�ı��� */
double get_failed_node_fraction(network & net)
{
	int failed_node_num = 0, count = net.number_of_nodes;
	for(int i = 0; i < count; i++)
	{
		if(net.nodes[i].status == infected) failed_node_num++;
	}

	return failed_node_num / (double)count;
}