#ifndef _ATTACK_METHOD_H_
#define _ATTACK_METHOD_H_

#include "network_def.h"

typedef enum
{
	random,
	degree,
	closeness,
	betweenness,
	eigenvector
}attack_type;

/* ѡ���ʼ��Ⱦ�Ľڵ��б� */
void get_infecting_node_index(network & net, attack_type type, vector<unsigned short int> & nodes, unsigned short int num_to_be_infected);


#endif