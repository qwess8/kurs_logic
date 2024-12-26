#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>


typedef struct Trio {
	int vertex;
	int deg;
	int color;
} Trio;


int compare(const void* a, const void* b) {
	return ((Trio*)b)->deg - ((Trio*)a)->deg;
}
int compare_ver(const void* a, const void* b) {
	return ((Trio*)a)->vertex - ((Trio*)b)->vertex;
}

int** createG(int size) {
	int** G = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++) {
		G[i] = (int*)malloc(size * sizeof(int));
	}
	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			G[i][j] = rand() % 2;
			G[j][i] = (i == j) ? 0 : G[i][j];
		}
	}
	return G;
}

int** createGmanualy(int size) {
	int** G = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++) {
		G[i] = (int*)malloc(size * sizeof(int));
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			scanf_s("%d", &G[i][j]);
			G[j][i] = (i == j) ? 0 : G[i][j];
		}
	}
	return G;
}

void printG(int** G, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%d ", G[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void findDeg(int** G, int size, Trio* deg) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			deg[i].deg += G[i][j];
		}
		deg[i].vertex = i;
	}
}

int findChrom(int** G, int size, Trio* deg, Trio* degV) {
	int color = 1, max = 0;

	deg[0].color = color;
	degV[deg[0].vertex].color = color;

	for (int i = 1; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (G[deg[i].vertex][j] == 1 && degV[j].color == color) {
				color++;
				j = 0;
			}
		}
		deg[i].color = color;
		degV[deg[i].vertex].color = color;
		color = 1;
	}

	for (int i = 1; i < size; i++) {
		if (deg[i].color > max) max = deg[i].color;
	}

	return max;
}


int main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);

	int size = 7;
	int** G = NULL;
	printf("Хотите сгенерировать случайный граф?(Y/N)");
	char rand = getchar();
	printf("Введите размер графа: ");
	scanf_s("%d", &size);
	if (rand == 'y' || rand == 'Y') G = createG(size);
	else G = createGmanualy(size);
	Trio* degD = (Trio*)malloc(size * sizeof(Trio));
	for (int i = 0; i < size; i++) {
		degD[i].deg = 0;
	}

	findDeg(G, size, degD);
	Trio* degV = (Trio*)malloc(size * sizeof(Trio));
	for (int i = 0; i < size; i++) {
		degV[i] = degD[i];
	}

	qsort(degD, size, sizeof(Trio), compare);
	qsort(degV, size, sizeof(Trio), compare_ver);

	int chromNum = findChrom(G, size, degD, degV);

	printf("\nГраф:\n");
	printG(G, size);

	printf("\n\nХроматическое число этого графа: %d", chromNum);
	printf("\n\nЦвета вершин:\n");

	for (int i = 1; i < chromNum + 1; i++) {
		for (int j = 0; j < size; j++) {
			if (degD[j].color == i) {
				SetConsoleTextAttribute(console_color, i + 1);
				printf("%d ", degD[j].vertex);
			}
		}
		printf("\n");
	}

	FILE* file = fopen("result.csv", "a");
	fprintf(file, "n: %d\n\n", size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fprintf(file, "%d    ", G[i][j]);
		}
		fprintf(file, "\n");
	}

	fprintf(file, "\nХроматическое число этого графа: %d\n__________________________________\n", chromNum);
	fclose(file);


	getchar();
	getchar();
	return 0;
}

