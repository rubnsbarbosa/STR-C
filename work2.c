#include <stdio.h>
#include <pthread.h>
// Programa N-Versao

void sendAsync(int *buf, int c);
void receive(int *buf, int c);

int canal[6] = {-1, -1, -1, -1, -1, -1};

void *threadVersaoA()
{
	int erro;
	int voto = 0, x = 5, y = 5;
	voto = (x + y);

	sendAsync(&voto, 0);
	receive(&erro, 3);

	if (erro == 0)
	{
		printf("Voto Correto - Versao A\n");
		while (1);
	}
	else
	{
		printf("Voto Errado - Versao A\n");
		return 0;
	}

	return NULL;
}

void *threadVersaoB()
{
	int erro;
        int voto = 0, x = 2, y = 5;
        voto = (x * y);

        sendAsync(&voto, 1);

	receive(&erro, 4);

        if (erro == 0)
        {
                printf("Voto Correto - Versao B\n");
                while (1);
        }
        else 
        {
		printf("Voto Errado - Versao B\n");
                return 0;
        }

        return NULL;
}

void *threadVersaoC()
{
	int erro;
        int voto = 0, x = 6, y = 5;
        voto = (x + y);

        sendAsync(&voto, 2);

	receive(&erro, 5);

        if (erro == 0)
        {
                printf("Voto Correto - Versao C\n");
                while (1);
        }
        else 
        { 
                printf("Voto Errado - Versao C\n");
                return 0;
        }

        return NULL;
}

void sendAsync(int *buf, int c)
{
	canal[c] = *buf;
	return;
}

void receive(int *buf, int c)
{
	while (canal[c] == -1);
	*buf = canal[c];
	canal[c] = -1;
	return;
}

int compara(int vetorComp[], int *versaoError)
{
	if ((vetorComp[0] == vetorComp[1]) && (vetorComp[1] != vetorComp[2]))
	{
		*versaoError = 2;
		return vetorComp[0];
	}
	else if ((vetorComp[0] != vetorComp[1]) && (vetorComp[1] == vetorComp[2]))
        {
                *versaoError = 0;
                return vetorComp[1];
        }
	else if ((vetorComp[0] == vetorComp[2]) && (vetorComp[1] != vetorComp[2]))
        {
                *versaoError = 1;
                return vetorComp[2];
        }
	else
	{
		return vetorComp[0]; // pode ser qqr indice, pois todos estao ==
	}
}

void *threadDrive()
{
	pthread_t thread_idA;
	pthread_t thread_idB;
	pthread_t thread_idC;
	int status;	
	int comp[3] = {0, 0, 0};
	int votoCorreto, erroVersao;

	status = pthread_create(&thread_idA, NULL, threadVersaoA, NULL); 
    	if (status != 0)
	{
		printf("Erro ao tentar criar a Thread VA.\n");
		return 0;
	}

	status = pthread_create(&thread_idB, NULL, threadVersaoB, NULL);
        if (status != 0)
        {
                printf("Erro ao tentar criar a Thread VB.\n");
                return 0;
        }

	status = pthread_create(&thread_idC, NULL, threadVersaoC, NULL);
        if (status != 0)
        {
                printf("Erro ao tentar criar a Thread VC.\n");
                return 0;
        }

	receive(&comp[0], 0);
	receive(&comp[1], 1);
	receive(&comp[2], 2);

	votoCorreto = compara(comp, &erroVersao);

	int statusErro = 1;
	int statusOk = 0;
	if (erroVersao == 0)
	{
		sendAsync(&statusErro, 3);
		sendAsync(&statusOk, 4);
		sendAsync(&statusOk, 5);
	} 
	else if (erroVersao == 1)
        {
                sendAsync(&statusErro, 4);
                sendAsync(&statusOk, 3);
                sendAsync(&statusOk, 5);
        } 
	else if (erroVersao == 2)
        {
                sendAsync(&statusErro, 5);
                sendAsync(&statusOk, 4);
                sendAsync(&statusOk, 3);
        } 

	printf("Voto Majoritario: %d\n", votoCorreto);
	int i;
	for (i = 0; i < 3; i++)
		printf("%d\n", comp[i]);

	return NULL;
}

int main()
{
	threadDrive();
	return 0;
}
