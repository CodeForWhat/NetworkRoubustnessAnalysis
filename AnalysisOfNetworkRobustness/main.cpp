#include "base_type_def.h"
#include "network_def.h"
#include "centralities.h"
#include "robustness_metrics.h"
#include "algorithms_implementation.h"
#include "virus_email_propagation_model.h"
#include <ctime>


int main(int argc, char * argv[])
{
	network net;

	//if(argc != 2) return -1;
	//make_net_from_file(argv[1], net);
	//printf("%s\n", argv[1]);
	//"C:\\Users\\KevinYin\\Desktop\\AS-output.txt"
	//make_net_from_file("C:\\Users\\KevinYin\\Desktop\\input_power-law(n=100,e=420,k=7.28).txt", net);
	//make_net_from_file("C:\\Users\\KevinYin\\Desktop\\output-karate-final.txt", net);
	//make_net_from_file("C:\\Users\\KevinYin\\Desktop\\network\\Output-email.txt", net);
	//make_net_from_file("C:\\Users\\KevinYin\\Desktop\\AS-output.txt", net);
	//make_net_from_file("C:\\Users\\KevinYin\\Desktop\\Output-AS.txt", net);
	//make_net_from_file("C:\\Users\\KevinYin\\Desktop\\China_model0.4.txt", net);
	//make_net_from_file("C:\\Users\\KevinYin\\Desktop\\networks3\\output_facebook_combined.txt", net);
	make_net_from_file("C:\\Users\\KevinYin\\Desktop\\network data\\output Enron network.txt", net);
	//make_net_from_file("C:\\Users\\KevinYin\\Desktop\\networks3\\output_CA-AstroPh.txt", net);

	printf("%d nodes and %d edges have been read\n", net.number_of_nodes, net.number_of_edges);
	
	time_t begin, end;
	
	time(&begin);

	run_simulation(net);
	//shortest_path_bfs(net);
	//("%.3f\n", get_apl(net));
	//printf("%.3f\n", get_diameter(net));

	printf_s("The number of nodes in giant component: %.3f.\n", get_giant_component_count(net));

	time(&end);



	printf("Consumed %d sec", end - begin); 

	destroy_network(net);

	return 0;
}


/*
int main()
{
	network net;
	//const char * filename = "C:\\Users\\KevinYin\\Desktop\\output-karate-final.txt";
	//const char * filename = "C:\\Users\\KevinYin\\Desktop\\output-footballnet-final.txt";
	const char * filename = "C:\\Users\\KevinYin\\Desktop\\Output-email.txt";
	
	make_net_from_file(filename, net);

	simulate_test(net);

	destroy_network(net);
	return 0;
}*/