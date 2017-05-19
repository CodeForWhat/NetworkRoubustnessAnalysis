#include "utility.h"

using std::pair;

bool contains(vector<unsigned short int> & v, unsigned short int elem)
{
	vector<unsigned short int>::iterator it;

	for(it = v.begin(); it != v.end(); it++)
	{
		if(*it == elem) return true;
	}
	return false;
}

void save_statistic_arrays_to_file(const char * filename, double (*data)[(int)(1 / INCREMENT) + 1])
{
	FILE * fp = fopen(filename, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "保存统计数组时，打开文件出错.\n");
		return;
	}

	fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n", "fraction", "random", "degree", "closeness", "betweenness", "eigenvector");
	for(int i = 0; i <= (int)(1 / INCREMENT); i++)
	{
		fprintf(fp, "%f\t", i * INCREMENT);
		for(int j = 0; j < ATTACK_TYPES_COUNT; j++)
		{
			fprintf(fp, "%f\t", data[j][i]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}

double multi_click_prob(double p , long n) 
{
	return 1.0 - pow( (1.0 - p) , n ) ;
}

double rand01() 
{
	return (double(rand()) + 0.5) / ((double)RAND_MAX + 1.0);
}

int exp_generator( double rate)
{ 
	return int( floor( 0.5 - log( 1 - rand01() ) / rate ) ) ; //floor(x)返回的是小于或等于x的最大整数
}

double normal_generator( nomal_distribution_arg normal)
{ 
	double temp, x ;
	static double Table[30]= {0.5000, 0.5398, 0.5793, 0.6179, 0.6554, 0.6915,
	                          0.7257, 0.7580, 0.7881, 0.8159, 0.8413, 0.8643,
       						  0.8849, 0.9032, 0.9192, 0.9332, 0.9452, 0.9554,
		       				  0.9641, 0.9713, 0.9773, 0.9822, 0.9861, 0.9893,
       						  0.9918, 0.9938, 0.9954, 0.9965, 0.9975, 1.0000 };	
	temp =rand01();
	long sign = 1, i;
	if ( temp <0.5) 
	{
		sign = -1;  //产生的是负半区的(0,1)正态分布数
		temp = 1 - temp;
	};
	for (i = 0; i < 30; i++)
	{
		if ( temp <= Table[i] )
		{ 
			x = double( i ) / 10.0; 
			break;
		}
	}
	//如果是负半区的(0,1)正态分布数，找其对应的正半区数
	if ( sign == -1 ) 
		x = 0 - x;
	//此时的x是0-1之间的符合(0,1)正态分布的随机数
	// variance is the standard deviation.
    // variance 是方差
	temp = normal.mu + x * normal.sigma; 
	// experiment require that normal distribution number to be non-negative.
	if ( temp <=0.00001) 
		temp = 0.01;

	return temp;
}

int check_time_generator(double check_rate)
{
	int temp = exp_generator(check_rate);
	if(temp < 1) temp = 1;
	return temp;
}