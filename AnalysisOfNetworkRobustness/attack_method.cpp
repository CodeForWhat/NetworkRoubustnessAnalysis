#include "attack_method.h"
#include "network_def.h"
#include <algorithm>
#include "utility.h"

using std::pair;
using std::make_pair;

static vector<pair<unsigned short int, double>> btw;
static vector<pair<unsigned short int, double>> cls;
static vector<pair<unsigned short int, double>> dgr;
static vector<pair<unsigned short int, double>> egn;

static bool has_collected_and_sorted = false;

/* 自定义比的较器 */
static bool pair_comp(const pair<unsigned short int, double> p1, const pair<unsigned short int, double> p2)
{
	return p1.second > p2.second;
}

void print_out_vector(const char * filename, vector<pair<unsigned short int, double>> & v)
{
	FILE * fp = fopen(filename, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "记录向量数组时，打开文件出错.\n");
		return;
	}

	vector<pair<unsigned short int, double>>::iterator it;
	for(it = v.begin(); it != v.end(); it++)
	{
		fprintf(fp, "%d %f\n", it->first, it->second);
	}
	fclose(fp);
}

static void collect_and_sort_centrality(network & net)
{
	/* 收集节点的介数中心性值 */
	btw.clear();
	cls.clear();
	dgr.clear();
	egn.clear();
	int count = net.number_of_nodes;
	for(int idx = 0; idx < count; idx++)
	{
		btw.push_back(make_pair(idx, net.nodes[idx].centralities.betweenness));
		cls.push_back(make_pair(idx, net.nodes[idx].centralities.closeness));
		dgr.push_back(make_pair(idx, net.nodes[idx].centralities.degree));
		egn.push_back(make_pair(idx, net.nodes[idx].centralities.eigenvector));
	}

	/* 对所有中心性指标进行排序 */
	std::sort(btw.begin(), btw.end(), pair_comp);
	std::sort(cls.begin(), cls.end(), pair_comp);
	std::sort(dgr.begin(), dgr.end(), pair_comp);
	std::sort(egn.begin(), egn.end(), pair_comp);
}

static void random_selector(network & net, vector<unsigned short int> & nodes, unsigned short int num_to_be_infected)
{
	std::default_random_engine generator;
	std::uniform_int_distribution<unsigned short int> distribution(0, net.number_of_nodes - 1);
	for(int i = 0; i < num_to_be_infected; i++)
	{
		int index;
		do
		{
			index = distribution(generator);
		}while(contains(nodes, index));
		nodes.push_back(index);
	}
}

static void closeness_selector(network & net, vector<unsigned short int> & nodes, unsigned short int num_to_be_infected)
{
	if(btw.size() < 0)
	{
		fprintf(stderr, "执行接近数选择器时遇到错误.\n");
		exit(1);
	}
	for(int i = 0; i < num_to_be_infected; i++)
	{
		nodes.push_back(cls.at(i).first);
	}
}

static void betwenness_selector(network & net, vector<unsigned short int> & nodes, unsigned short int num_to_be_infected)
{
	if(btw.size() < 0)
	{
		fprintf(stderr, "执行介数选择器时遇到错误.\n");
		exit(1);
	}
	for(int i = 0; i < num_to_be_infected; i++)
	{
		nodes.push_back(btw.at(i).first);
	}
}

static void degree_selector(network & net, vector<unsigned short int> & nodes, unsigned short int num_to_be_infected)
{
	if(btw.size() < 0)
	{
		fprintf(stderr, "执行度数选择器时遇到错误.\n");
		exit(1);
	}
	for(int i = 0; i < num_to_be_infected; i++)
	{
		nodes.push_back(dgr.at(i).first);
	}
}


static void eigenvector_selector(network & net, vector<unsigned short int> & nodes, unsigned short int num_to_be_infected)
{
	if(btw.size() < 0)
	{
		fprintf(stderr, "执行向量中心性选择器时遇到错误.\n");
		exit(1);
	}
	for(int i = 0; i < num_to_be_infected; i++)
	{
		nodes.push_back(egn.at(i).first);
	}
}

/* 获取要攻击的节点的列表 */
void get_infecting_node_index(network & net, attack_type type, vector<unsigned short int> & nodes, unsigned short int num_to_be_infected)
{
	/* 是否收集过节点中心性指标 */
	if(!has_collected_and_sorted)
	{
		collect_and_sort_centrality(net);
		has_collected_and_sorted = true;
	}

	switch (type)
	{
	case random:
		random_selector(net, nodes, num_to_be_infected);
		break;
	case degree:
		degree_selector(net, nodes, num_to_be_infected);
		break;
	case closeness:
		closeness_selector(net, nodes, num_to_be_infected);
		break;
	case betweenness:
		betwenness_selector(net, nodes, num_to_be_infected);
		break;
	case eigenvector:
		eigenvector_selector(net, nodes, num_to_be_infected);
		break;
	default:
		break;
	}

}