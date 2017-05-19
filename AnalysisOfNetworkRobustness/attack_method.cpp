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

/* �Զ���ȵĽ��� */
static bool pair_comp(const pair<unsigned short int, double> p1, const pair<unsigned short int, double> p2)
{
	return p1.second > p2.second;
}

void print_out_vector(const char * filename, vector<pair<unsigned short int, double>> & v)
{
	FILE * fp = fopen(filename, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "��¼��������ʱ�����ļ�����.\n");
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
	/* �ռ��ڵ�Ľ���������ֵ */
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

	/* ������������ָ��������� */
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
		fprintf(stderr, "ִ�нӽ���ѡ����ʱ��������.\n");
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
		fprintf(stderr, "ִ�н���ѡ����ʱ��������.\n");
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
		fprintf(stderr, "ִ�ж���ѡ����ʱ��������.\n");
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
		fprintf(stderr, "ִ������������ѡ����ʱ��������.\n");
		exit(1);
	}
	for(int i = 0; i < num_to_be_infected; i++)
	{
		nodes.push_back(egn.at(i).first);
	}
}

/* ��ȡҪ�����Ľڵ���б� */
void get_infecting_node_index(network & net, attack_type type, vector<unsigned short int> & nodes, unsigned short int num_to_be_infected)
{
	/* �Ƿ��ռ����ڵ�������ָ�� */
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