#ifndef _BASE_TYPE_DEF_H_
#define _BASE_TYPE_DEF_H_

/* ��������ͷ�ļ� */
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

/* ����ڵ�״̬ */
typedef enum
{
	healthy,
	danger,
	infected
}node_status;

/* ��̫�ֲ����� */
typedef struct
{
	double mu;
	double sigma;
}nomal_distribution_arg;

/* �ڵ������� */
typedef struct
{
	double degree;
	double closeness;
	double betweenness;
	double eigenvector;
}node_centrality;

/* �ڵ����ݽṹ */
typedef struct node
{
	node_status status;
	unsigned short int id;
	unsigned short int degree_value;
	set<unsigned short int> neighbor_indexes;/* �ھӽڵ����� */
	double click_prob; /* ��������ʼ������ĸ��� */
	unsigned short int check_email_inter; /* ����ʼ���ƽ��ʱ���� */
	set<unsigned short int> predeccesors; /* �ڵ��ǰ���ڵ㼯 */
	node_centrality centralities; /* �ڵ�Ĳ�ͬ������ָ�� */
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