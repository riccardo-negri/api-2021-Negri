// Riccardo Negri's solution for Algoritmi e Principi dell'Informatica 2020-2021's Project
#include <stdio.h>
#define INFINITY 4294967295

typedef struct node {
   unsigned int id;
   unsigned int dist;
} node_type;

typedef struct graph {
   unsigned int index;
   unsigned long int value;
   struct graph * next;
   struct graph * prev;
} graph_type;

void MinHeapify(node_type * min_heap, unsigned int d, unsigned int heap_start, unsigned int i) {
	unsigned int l, r, min, id, dist;
	l = 2*(i-heap_start)+heap_start;
	r = l+1;
	if (l < d && min_heap[l].dist < min_heap[i].dist) min = l;
	else min = i;
	if (r < d && min_heap[r].dist < min_heap[min].dist) min = r;
	if (min != i) {
		id = min_heap[i].id;
		dist = min_heap[i].dist;
		min_heap[i].id = min_heap[min].id;
		min_heap[i].dist = min_heap[min].dist;
		min_heap[min].id = id;
		min_heap[min].dist = dist;
		MinHeapify(min_heap, d, heap_start, min);
	}
}

int main() {
	char chr;

	unsigned int i, j, temp, curr_min_id, arch_weigth, heapsize, heap_start, flag, first;
	unsigned int d = 0; // number of nodes
	unsigned int k = 0; // length of top graphs ranking
	unsigned int ranking_elements = 0;
	unsigned int curr_index = 0;

	unsigned long int paths_sum, curr_min_dist, new_dist;
	
	graph_type * head_list_minimum = NULL;
	graph_type * tail_list_maximum = NULL;
	graph_type * p_temp; 
	graph_type * old_maximum;

	chr = getchar_unlocked();
	while(chr != ' ') {
		d = d*10 + chr-'0';
		chr = getchar_unlocked();	
	}
	
	chr = getchar_unlocked();
	while(chr != '\n') {
		k = k*10 + chr-'0';
		chr = getchar_unlocked();	
	}	

	unsigned int adjacency_matrix[d][d];
	node_type min_heap_dijkstra[d];
	graph_type ranking_queue[k];

	while(chr != EOF) {
		// --> AggiungiGrafo
		if (chr == 'A') {
			// --> Reading and acquisition
			while (chr != '\n') {
				chr = getchar_unlocked();	
			}
			for (i = 0; i < d; i++) {
				min_heap_dijkstra[i].id = i;
				min_heap_dijkstra[i].dist = INFINITY;
				for (j = 0; j < d; j++) {
					chr = getchar_unlocked();
					temp = 0;
					while (chr != ',' && chr != '\n') {
						temp = 10*temp + chr-'0';
						chr = getchar_unlocked();
					}
					adjacency_matrix[i][j] = temp;
				}
			}

			// --> Computation sum of paths with Dijkstra
			min_heap_dijkstra[0].dist = 0;
			paths_sum = 0;
			for (i = 0; i < d; i++) {
				curr_min_id = min_heap_dijkstra[i].id;
				curr_min_dist =  min_heap_dijkstra[i].dist;
				if(curr_min_dist < INFINITY) {
					paths_sum += curr_min_dist;
				}
				else {
					break;
				}
				for (j = i+1; j<d; j++) {
					arch_weigth = adjacency_matrix[curr_min_id][min_heap_dijkstra[j].id];
					if (arch_weigth != 0) {
						new_dist = curr_min_dist + arch_weigth;
						if (min_heap_dijkstra[j].dist > new_dist) {
							min_heap_dijkstra[j].dist = new_dist;
						}
					}
				}
				// --> Rebuild MinHeap
				heapsize = d-i-1;
				heap_start = i+1;
				for (j = ((heapsize)/2)+heap_start; j >= heap_start; j--) {
					MinHeapify(min_heap_dijkstra, d, heap_start, j);
				}
				
			}

			// --> Insertion of new graph in ranking (queue as a list with head as minimum and tail as maximum)
			if (ranking_elements < k) {	// Vacant spots available
				ranking_queue[ranking_elements].index = curr_index;
				ranking_queue[ranking_elements].value = paths_sum;
				ranking_elements++;
				if(ranking_elements == 2) {
					if(ranking_queue[0].value < ranking_queue[1].value) {
						head_list_minimum = &ranking_queue[0];
						tail_list_maximum = &ranking_queue[1];
						ranking_queue[0].next = &ranking_queue[1];
						ranking_queue[0].prev = NULL;
						ranking_queue[1].next = NULL;
						ranking_queue[1].prev = &ranking_queue[0];
					}
					else {
						head_list_minimum = &ranking_queue[1];
						tail_list_maximum = &ranking_queue[0];
						ranking_queue[1].next = &ranking_queue[0];
						ranking_queue[1].prev = NULL;
						ranking_queue[0].next = NULL;
						ranking_queue[0].prev = &ranking_queue[1];
					}
				}	
				else if (ranking_elements > 2) {
					p_temp = head_list_minimum;
					if(p_temp->value > paths_sum) {	// head
						ranking_queue[ranking_elements-1].next = head_list_minimum;
						ranking_queue[ranking_elements-1].prev = NULL;
						head_list_minimum->prev = &ranking_queue[ranking_elements-1];
						head_list_minimum = &ranking_queue[ranking_elements-1];
					}
					else {
						while(p_temp->next != NULL) {	// middle
							if(p_temp->next->value > paths_sum) {
								ranking_queue[ranking_elements-1].next = p_temp->next;
								ranking_queue[ranking_elements-1].prev = p_temp;
								p_temp->next->prev = &ranking_queue[ranking_elements-1];
								p_temp->next = &ranking_queue[ranking_elements-1];
								break;
							}
							p_temp = p_temp->next;
						}
						if(p_temp->next == NULL) {	// tail
							p_temp->next = &ranking_queue[ranking_elements-1];
							ranking_queue[ranking_elements-1].prev = p_temp;
							ranking_queue[ranking_elements-1].next = NULL;
							tail_list_maximum = &ranking_queue[ranking_elements-1];
						}
					}

				}
			}
			else {	// No vacant spot available	
				if (paths_sum < tail_list_maximum->value) {				
					if (paths_sum < head_list_minimum->value) {	//head
						old_maximum = tail_list_maximum;
						tail_list_maximum = tail_list_maximum->prev;
						tail_list_maximum->next = NULL;
						old_maximum->index = curr_index;
						old_maximum->value = paths_sum;
						old_maximum->prev = NULL;
						old_maximum->next = head_list_minimum;
						head_list_minimum->prev = old_maximum;
						head_list_minimum = old_maximum;
					}
					else {
						flag = 0;
						p_temp = head_list_minimum->next;
						while(p_temp->next != NULL) {	// middle
							if (paths_sum < p_temp->value) {
								old_maximum = tail_list_maximum;
								tail_list_maximum = tail_list_maximum->prev;
								old_maximum->index = curr_index;
								old_maximum->value = paths_sum;
								old_maximum->prev = p_temp->prev;
								old_maximum->next = p_temp;
								p_temp->prev = old_maximum;
								old_maximum->prev->next = old_maximum;
								tail_list_maximum->next = NULL;
								flag = 1;
								break;
							}
							p_temp = p_temp->next;
						}
						if(p_temp->next == NULL && paths_sum < p_temp->value && flag == 0) {	// tail
							tail_list_maximum->index = curr_index;
							tail_list_maximum->value = paths_sum;
						}
					}
				}		
			}
			
			// Graph index counter
			curr_index++;
		}

		// --> TopK
		else if (chr == 'K') {
			// --> Print ranking
			first = 1;
			for (int i = 0; i < ranking_elements; i++) {
				if (first == 1) {
					printf("%d", ranking_queue[i].index);
					first = 0;
				}
				else {
					printf(" %d", ranking_queue[i].index);
				}
			}
			printf("\n");
		}

		chr = getchar_unlocked();
	}	
}