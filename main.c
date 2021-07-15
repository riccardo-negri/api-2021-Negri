// My solution for Algoritmi e Principi dell'Informatica 2020-2021's project

#include <stdio.h>
#include <stdlib.h>

#define INFINITY 4294967295

typedef struct node {
   unsigned int id;
   unsigned int dist;	// I use this variable with two different meaning depending on the context
   struct node * next;	// I add to this only the nodes reachable by the current node with their distance
} node_type;

typedef struct graph {
   unsigned int index;
   unsigned int value;
} graph_type;

unsigned int ReadGraph(unsigned int d);
unsigned int DeleteMin(unsigned int * F_minheap, unsigned int start_minheap);
void RebuildMinHeap (node_type * F_main, unsigned int * F_minheap, unsigned int heapsize, unsigned int i);
void MinHeapify (node_type * F_main, unsigned int * F_minheap, unsigned int array_dim, unsigned int i, unsigned int start_minheap);

void AddGraph(unsigned int value, unsigned int index, graph_type * ranking, unsigned int ranking_length, unsigned int k);
void RebuildMaxHeap (graph_type * ranking, unsigned int array_dim);
void MaxHeapify (graph_type * ranking, unsigned int array_dim, unsigned int i);

void BestGraphs(graph_type * ranking, unsigned int array_dim);

int main(int argc, char const *argv[])
{
	unsigned int d = 0; // number of nodes
	unsigned int temp = getchar_unlocked();

	unsigned int k = 0; // length of top graphs

	unsigned int index = 0;
	unsigned int paths_sum;

	unsigned int ranking_length = 0;

	while(temp != ' ') {
		d = d*10 + temp-'0';
		temp = getchar_unlocked();	
	}
	
	temp = getchar_unlocked();
	while(temp != '\n') {
		k = k*10 + temp-'0';
		temp = getchar_unlocked();	
	}

	//long long int blank=0;
	graph_type ranking[k]; //max heap to keep the best graphs
	//long long int blank2=0;

	//printf("STARTED %lld %lld\n", blank, blank2);
	//printf("%d %d \n", d, k);

	while(temp != EOF) {

		if (temp == 'A') {
			paths_sum = ReadGraph(d);	// returns the sum of all the minimum paths from zero
			
			AddGraph(paths_sum, index, ranking, ranking_length, k);
			if (ranking_length < k) {
				ranking_length++;
			}
			index ++;
		}

		else if (temp == 'T') {
			BestGraphs(ranking, ranking_length);
		}

		temp = getchar_unlocked();
	}	

	//printf("FINISHED\n");

	return 0;
}

unsigned int ReadGraph(unsigned int d) {

	unsigned int temp = getchar_unlocked();	// get at the start of the matrix
	node_type * pt;
	node_type * temp_pt;
	unsigned int j;
	unsigned int num;
	unsigned int minpaths_sum = 0;
	unsigned int nodes_left = d;
	unsigned int min_id;
	unsigned int start_minheap = 0;
	
	node_type * F_main = malloc(sizeof(node_type)*d); // F_main is set of nodes that are yet to achieve final distance estimates
	unsigned int F_minheap[d]; // this is gonna be tha actual MIN-HEAP
	
	while (temp != '\n') {
		temp = getchar_unlocked();	
	}

	for (unsigned int i = 0; i < d; i++){	// read the graph
		F_main[i].id = i;
		F_minheap[i] = i;
		F_main[i].dist = INFINITY; // set initial distance estimate
		node_type * last_added = &F_main[i];
		for (j = 0; j< d; j++) {
			temp = getchar_unlocked();
			num = 0;
			while (temp != ',' && temp != '\n') {
				num = 10*num + temp-'0';
				temp = getchar_unlocked();
			}
			if (num != 0) {
				last_added->next = malloc(sizeof(node_type));
				last_added = last_added->next;
				last_added->id = j;
				last_added->dist = num;
			}
		}
		last_added->next = NULL;
	}
	F_main[0].dist=0;

	// print F_main
	/*for (int w = 0; w < d; w++) {
		printf("id:%u dist:%u ", F_main[w].id, F_main[w].dist);
		pt = F_main[w].next;
		while (pt != NULL) {
			printf("- node:%u dist:%u ", pt->id, pt->dist);
			pt = pt->next;
		}
		printf("\n");
	}
	// print F_minheap
	printf("Minheap: ");
	for (int w = 0; w < d; w++) {
		printf("%d ", F_minheap[w]);
	}
	printf("\n");*/

	nodes_left = d;
	while (nodes_left != 0) {
		min_id = DeleteMin(F_minheap, start_minheap); // this modified delete min will actually put my min in the last place of the array so i can use it easily
		start_minheap++;
		if (F_main[min_id].dist == INFINITY) {
			//printf("Not connected graph from here");
			return minpaths_sum;
		}
		//printf("Here ");
		
		pt = F_main[min_id].next;
		while (pt != NULL) {	//WARNING A LOT TO BE OPTIMIZED
			if(F_main[pt->id].dist > F_main[min_id].dist + pt->dist) {
				F_main[pt->id].dist = F_main[min_id].dist + pt->dist;
			}
			temp_pt = pt;
			pt = pt->next;
			free(temp_pt);
		}
		
		minpaths_sum = minpaths_sum + F_main[min_id].dist;

		nodes_left--;

		if (nodes_left != 0) {
			//printf("\n\nRebuilding minheap with this value %u and index %u\n", F_main[min_id].dist, min_id);

			//printf("Minheap before rebuild: ");
			//for (int i=start_minheap; i<d; i++) {
			//	printf("%u-%u ", F_minheap[i], F_main[F_minheap[i]].dist);
			//}
			//
			RebuildMinHeap(F_main, F_minheap, start_minheap, d); // now that the distances have changes the MIN-HEAP must be updated
			//
			//printf("\nMinheap After rebuild: ");
			//for (int i=start_minheap; i<d; i++) {
			//	printf("%u-%u ", F_minheap[i], F_main[F_minheap[i]].dist);
			//}
		}
		if (nodes_left == 0 && start_minheap == d) {
			//printf("----------");
		}
		//printf("Maxheap after: \n");
		//for (int i = 0; i < array_dim; i++) {
		//	printf("%u-%u ", ranking[i].index, ranking[i].value);
		//}
/*		printf("Minheap: ");
		for (int w = start_minheap; w < d; w++) {
			printf("%d ", F_minheap[w]);
		}
		printf("\n");*/
	}

	free(F_main);

	//printf("Somma cammini: %u \n", minpaths_sum);
	//printf("--------------");
	return minpaths_sum;
}

unsigned int DeleteMin(unsigned int * F_minheap, unsigned int start_minheap) {
	return F_minheap[start_minheap];
}

void RebuildMinHeap (node_type * F_main, unsigned int * F_minheap, unsigned int start_minheap, unsigned int array_dim) {
	unsigned int heapsize = array_dim - start_minheap;
	
	for (int i = (heapsize/2)+start_minheap; i >= start_minheap; i--) {
		MinHeapify(F_main, F_minheap, array_dim, i, start_minheap);
	}
	
	return;
}

void MinHeapify (node_type * F_main, unsigned int * F_minheap, unsigned int array_dim, unsigned int i, unsigned int start_minheap) {
	unsigned int temp, min;
	unsigned int l = 2*(i-start_minheap)+start_minheap;
	unsigned int r = 2*(i-start_minheap)+1+start_minheap;
	if (l < array_dim && F_main[F_minheap[l]].dist < F_main[F_minheap[i]].dist) {
		min = l;
	}
	else {
		min = i;
	}

	if (r < array_dim && F_main[F_minheap[r]].dist < F_main[F_minheap[min]].dist){
		min = r;
	}

	if (min != i){
		temp = F_minheap[i];
		F_minheap[i] = F_minheap[min];
		F_minheap[min] = temp;
		MinHeapify(F_main, F_minheap, array_dim, min, start_minheap);
	}
	return;
}

void AddGraph(unsigned int value, unsigned int index, graph_type * ranking, unsigned int ranking_length, unsigned int k) {
	unsigned int array_dim = ranking_length;
	if (array_dim < k) {
		ranking[array_dim].index = index;
		ranking[array_dim].value = value;
		array_dim++;
		RebuildMaxHeap(ranking, array_dim);
	}
	else {
		if (value < ranking[0].value) {
			ranking[0].index = index;
			ranking[0].value = value;
			//printf("CALLIN REBUILD con dim %d ", array_dim);
			RebuildMaxHeap(ranking, array_dim);
		}
	}

	//printf("\nAdding graph with value %u and index %u\n", value, index);
	//printf("Maxheap after: \n");
	//for (int i = 0; i < array_dim; i++) {
	//	printf("%u-%u ", ranking[i].index, ranking[i].value);
	//}

	return;
}

void RebuildMaxHeap (graph_type * ranking, unsigned int array_dim) {
	int i = array_dim/2;
	//printf("I vale %d ", i);
	for (; i >= 0; i--) {
		//printf("MAXHEAPIFY");
		MaxHeapify(ranking, array_dim, i);
	}
	
	return;
}

void MaxHeapify (graph_type * ranking, unsigned int array_dim, unsigned int i) {
	unsigned int temp_ind, temp_value, max;
	unsigned int l = 2*i;
	unsigned int r = 2*i+1;
	if (l < array_dim && ranking[l].value > ranking[i].value) {
		max = l;
	}
	else {
		max = i;
	}

	if (r < array_dim && ranking[r].value > ranking[max].value){
		max = r;
	}

	if (max != i){
		//printf("SWAPPING");
		temp_ind = ranking[i].index;
		temp_value = ranking[i].value;
		ranking[i].index = ranking[max].index;
		ranking[i].value = ranking[max].value;
		ranking[max].index = temp_ind;
		ranking[max].value = temp_value;
		MaxHeapify(ranking, array_dim, max);
	}
	return;
}

void BestGraphs(graph_type * ranking, unsigned int array_dim) {
	//printf("\nBest graphs: \n");
	int start = 1;
	for (int i = 0; i < array_dim; i++) {
		if (start == 1) {
			printf("%d", ranking[i].index);
			start = 0;
		}
		else {
			printf(" %d", ranking[i].index);
		}
	}
	printf("\n");
	return;
}