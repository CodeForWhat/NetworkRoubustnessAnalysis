#ifndef _VIRUS_EMAIL_PROPAGATION_MODEL_H_
#define _VIRUS_EMAIL_PROPAGATION_MODEL_H_

#define RUN_TIMES          10
#define TOTAL_TICKS        200
#define INCREMENT          0.05
#define ATTACK_TYPES_COUNT 5

#include "network_def.h"

void run_simulation(network & net);
void simulate_test(network & net);

#endif