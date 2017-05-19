#ifndef _ROBUSTNESS_METRICS_H_
#define _ROBUSTNESS_METRICS_H_

#include "network_def.h"


double get_apl(network & net);
double get_giant_component_count(network & net);
double get_diameter(network & net);
double get_failed_node_fraction(network & net);
#endif
