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


/* 从文件中读取网络数据 */
void make_net_from_file(const char * filename, network & net);

/* 根据现有网络拷贝一个新的网络 */
void make_net_copy(network & dst, network & src);

/* 销毁网络，并释放其占用的内存 */
void destroy_network(network & net);

/* 输出网络中所有节点的中心性计算结果 */
void output_centralities_of_nodes(network & net, const char * filename);

void copy_net(network & dst, const network & src);

void output_adjacency_matrix(bool ** adj, int number_of_node, double fraction, const char * filename);

void output_node_status(network & net, const char * attack_type);

#endif