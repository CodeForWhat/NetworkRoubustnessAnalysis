#ifndef _CENTRALITIES_H_
#define _CENTRALITIES_H_

#include "network_def.h"

using std::priority_queue;
/* ugΪUnweighted Graph*/
void ug_degree_centrality(network & network);
void ug_closeness_betweenness_centrality(network & network);
void ug_eigenvector_centrality(network & network);
void compute_centralities(network & net);

void print_out_queue(priority_queue<node> * q);

#endif