#include "base_type_def.h"

void copy_node(node & dst, const node & src)
{
	dst.status = src.status;

	dst.click_prob = src.click_prob;

	dst.degree_value = src.degree_value;

	dst.check_email_inter = src.check_email_inter;
	
	dst.centralities.degree = src.centralities.degree;
	dst.centralities.betweenness = src.centralities.betweenness;
	dst.centralities.closeness = src.centralities.closeness;
	dst.centralities.eigenvector = src.centralities.eigenvector;

	dst.delta    = src.delta;
	dst.sigma    = src.sigma;
	dst.distance = src.distance;
	dst.id       = src.id;

	dst.virus_email_num = src.virus_email_num;
	dst.next_virus_email_num = src.next_virus_email_num;
	dst.check_rate = src.check_rate;

	dst.neighbor_indexes.clear();
	dst.predeccesors.clear();

	set<unsigned short int>::iterator it;
	for(it = src.neighbor_indexes.begin(); it != src.neighbor_indexes.end(); it++)
	{
		dst.neighbor_indexes.insert(*it);
	}
	for(it = src.predeccesors.begin(); it != src.predeccesors.end(); it++)
	{
		dst.predeccesors.insert(*it);
	}
}