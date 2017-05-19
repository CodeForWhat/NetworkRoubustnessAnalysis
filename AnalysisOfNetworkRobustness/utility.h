#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "base_type_def.h"
#include "virus_email_propagation_model.h"

using std::vector;

bool contains(vector<unsigned short int> & v, unsigned short int elem);
void save_statistic_arrays_to_file(const char * filename, double (*data)[(int)(1 / INCREMENT) + 1]);
double multi_click_prob(double p, long n);
double rand01();
int exp_generator(double rate);
double normal_generator(nomal_distribution_arg normal);
int check_time_generator(double check_rate);

#endif