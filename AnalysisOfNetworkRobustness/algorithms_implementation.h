#ifndef _ALGORITHMS_IMPLEMENTATION_H_
#define _ALGORITHMS_IMPLEMENTATION_H_

#include "network_def.h"

using std::priority_queue;

priority_queue<node> * bfs(unsigned short int source, network & net);
void shortest_path_bfs(network & net);
void floyd(network & net);

#endif