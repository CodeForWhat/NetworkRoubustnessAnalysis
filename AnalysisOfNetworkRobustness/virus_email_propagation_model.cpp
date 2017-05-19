#include "virus_email_propagation_model.h"
#include "algorithms_implementation.h"
#include "attack_method.h"
#include "robustness_metrics.h"
#include "utility.h"
#include "centralities.h"
#include <ctime>


/* 记录鲁棒性值的数组 */
double apls[ATTACK_TYPES_COUNT][(int)(1 / INCREMENT) + 1];
double diameter[ATTACK_TYPES_COUNT][(int)(1 / INCREMENT) + 1];
double component_node_count[ATTACK_TYPES_COUNT][(int)(1 / INCREMENT) + 1];
double failed_node_fraction[ATTACK_TYPES_COUNT][(int)(1 / INCREMENT) + 1];
double infected_nodes_count[ATTACK_TYPES_COUNT][(int)(1 / INCREMENT) + 1];

char * attack_type_name[] = 
{
	"random.csv",
	"degree.csv",
	"closeness.csv",
	"betweenness.csv",
	"eigenvector.csv"
};



/* 初始化统计数组 */
void init_statistic_arrays()
{
	for(int i = 0; i < ATTACK_TYPES_COUNT; i++)
	{
		for(int j = 0; j < int(1 / INCREMENT); j++)
		{
			apls[i][j]                 = 0;
			diameter[i][j]             = 0;
			component_node_count[i][j] = 0;
			failed_node_fraction[i][j] = 0;
		}
	}
}

/* 初始化节点的与传播模型相关的属性 */
void init_node_data(network & net, nomal_distribution_arg & click_prob, nomal_distribution_arg & check_email_inter)
{
	unsigned short int count = net.number_of_nodes;
	double temp;

	for(int idx = 0; idx < count; idx++)
	{
		net.nodes[idx].status = healthy;
		temp = normal_generator(check_email_inter); 
		if ( temp < 1) temp = 1.0;
		net.nodes[idx].check_rate = 1 / temp;
		net.nodes[idx].click_prob = normal_generator (click_prob);
		if ( net.nodes[idx].click_prob >= 1 ) net.nodes[idx].click_prob = 0.9999;
		net.nodes[idx].check_email_inter = check_time_generator(1 / temp); 
		net.nodes[idx].next_virus_email_num = 0;
		net.nodes[idx].virus_email_num      = 0;
	}
}

/* 将病毒邮件发送给已感染节点的邻居节点 */
void infect_node_and_spread_to_neighbors(network & net, unsigned int infected_node_index)
{
	int count = net.number_of_nodes;
	//set<unsigned short int> neighbors_indexes = net.nodes[infected_node_index].neighbor_indexes;
	net.nodes[infected_node_index].status          = infected;

	/* 感染节点的后继节点 */
	for(int idx = 0; idx < count; idx++)
	{
		if(!net.adjacency_matrix[infected_node_index][idx] || net.nodes[idx].status == infected) continue;
		net.nodes[idx].status = danger;
		net.nodes[idx].virus_email_num++;
		net.adjacency_matrix[infected_node_index][idx] = false;
		net.adjacency_matrix[idx][infected_node_index] = false;
		net.distance[infected_node_index][idx] = USHRT_MAX;
		net.distance[idx][infected_node_index] = USHRT_MAX;
	}
	/*
	set<unsigned short int>::iterator it;
	for(it = neighbors_indexes.begin(); it != neighbors_indexes.end(); it++)
	{
		if(net.nodes[*it].status == infected) continue;
		
		net.nodes[*it].status = danger;
		net.nodes[*it].virus_email_num++;
		net.adjacency_matrix[infected_node_index][*it] = false;
		net.adjacency_matrix[*it][infected_node_index] = false;
		net.distance[infected_node_index][*it] = USHRT_MAX;
		net.distance[*it][infected_node_index] = USHRT_MAX;
	}*/
}






/* 病毒邮件传播模型 */
double * simulate(network & net, attack_type type, unsigned short int num_to_be_infected)
{
	network copy;
	vector<unsigned short int> objects_infected;// 初始状态下，将要被感染节点的列表
	double * metrics = NULL;
	//metrics = new double[5];//
	int total_infected_node_num = 0;

	//for(int i = 0; i < 5; i++) metrics[i] = 0;

	for(int times = 0; times < RUN_TIMES; times++)
	{
		printf("\t\tSimulation time %d\n", times);
		total_infected_node_num = num_to_be_infected;
		unsigned short int count = net.number_of_nodes;

		//获取原网络的一个拷贝
		make_net_copy(copy, net);

		//获取要被感染的节点的列表
		objects_infected.clear();
		get_infecting_node_index(copy, type, objects_infected, num_to_be_infected);
		
		// 初始化，感染节点
		for(int i = 0; i < objects_infected.size(); i++)
		{
			infect_node_and_spread_to_neighbors(copy, objects_infected.at(i));
		}
		
		//每一次传播TOTAL_TICKS个tick
		for(int ticks = 0; ticks < TOTAL_TICKS; ticks++)
		{
			for(int idx = 0; idx < count; idx++)
			{
				copy.nodes[idx].check_email_inter--;

				if(copy.nodes[idx].check_email_inter > 0 || 
				   copy.nodes[idx].status == infected) continue;

				if(copy.nodes[idx].status == danger)
				{
					double prob = multi_click_prob(copy.nodes[idx].click_prob, copy.nodes[idx].virus_email_num);
					copy.nodes[idx].virus_email_num   = 0;

					if(rand01() < prob || ticks == 1)
					{
						total_infected_node_num++;
						infect_node_and_spread_to_neighbors(copy, idx);
					}				
				}
				copy.nodes[idx].check_email_inter = check_time_generator(copy.nodes[idx].check_rate);
			}
		}
		
		output_adjacency_matrix(copy.adjacency_matrix, net.number_of_nodes, num_to_be_infected/(double)count, attack_type_name[type]); 
		output_node_status(copy, attack_type_name[type]);
		//shortest_path_bfs(copy);
		//floyd(copy);
		//metrics[0] += get_apl(copy);
		//metrics[1] += get_diameter(copy);
		//metrics[2] += get_giant_component_count(copy);
		//metrics[3] += get_failed_node_fraction(copy);
		//metrics[4] += total_infected_node_num;

		//销毁网络释放其占用的内存空间
		destroy_network(copy);
	}
	
	//鲁棒性指标均值
	/*metrics[0] /= RUN_TIMES;
	metrics[1] /= RUN_TIMES;
	metrics[2] /= RUN_TIMES;
	metrics[3] /= RUN_TIMES;
	metrics[4] /= RUN_TIMES;*/

	return metrics;
}

/* 在不同攻击方式和初始感染节点的比例下运行传播模型 */
void run_simulation(network & net)
{
	int count = net.number_of_nodes;
	attack_type types[] = {random, degree, closeness, betweenness, eigenvector};
	
	//设置正太分布参数
	nomal_distribution_arg click_prob, check_email_inter;
	click_prob.mu = 0.5,       click_prob.sigma = 0.3;
	check_email_inter.mu = 40, check_email_inter.sigma = 20;

	//初始化相关数组
	printf("Initiating data and compute centralities\n");
	init_statistic_arrays();
	init_node_data(net, click_prob, check_email_inter);
	compute_centralities(net);
	printf("Centrality computation has been finished\n");
	
	//针对每一种攻击方式，在去除不同比列节点数之后，进行传播模型模拟
	for(int tp_idx = 0; tp_idx < ATTACK_TYPES_COUNT; tp_idx++)
	{
		printf("Attack method index: %d\n", tp_idx);
		
		/*
		for(int i = 0; i <= int(1 / INCREMENT); i++)
		{
			printf("\tNumber of nodes to be attacked: %d\n", (unsigned short int)(i * INCREMENT * count));

			double * metrics = simulate(net, types[tp_idx], (unsigned short int)(i * INCREMENT * count));
			apls[tp_idx][i]                 = metrics[0];
			diameter[tp_idx][i]             = metrics[1];
			component_node_count[tp_idx][i] = metrics[2];
			failed_node_fraction[tp_idx][i] = metrics[3];
			infected_nodes_count[tp_idx][i] = metrics[4];
			delete[] metrics;

			//显示计算进度
			printf("%d\n", tp_idx * ((int)(1 / INCREMENT) + 1) + i);
		}*/
		
		
		simulate(net, types[tp_idx], (unsigned short int)(1.0 * count));
	}

	//保存数据到文件
	//save_statistic_arrays_to_file("apl.txt", apls);
	//save_statistic_arrays_to_file("diameter.txt", diameter);
	//save_statistic_arrays_to_file("component_node_count.txt", component_node_count);
	//save_statistic_arrays_to_file("failed_node_fraction.txt", failed_node_fraction);
	//save_statistic_arrays_to_file("infected_nodes_count.txt", infected_nodes_count);
}



/* 测试 */
void simulate_test(network & net)
{
	FILE * fp = NULL;
	network copy;
	vector<unsigned short int> objects_infected;

	fp = fopen("apl.txt", "w");
	

	//获取要被感染的节点的列表
	for(int i = 0; i < net.number_of_nodes; i++)
	{
		make_net_copy(copy, net);
		objects_infected.clear();
		get_infecting_node_index(copy, random, objects_infected, i);
		// 初始化，感染节点
		for(int i = 0; i < objects_infected.size(); i++)
		{
			infect_node_and_spread_to_neighbors(copy, objects_infected.at(i));
		}
		shortest_path_bfs(copy);
		fprintf(fp, "%f %f\n", i / (double)net.number_of_nodes, get_apl(copy));
		destroy_network(copy);
	}
	fclose(fp);
}
/* 测试 */