#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../src/header.h"

inline static double double_rand()
{
	double result = 0;
	for (int i = 0; i < 20; i++)
		result += (double)(rand() % 1000) / 400;
	return (double)((int)(result * 100)) / 100;
}

void function(const void* key, const void* data, const void* arg)
{
	printf("%lg %d\n", *(double*)key, *(int*)data);
	return;
}

int main(int argc, char* argv, char** env)
{
	const int N = 1000000;

	map_t M = MapHashCreate(sizeof(double), sizeof(int));

	for (int i = 0; i < N; i++) {
		double now = double_rand();
		if (M->find(M, &now)) {
			int cnt;
			M->get(M, &now, &cnt);
			cnt++;
			M->set(M, &now, &cnt);
		}
		else {
			int cnt = 1;
			M->set(M, &now, &cnt);
		}
	}
	M->foreach(M, NULL, function);
	M->destroy(M);

	return 0;
}
