#include <stdio.h>
#include <pthread.h>
// gcc work1.c -o w1 -lpthread

char vetor[30];
int indice = 0;
int FLAGA = 1;
int FLAGB = 0;
int FLAGC = 0;

void *threadA()
{
	while (indice <= 27)
	{
		while (FLAGA == 0);
		vetor[indice] = 'A';
		indice = indice + 1;
		FLAGA = 0;
		FLAGB = 1;
	}
	return NULL;
}

void *threadB()
{
	while (indice <= 28)
	{
		while (FLAGB == 0);
		vetor[indice] = 'B';
		indice = indice + 1;
		FLAGB = 0;
		FLAGC = 1;
	}
	return NULL;
}

void *threadC()
{
	while (indice <= 29)
	{
		while (FLAGC == 0);
		vetor[indice] = 'C';
		indice = indice + 1;
		FLAGC = 0;
		FLAGA = 1;
	}
	int i;
	for (i = 0; i < 30; i++)
		printf("%c", vetor[i]);
	printf("\n");
	return NULL;
}

int main()
{
	pthread_t thread_idA;
	pthread_t thread_idB;
	pthread_t thread_idC;
	int status;

	status = pthread_create(&thread_idA, NULL, threadA, NULL); 
    	if (status != 0)
	{
		printf("Erro ao tentar criar a Thread A.\n");
		return 0;
	}

	status = pthread_create(&thread_idB, NULL, threadB, NULL);
        if (status != 0)
	{
		printf("Erro ao tentar criar a Thread B.\n");
		return 0;
	}

	status = pthread_create(&thread_idC, NULL, threadC, NULL);
        if (status != 0)
	{
		printf("Erro ao tentar criar a Thread C.\n");
		return 0;
	}

	pthread_exit(NULL);
	return 0;
}
