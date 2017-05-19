#ifndef _NETWORK_DEF_H_
#define _NETWORK_DEF_H_

#include "base_type_def.h"

typedef struct
{
	node * nodes;
	bool ** adjacency_matrix;
	unsigned short int ** distance;
	unsigned short int number_of_edges;
	unsigned short int number_of_nodes;
}network;


/* ���ļ��ж�ȡ�������� */
void make_net_from_file(const char * filename, network & net);

/* �����������翽��һ���µ����� */
void make_net_copy(network & dst, network & src);

/* �������磬���ͷ���ռ�õ��ڴ� */
void destroy_network(network & net);

/* ������������нڵ�������Լ����� */
void output_centralities_of_nodes(network & net, const char * filename);

void copy_net(network & dst, const network & src);

void output_adjacency_matrix(bool ** adj, int number_of_node, double fraction, const char * filename);

void output_node_status(network & net, const char * attack_type);

#endif