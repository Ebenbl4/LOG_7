#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>

typedef struct Node {
	int vertex;
	struct Node* next;
} Node;

typedef struct {
	int* data;
	int top;
	int capacity;
} Stack;

int_fast8_t** create_graph(int size);
void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void print_graph(int size, int_fast8_t** graph_pointer);
Node** conv_to_adj_list(int size, int_fast8_t** graph);
void print_adj_list(int size, Node** adj_list);
void free_adj_list(int size, Node** adj_list);
void matrix_DFS(int v, int size, int_fast8_t** graph, bool* visited);
void matrix_depth_search(int size, int_fast8_t** graph);
void adj_list_DFS(Node** adj_list, int vertex, bool* visited, int size);
void adj_list_depth_search(Node** adj_list, int size);
void stack_matrix_DFS(int v, int size, int_fast8_t** graph, bool* visited);
void stack_matrix_depth_search(int size, int_fast8_t** graph);
void stack_adj_list_DFS(Node** adj_list, int start, int size, bool* visited);
void stack_adj_list_depth_search(Node** adj_list, int size);

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
	//print_graph(size, graph);

	printf("\nСписок смежности: \n\n");
	Node** adj_list = conv_to_adj_list(size, graph);
	//print_adj_list(size, adj_list);

	printf("\nРекурсивный порядок обхода вершин в глубину матрицы смежности:   ");
	//matrix_depth_search(size, graph);

	printf("\nРекурсивный порядок обхода вершин в глубину списка смежности:    ");
	//adj_list_depth_search(adj_list, size);

	printf("\nНерекурсивный порядок обхода вершин в глубину матрицы смежности: ");
	stack_matrix_depth_search(size, graph);

	printf("\nНерекурсивный порядок обхода вершин в глубину списка смежности:  ");
	stack_adj_list_depth_search(adj_list, size);

	for (int i = 0; i < size; i++) {
		free(graph[i]);
	}
	free(graph);
	free_adj_list(size, adj_list);
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

Node** conv_to_adj_list(int size, int_fast8_t** graph) {
	Node** adj_list = (Node**)malloc(size * sizeof(Node*));

	for (int i = 0; i < size; i++) {
		adj_list[i] = NULL;
	}

	for (int i = 0; i < size; i++) {
		Node* last = NULL;

		for (int j = 0; j < size; j++) {
			if (graph[i][j]) {
				Node* new_node = (Node*)malloc(sizeof(Node));
				new_node->vertex = j;
				new_node->next = NULL;

				if (adj_list[i] == NULL) {
					adj_list[i] = new_node;
				}

				else {
					last->next = new_node;
				}
				last = new_node;
			}
		}
	}
	return adj_list;
}

void print_adj_list(int size, Node** adj_list) {
	for (int i = 0; i < size; i++) {
		printf("%d: ", i + 1);
		Node* current = adj_list[i];
		while (current != NULL) {
			printf("%d ", current->vertex + 1);
			current = current->next;
		}
		printf("\n");
	}
}

void free_adj_list(int size, Node** adj_list) {
	for (int i = 0; i < size; i++) {
		Node* current = adj_list[i];
		while (current != NULL) {
			Node* temp = current;
			current = current->next;
			free(temp);
		}
	}
	free(adj_list);
}

void matrix_DFS(int v, int size, int_fast8_t** graph, bool* visited) {
	visited[v] = true;
	printf("%d ", v + 1);

	for (int i = 0; i < size; i++) {
		if (graph[v][i] == 1 && !visited[i]) {
			matrix_DFS(i, size, graph, visited);
		}
	}
}

void matrix_depth_search(int size, int_fast8_t** graph) {
	bool* visited = (bool*)calloc(size, sizeof(bool));
	if (visited == NULL) {
		fprintf(stderr, "Ошибка выделения памяти");
		exit(1);
	}

	for (int v = 0; v < size; v++) {
		if (!visited[v]) {
			matrix_DFS(v, size, graph, visited);
		}
	}

	free(visited);
	printf("\n");
}

void adj_list_DFS(Node** adj_list, int vertex, bool* visited, int size) {
	visited[vertex] = true;
	printf("%d ", vertex + 1);

	Node* current = adj_list[vertex];
	while (current != NULL) {
		if (!visited[current->vertex]) {
			adj_list_DFS(adj_list, current->vertex, visited, size);
		}
		current = current->next;
	}
}

void adj_list_depth_search(Node** adj_list, int size) {
	bool* visited = (bool*)calloc(size, sizeof(bool));
	if (visited == NULL) {
		fprintf(stderr, "Ошибка выделения памяти\n");
		exit(1);
	}

	for (int v = 0; v < size; v++) {
		if (!visited[v]) {
			adj_list_DFS(adj_list, v, visited, size);
		}
	}

	free(visited);
	printf("\n");
}

Stack* createStack(int capacity) {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	if (stack == NULL) {
		fprintf(stderr, "Ошибка выделения памяти для стека");
		exit(1);
	}
	stack->capacity = capacity;
	stack->top = -1;
	stack->data = (int*)malloc(stack->capacity * sizeof(int));
	if (stack->data == NULL) {
		fprintf(stderr, "Ошибка выделения памяти для данных стека");
		free(stack);
		exit(1);
	}
	return stack;
}

int isEmpty(Stack* stack) {
	return stack->top == -1;
}

void push(Stack* stack, int item) {
	if (stack->top >= stack->capacity - 1) {
		fprintf(stderr, "Переполнение стека");
		free(stack->data);
		free(stack);
		exit(1);
	}
	stack->data[++stack->top] = item;
}

int pop(Stack* stack) {
	if (isEmpty(stack)) {
		fprintf(stderr, "Стек пуст");
		free(stack->data);
		free(stack);
		exit(1);
	}
	return stack->data[stack->top--];
}

void stack_matrix_DFS(int start, int size, int_fast8_t** graph, bool* visited) {
	Stack* stack = createStack(size * size);

	push(stack, start);

	while (!isEmpty(stack)) {
		int v = pop(stack);

		if (!visited[v]) {
			visited[v] = true;
			printf("%d ", v + 1);

			for (int i = size - 1; i >= 0; i--) {
				if (graph[v][i] == 1 && !visited[i]) {
					push(stack, i);
				}
			}
		}
	}

	free(stack->data);
	free(stack);
}

void stack_matrix_depth_search(int size, int_fast8_t** graph) {
	bool* visited = (bool*)calloc(size, sizeof(bool));
	if (visited == NULL) {
		fprintf(stderr, "Ошибка выделения памяти");
		exit(1);
	}

	for (int v = 0; v < size; v++) {
		if (!visited[v]) {
			stack_matrix_DFS(v, size, graph, visited);
		}
	}

	free(visited);
	printf("\n");
}

void stack_adj_list_DFS(Node** adj_list, int start, int size, bool* visited) {
	Stack* stack = createStack(size * size);
	push(stack, start);

	while (!isEmpty(stack)) {
		int v = pop(stack);

		if (!visited[v]) {
			visited[v] = true;
			printf("%d ", v + 1);

			Node* current = adj_list[v];
			int* temp = (int*)malloc(size * sizeof(int));
			int count = 0;

			while (current != NULL) {
				if (!visited[current->vertex]) {
					temp[count++] = current->vertex;
				}
				current = current->next;
			}

			for (int i = count - 1; i >= 0; i--) {
				push(stack, temp[i]);
			}

			free(temp);
		}
	}

	free(stack->data);
	free(stack);
}

void stack_adj_list_depth_search(Node** adj_list, int size) {
	bool* visited = (bool*)calloc(size, sizeof(bool));
	if (visited == NULL) {
		fprintf(stderr, "Ошибка выделения памяти");
		exit(1);
	}

	for (int v = 0; v < size; v++) {
		if (!visited[v]) {
			stack_adj_list_DFS(adj_list, v, size, visited);
		}
	}
	
	free(visited);
	printf("\n");
}