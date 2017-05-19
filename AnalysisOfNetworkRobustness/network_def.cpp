#include "network_def.h"
#include "utility.h"

#define LINE_BUF_SIZE 2000


void make_net_from_file(const char * filename, network & net)
{
	FILE * input = NULL;
	long num_of_nodes = 0, num_of_edges = 0;
	char line_buffer[LINE_BUF_SIZE];

	const char edge_start_mark[] = " Edges: (";
	const char node_start_mark[] = " Nodes: (";

	if((input = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "文件打开出错.\n");
		exit(1);
	}

	do
	{
		fgets(line_buffer, LINE_BUF_SIZE, input);
	}while(strstr(line_buffer, node_start_mark) == NULL);

	/* 读取并存储节点数据 */
	sscanf(line_buffer, " Nodes: (%d)", &num_of_nodes);

	if(num_of_nodes < 0)
	{
		fprintf(stderr, "节点数为负.\n");
		exit(1);
	}

	net.nodes = new node[num_of_nodes];
	net.distance         = new unsigned short int * [num_of_nodes];
	net.adjacency_matrix = new bool * [num_of_nodes];
	for(int i = 0; i < num_of_nodes; i++)
	{
		net.distance[i]         = new unsigned short int[num_of_nodes];
		net.adjacency_matrix[i] = new bool[num_of_nodes];
		for(int j = 0; j < num_of_nodes; j++)
		{
			net.distance[i][j] = USHRT_MAX;
			net.adjacency_matrix[i][j] = false;
		}
		net.distance[i][i]         = 0;
		net.adjacency_matrix[i][i] = true;
	}

	int node_index, node_degree;
	for(int i = 1; i <= num_of_nodes; i++)
	{
		fgets(line_buffer, LINE_BUF_SIZE, input);
		sscanf(line_buffer, "%d %d", &node_index, &node_degree);
		if(node_index != i)
		{
			fprintf(stderr, "读取网络节点时遇到错误.\n");
			exit(1);
		}
		net.nodes[i - 1].degree_value    = node_degree;
		net.nodes[i - 1].status          = healthy;
		net.nodes[i - 1].centralities.betweenness = 0;
		net.nodes[i - 1].centralities.closeness   = 0;
		net.nodes[i - 1].centralities.degree      = 0;
		net.nodes[i - 1].centralities.eigenvector = 0;
		net.nodes[i - 1].delta    = 0.0;
		net.nodes[i - 1].sigma    = 0.0;
		net.nodes[i - 1].distance = SHRT_MAX;
		net.nodes[i - 1].id       = i - 1;
	}

	/* 读取边的信息 */
	do
	{
		fgets(line_buffer, LINE_BUF_SIZE, input);
	}while(strstr(line_buffer, edge_start_mark) == NULL);

	sscanf(line_buffer, " Edges: (%d)", &num_of_edges);

	int edge_index, start, end;
	for(int i = 1; i <= num_of_edges; i++)
	{
		fgets(line_buffer, LINE_BUF_SIZE, input);
		sscanf(line_buffer, "%d %d %d", &edge_index, &start, &end);
		//if (edge_index != i) 
		//{
		//	fprintf(stderr, "读取网络中边信息时出错.\n"); 
		//	exit(1);
		//}
		net.nodes[start - 1].neighbor_indexes.insert(end - 1);
		net.nodes[end - 1].neighbor_indexes.insert(start - 1);
		net.adjacency_matrix[start - 1][end - 1] = true;
		net.adjacency_matrix[end - 1][start - 1] = true;
	}
	fclose(input);
	net.number_of_edges = num_of_edges;
	net.number_of_nodes = num_of_nodes;
}


void make_net_copy(network & dst, network & src)
{
	dst.number_of_nodes = src.number_of_nodes;
	dst.number_of_edges = src.number_of_edges;
	dst.nodes            = new node[dst.number_of_nodes];
	dst.distance         = new unsigned short int * [dst.number_of_nodes];
	dst.adjacency_matrix = new bool * [dst.number_of_nodes];
	for(int i = 0; i < dst.number_of_nodes; i++)
	{
		dst.distance[i]         = new unsigned short int[dst.number_of_nodes];
		dst.adjacency_matrix[i] = new bool[dst.number_of_nodes];
		for(int j = 0; j < dst.number_of_nodes; j++) dst.distance[i][j] = USHRT_MAX;
		dst.distance[i][i] = 0;
		//memcpy(dst.distance[i], src.distance[i], dst.number_of_nodes * sizeof(unsigned short int));
		memcpy(dst.adjacency_matrix[i], src.adjacency_matrix[i], dst.number_of_nodes * sizeof(bool));
		copy_node(dst.nodes[i], src.nodes[i]);
	}
}

void copy_net(network & dst, const network & src)
{
	for(int i = 0; i < dst.number_of_nodes; i++)
	{
		memcpy(dst.distance[i], src.distance[i], dst.number_of_nodes * sizeof(int));
		memcpy(dst.adjacency_matrix[i], src.adjacency_matrix[i], dst.number_of_nodes * sizeof(int));
		copy_node(dst.nodes[i], src.nodes[i]);
	}
	dst.number_of_nodes = src.number_of_nodes;
	dst.number_of_edges = src.number_of_edges;
}

void destroy_network(network & net)
{
	for(int i = 0; i < net.number_of_nodes; i++)
	{
		delete[] net.distance[i];
		delete[] net.adjacency_matrix[i];
	}
	delete[] net.nodes;
	delete[] net.distance;
	delete[] net.adjacency_matrix;

	net.number_of_nodes = -1;
	net.number_of_edges = -1;
	net.distance         = NULL;
	net.adjacency_matrix = NULL;
	net.nodes            = NULL;
}

void output_centralities_of_nodes(network & net, const char * filename)
{
	FILE * output = NULL;

	if((output = fopen(filename, "w")) == NULL)
	{
		fprintf(stderr, "打开文件出错.\n");
		return;
	}

	fprintf(output, "%-15s%-15s%-15s%-15s%-15s\n", "Node index", "Degree", "Closeness", "Betweenness", "Eigenvector");
	for(int idx = 0; idx < net.number_of_nodes; idx++)
	{
		fprintf(output, "%d\t%f\t%f\t%f\t%f\n", idx + 1,
			net.nodes[idx].centralities.degree,
			net.nodes[idx].centralities.closeness,
			net.nodes[idx].centralities.betweenness,
			net.nodes[idx].centralities.eigenvector);
	}
	fclose(output);
}

void output_node_status(network & net, const char * attack_type)
{
	char final_name[256];
	FILE * fptr = NULL;

	sprintf_s(final_name, "node_status_%s", attack_type);
	fopen_s(&fptr, final_name, "w");
	if(fptr == NULL){ fprintf_s(stderr, "Can't open file normally.\n"); exit(1); }

	for(int i = 0; i < net.number_of_nodes; i++)
	{
		fprintf_s(fptr, "%d %d\n", i+1, net.nodes[i].status == infected ? 1 : 0);
	}

	fclose(fptr);
}

void output_adjacency_matrix(bool ** adj, int number_of_node, double fraction, const char * filename)
{
	char final_filename[256];

	sprintf(final_filename, "%.2f-%s", fraction, filename);
	FILE * fp = fopen(final_filename, "w");
	if(fp == NULL) return;

	fprintf(fp, ";");
	for(int i = 0; i < number_of_node - 1; i++)
	{
		fprintf(fp, "%d;", i + 1);
	}
	fprintf(fp, "%d\n", number_of_node);

	for(int i = 0; i < number_of_node; i++)
	{
		fprintf(fp, "%d;", i + 1);
		for(int j = 0; j < number_of_node - 1; j++)
		{
			if(i == j) 
			{
				fprintf(fp, "%d;", 0);
			}
			else
			{
				fprintf(fp, "%d;", adj[i][j] == true ? 1 : 0);
			}
		}
		if(i == number_of_node - 1)
		{
			fprintf(fp, "%d\n", 0);
		}
		else
		{
			fprintf(fp, "%d\n", adj[i][number_of_node - 1] ? 1 : 0);
		}
	}

	fclose(fp);
}