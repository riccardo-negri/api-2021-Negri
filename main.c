// My solution for Algoritmi e Principi dell'Informatica 2020-2021's project
#include <stdio.h>
#include <stdlib.h>

#define INFINITY 4294967295

typedef struct node {
   unsigned int id;
   unsigned int dist;
} node_type;

typedef struct graph {
   unsigned int index;
   unsigned long int value;
} graph_type;

void MinHeapify(node_type * min_heap, unsigned int d, unsigned int heap_start, unsigned int i) {
	unsigned int l, r, min, temp_id, temp_dist;
	l = 2*(i-heap_start)+heap_start;
	r = l+1;
	if (l < d && min_heap[l].dist < min_heap[i].dist) min = l;
	else min = i;

	if (r < d && min_heap[r].dist < min_heap[min].dist) min = r;

	if (min != i) {
		temp_id = min_heap[i].id;
		temp_dist = min_heap[i].dist;
		min_heap[i].id = min_heap[min].id;
		min_heap[i].dist = min_heap[min].dist;
		min_heap[min].id = temp_id;
		min_heap[min].dist = temp_dist;
		MinHeapify(min_heap, d, heap_start, min);
	}

	return;
}

void MaxHeapify(graph_type * max_heap, unsigned int k, unsigned int i) {
	unsigned int max, temp_index;
	unsigned long int temp_value;
	unsigned int l = 2*i;
	unsigned int r = l+1;

	if (l < k && max_heap[l].value > max_heap[i].value) max = l;
	else max = i;

	if (r < k && max_heap[r].value > max_heap[max].value) max = r;
	//printf("--> %lu, %lu. max:%u, i:%u\n", max_heap[l].value, max_heap[i].value, max, i);
	if (max != i) {
		temp_index = max_heap[i].index;
		temp_value = max_heap[i].value;
		max_heap[i].index = max_heap[max].index;
		max_heap[i].value = max_heap[max].value;
		max_heap[max].index = temp_index;
		max_heap[max].value = temp_value;
		MaxHeapify(max_heap, k, max);
	}

	return;
}

void StampaMatrice(unsigned int dim, unsigned int matrix[dim][dim]) {
	unsigned int i, j;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			printf("%u ", matrix[i][j]);
		}
		printf("\n");
	}
	fflush( stdout );
	return;
}

void StampaMinHeap(node_type * min_heap, unsigned int heap_start, unsigned int d) {
	unsigned int i;
	unsigned int start = 1;
	printf("[");
	for (i = heap_start; i < d; i++) {
		if (start == 1) {
			start = 0;
			printf("{id:%u, dist:%u}", min_heap[i].id, min_heap[i].dist);
		}
		else printf("\n{id:%u, dist:%u}", min_heap[i].id, min_heap[i].dist);
	}
	printf("]\n");
	fflush( stdout );
}

void StampaMaxHeap(graph_type * min_heap, unsigned int d) {
	unsigned int i;
	unsigned int start = 1;
	printf("[");
	for (i = 0; i < d; i++) {
		if (start == 1) {
			start = 0;
			printf("{ind:%u, value:%lu}", min_heap[i].index, min_heap[i].value);
		}
		else printf("\n{ind:%u, value:%lu}", min_heap[i].index, min_heap[i].value);
	}
	printf("]\n");
	fflush( stdout );
}

int main() {
	unsigned int d = 0; // number of nodes
	char char_letto;
	unsigned int k = 0; // length of top graphs ranking
	unsigned int row;
	unsigned int col;
	unsigned int temp_num;
	unsigned int i, j, curr_min_id, curr_min_dist, ndist, arch_weigth, heapsize, heap_start;
	unsigned long int somma_cammini;
	unsigned int elementi_classifica = 0;
	unsigned int indice_grafo_corr = 0;
	long int w; // lo uso in loop dove può diventare -1, in realtà in uno no e in uno sì

	char_letto = getchar_unlocked();
	while(char_letto != ' ') {
		d = d*10 + char_letto-'0';
		char_letto = getchar_unlocked();	
	}
	
	char_letto = getchar_unlocked();
	while(char_letto != '\n') {
		k = k*10 + char_letto-'0';
		char_letto = getchar_unlocked();	
	}

	//printf("Dati letti: d è %u, k è %u\n", d, k);
	fflush( stdout );

	unsigned int matrice_adiacenza[d][d];
	node_type min_heap_dijkstra[d];
	graph_type classifica[k];

	while(char_letto != EOF) {

		if (char_letto == 'A') {
			// --> Lettura e importazione
			while (char_letto != '\n') {
				char_letto = getchar_unlocked();	
			}
			for (row = 0; row < d; row++) {
				min_heap_dijkstra[row].id = row;
				min_heap_dijkstra[row].dist = INFINITY;
				for (col = 0; col < d; col++) {
					char_letto = getchar_unlocked();
					temp_num = 0;
					while (char_letto != ',' && char_letto != '\n') {
						temp_num = 10*temp_num + char_letto-'0';
						char_letto = getchar_unlocked();
					}
					matrice_adiacenza[row][col] = temp_num;
				}
			}

			//StampaMatrice(d, matrice_adiacenza);

			//printf("HERE");
			//fflush( stdout );

			// --> Calcolo somma cammini
			min_heap_dijkstra[0].dist = 0;
			somma_cammini = 0;
			for (i = 0; i < d; i++) {
				curr_min_id = min_heap_dijkstra[i].id;
				curr_min_dist =  min_heap_dijkstra[i].dist;
				if(curr_min_dist < INFINITY) {
					somma_cammini += curr_min_dist;
				}
				else {
					break;
				}
				//printf("Qui, curr min id: %u\n", curr_min_id);
				fflush( stdout );
				for (j = i+1; j<d; j++) {
					arch_weigth = matrice_adiacenza[curr_min_id][min_heap_dijkstra[j].id];
					if (arch_weigth != 0) {
						ndist = curr_min_dist + arch_weigth;
						if (min_heap_dijkstra[j].dist > ndist) {
							min_heap_dijkstra[j].dist = ndist;
						}
					}
				}
				// --> Rimetto in ordine il MinHeap ((heapsize/2)+heap_start; i >= heap_start+1; i--)
				heapsize = d-i-1;
				heap_start = i+1;
				//printf("Parto con MinHeapify: heapsize of %u, heap start at %u\n", heapsize, heap_start);
				fflush( stdout );
				//StampaMinHeap(min_heap_dijkstra, heap_start, d);

				for (w = ((heapsize)/2)+heap_start; w >= heap_start; w--) {
					//printf("w:%lu\n", w);
					MinHeapify(min_heap_dijkstra, d, heap_start, w);
				}

				//printf("Finito di sistemare il MinHeap\n");
				//StampaMinHeap(min_heap_dijkstra, heap_start, d);
				//printf("\n");
				fflush( stdout );
			}

			// --> Inserimento nella classifica dei grafi
			//printf("{ind:%u, value:%lu}\n", indice_grafo_corr, somma_cammini);

			if (elementi_classifica < k) {
				classifica[elementi_classifica].index = indice_grafo_corr;
				classifica[elementi_classifica].value = somma_cammini;
				elementi_classifica++;
				//printf("%u", elementi_classifica);
				//fflush( stdout );
			}
			else {
				// Prima sistemo la classifica, poi tolgo il più grande se è il caso
				//if(somma_cammini < classifica[0].value || somma_cammini < classifica[1].value || somma_cammini < classifica[2].value) {
					
					//printf("Parto con MaxHeapify, ciclo num %ld\n", w);
					//StampaMaxHeap(classifica, elementi_classifica);
					
					for (w = k/2; w >= 0; w--) {
						MaxHeapify(classifica, k, w);	
					}

					//StampaMaxHeap(classifica, elementi_classifica);
					//printf("Finito di sistemare MaxHeap\n");
					//printf("\n");
					

					if (somma_cammini < classifica[0].value) {
						classifica[0].value = somma_cammini;
						classifica[0].index = indice_grafo_corr;
					}
				//}
			}

			// Contatore
			indice_grafo_corr++;
		}

		// --> TopK
		else if (char_letto == 'K') {
			// --> Stampa classifica
			//printf("Stampa classifica: \n");
			fflush( stdout );

			int start = 1;
			for (int i = 0; i < elementi_classifica; i++) {
				if (start == 1) {
					printf("%d", classifica[i].index);
					start = 0;
				}
				else {
					printf(" %d", classifica[i].index);
				}
			}
			printf("\n");
		}

		char_letto = getchar_unlocked();
	}	
}