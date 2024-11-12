#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>

int_fast8_t** create_graph(int size);
void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void print_graph(int size, int_fast8_t** graph_pointer);
void DFS(int v, int size, int_fast8_t** graph, bool* visited);
void depth_search(int size, int_fast8_t** graph);

int main(void) {
	setlocale(LC_ALL, "Russian");
	int_fast8_t** graph;
	int size;
	float edge_probability;

	fprintf(stdout, "Введите количество вершин графа: ");
	if (!scanf("%d", &size) || size <= 1) {
		fprintf(stderr, "Ошибка ввода размера массива");
		exit(1);
	}

	fprintf(stdout, "Введите вероятность создания ребра (от 0 до 100): ");
	if (!scanf("%f", &edge_probability) || edge_probability < 0 || edge_probability > 100) {
		fprintf(stderr, "Ошибка ввода вероятности");
		exit(1);
	}

	graph = create_graph(size);
	srand((unsigned int)time(NULL));
	initialize_graph(size, graph, edge_probability);
	printf("Матрица смежности: \n\n");
	print_graph(size, graph);

	printf("\nПорядок обхода вершин в глубину: ");
	depth_search(size, graph);

	for (int i = 0; i < size; i++) {
		free(graph[i]);
	}
	free(graph);

	return 0;
}

int_fast8_t** create_graph(int size) {
	int_fast8_t** array = (int_fast8_t**)malloc(sizeof(int_fast8_t*) * size);
	for (int i = 0; i < size; i++) {
		array[i] = (int_fast8_t*)malloc(sizeof(int_fast8_t) * size);
	}
	if (array == NULL) {
		fprintf(stderr, "Ошибка создания массива");
		exit(1);
	}
	return array;
}

void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if ((float)rand() / (RAND_MAX / 100) < edge_probability) {
				graph_pointer[i][j] = 1;
				graph_pointer[j][i] = 1;
			}
			else {
				graph_pointer[i][j] = 0;
				graph_pointer[j][i] = 0;
			}
		}
		graph_pointer[i][i] = 0;
	}
}

void print_graph(int size, int_fast8_t** graph_pointer) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fprintf(stdout, "%d ", graph_pointer[j][i]);
		}
		fprintf(stdout, "\n");
	}
}

void DFS(int v, int size, int_fast8_t** graph, bool* visited) {
	visited[v] = true;
	printf("%d ", v + 1);

	for (int i = 0; i < size; i++) {
		if (graph[v][i] == 1 && !visited[i]) {
			DFS(i, size, graph, visited);
		}
	}
}

void depth_search(int size, int_fast8_t** graph) {
	bool* visited = (bool*)calloc(size, sizeof(bool));
	if (visited == NULL) {
		fprintf(stderr, "Ошибка выделения памяти");
		exit(1);
	}

	for (int v = 0; v < size; v++) {
		if (!visited[v]) {
			DFS(v, size, graph, visited);
		}
	}

	free(visited);
	printf("\n");
}