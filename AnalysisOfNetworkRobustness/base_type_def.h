#ifndef _BASE_TYPE_DEF_H_
#define _BASE_TYPE_DEF_H_

/* 包含基本头文件 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <list>
#include <set>
#include <limits>
#include <random>
#include <iostream>

using std::vector;
using std::set;

/* 定义节点状态 */
typedef enum
{
	healthy,
	danger,
	infected
}node_status;

/* 正太分布参数 */
typedef struct
{
	double mu;
	double sigma;
}nomal_distribution_arg;

/* 节点中心性 */
typedef struct
{
	double degree;
	double closeness;
	double betweenness;
	double eigenvector;
}node_centrality;

/* 节点数据结构 */
typedef struct node
{
	node_status status;
	unsigned short int id;
	unsigned short int degree_value;
	set<unsigned short int> neighbor_indexes;/* 邻居节点的序号 */
	double click_prob; /* 点击病毒邮件附件的概率 */
	unsigned short int check_email_inter; /* 检查邮件的平均时间间隔 */
	set<unsigned short int> predeccesors; /* 节点的前驱节点集 */
	node_centrality centralities; /* 节点的不同中心性指标 */
	short int next_virus_email_num;
	short int virus_email_num;
	double check_rate;
	double sigma, delta;
	int distance;
	friend bool operator< (const node & a, const node & b)
	{
		return a.distance < b.distance;
	}
}node;

void copy_node(node & dst, const node & src);

#endif