#include "centralities.h"
#include "algorithms_implementation.h"

#define MAX_ITER 100;

using std::priority_queue;

static bool has_compute_distance = false;
static bool did_centralities_computed_once = false;


void ug_degree_centrality(network & net)
{
	for(int idx = 0; idx < net.number_of_nodes; idx++)
	{
		net.nodes[idx].centralities.degree = net.nodes[idx].degree_value / (double)(net.number_of_nodes - 1);
	}
}

void ug_betweenness_centrality(network & net)
{
	int count = net.number_of_nodes;
	priority_queue<node> * S = NULL;

	/* 介数计算 */
	for(int idx = 0; idx < count; idx++)
	{
		//只适用无权图
		S = bfs(idx, net);

		/* brands算法核心: dependency accumulation phase */
		while(!S->empty())
		{
			unsigned short int w = S->top().id;
			S->pop();

			set<unsigned short int>::iterator it;
			for(it = net.nodes[w].predeccesors.begin(); it != net.nodes[w].predeccesors.end(); it++)
			{
				unsigned short int v = *it;
				net.nodes[v].delta += ((net.nodes[v].sigma / net.nodes[w].sigma) * (1.0 + net.nodes[w].delta));
			}
			if(w != idx)
			{
				net.nodes[w].centralities.betweenness += net.nodes[w].delta;
			}
		}
		delete S;
		S = NULL;
	}
	/* 归一化处理 */
	double total_path_pairs = (count - 1) * (count - 2);
	for(int idx = 0; idx < count; idx++)
	{
		double bc = net.nodes[idx].centralities.betweenness;
		net.nodes[idx].centralities.betweenness = bc / total_path_pairs;
	}
	has_compute_distance = true;
}

void ug_closeness_centrality(network & net)
{
	/* 距离矩阵未提前计算，无法进行后续的中心性计算 */
	if(!has_compute_distance)
	{
		return;
	}
	double sum = 0;
	for(int idx = 0; idx < net.number_of_nodes; idx++)
	{
		sum = 0;
		for(int j = 0; j < net.number_of_nodes; j++)
		{
			if(net.distance[idx][j] < USHRT_MAX)
			{
				sum += net.distance[idx][j];
			}
		}
		net.nodes[idx].centralities.closeness = (net.number_of_nodes - 1) / sum;
	}
}

void ug_eigenvector_centrality(network & net)
{
	int count = net.number_of_nodes;
	double * x1 = new double[count];
	double * x2 = new double[count];
	double * t, s;
	
	int iter = MAX_ITER;

	/* 初始化辅助变量及数组 */
	for(int i = 0; i < count; i++)
	{
		x2[i] = 1.0 / count;
	}

	while(iter-- > 0)
	{
		//交换x1和x2
		t = x1;
		x1 = x2;
		x2 = t;

		/* 重新初始化x2 */
		for(int i = 0; i < count; i++)
		{
			x2[i] = 0;
		} 
		s = 0;

		for(int idx = 0; idx < count; idx++)
		{
			set<unsigned short int> neighbors = net.nodes[idx].neighbor_indexes;
			set<unsigned short int>::iterator it;
			for(it = neighbors.begin(); it != neighbors.end(); it++)
			{
				x2[idx] += x1[*it];
			}

			s += x2[idx] * x2[idx];
		}

		s = (s == 0 ? 1.0 : 1.0 / sqrt(s));

		for(int idx = 0; idx < count; idx++)
		{
			x2[idx] *= s;
		}
	}

	/* 归一化处理 */
	double max = x2[0];
	double min = max;

	for (int idx = 1; idx < count; idx++) 
	{
		max = max < x2[idx] ? x2[idx] : max;
		min = min > x2[idx] ? x2[idx] : min;
	}

	for(int idx = 0; idx < count; idx++)
	{
		net.nodes[idx].centralities.eigenvector = (x2[idx] - min) / (max - min);
	}

	delete[] x1;
	delete[] x2;
}

void compute_centralities(network & net)
{
	if(did_centralities_computed_once) return;
	printf("To compute degree centrality\n");
	ug_degree_centrality(net);
	printf("To compute betweenness centrality\n");
	ug_betweenness_centrality(net);
	printf("To compute closeness centrality\n");
	ug_closeness_centrality(net);
	printf("To compute eigenvector centrality\n");
	ug_eigenvector_centrality(net);
	did_centralities_computed_once = true;
}
