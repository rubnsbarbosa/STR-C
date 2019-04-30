#include <stdio.h>
#include <pthread.h>

// gcc work3.c -o w3 -lpthread

void sendAsync(int *buf, int c);
void receive(int *buf, int c);

int canal[6] = {-1, -1, -1, -1, -1, -1};

int fa(){
    return 5;
}

int fb(){
    return 550;
}

int fc(){
    return 610;
}

void *TA()
{
    int x, y, z;
    int status = -5;

    do {
        x = fa();
        printf("TA executando fa\n");
        sendAsync(&x, 0);
        receive(&y, 2);

    } while (y >= 1 && y <= 10);

    if (y == -5) {
        do {
            x = fa();
            printf("TA executando fa, TB em estado de erro\n");
            sendAsync(&x, 3);
            receive(&y, 2);

        } while (y >= 1 && y <= 10);

	while(1){
        printf("TA executando fa, fb e fc, TB e TC em estado de erro\n");
        x = fa();
        y = fb();
        z = fc();
    	}

    } else {
        sendAsync(&status, 0);
        receive(&y, 5);
        // Testa intervalo de TC
	    while ((y >= 50) && (y <= 100)){
            x = fa();
            z = fc();
            printf("TA executando fa e fc, TC em estado de erro\n");
            sendAsync(&x, 0);
            receive(&y, 5);            
            
        }
	
	while(1){
        printf("TA executando fa, fb e fc, TB e TC em estado de erro\n");
        x = fa();
        y = fb();
        z = fc();
    	}

    }    
}

void *TB(){

    int x, y, z;
    int status = -5;
    
    // Testa intervalo de TA
    receive(&y, 0);
	while (y >= 5 && y <= 10) {
	    x = fb();
	    printf("TB executando fb\n");
	    sendAsync(&x, 1);
	    receive(&y, 0);                    
	} 

    if (y == -5) {
        do {
            x = fb();
            printf("TB executando fb, TC em estado de erro\n");
            sendAsync(&x, 5);
            receive(&y, 0);

        } while (y >= 5 && y <= 10);
	
	while(1){
        printf("TB executando fa, fb e fc, TA e TC em estado de erro\n");
        x = fa();
        y = fb();
        z = fc();
    	}

    } else {
        sendAsync(&status, 1);
        receive(&y, 4);
        // Testa intervalo de TC
	    while ((y >= 1) && (y <= 10)){
            x = fb();
            z = fa();
            printf("TB executando fb e fa, TA em estado de erro\n");
            sendAsync(&x, 1);
            receive(&y, 4);            
            
        }
    
    	while(1){
            printf("TB executando fa, fb e fc, TA e TC em estado de erro\n");
            x = fa();
            y = fb();
       	    z = fc();
    	}
    }
}

void *TC(){

    int x, y, z;
    int status = -5;
    
    // Testa intervalo de TA
    receive(&y, 1);
	while (y >= 50 && y <= 100) {
	    x = fc();
	    printf("TC executando fc\n");
	    sendAsync(&x, 2);
	    receive(&y, 1);                    
	} 

    if (y == -5) {
        do {
            x = fc();
            printf("TC executando fc, TA em estado de erro\n");
            sendAsync(&x, 4);
            receive(&y, 1);

        } while (y >= 50 && y <= 100);
	
	while(1){
        printf("TC executando fa, fb e fc, TA e TB em estado de erro\n");
        x = fa();
        y = fb();
        z = fc();
        }

    } else {        
        sendAsync(&status, 2);
        receive(&y, 3);
        // Testa intervalo de TB
	    while ((y >= 5) && (y <= 10)){
            x = fc();
            z = fb();
            printf("TC executando fc e fb, TB em estado de erro\n");
            sendAsync(&x, 2);
            receive(&y, 3);            
            
        }
    
        while(1){
            printf("TC executando fa, fb e fc, TA e TB em estado de erro\n");
            x = fa();
            y = fb();
            z = fc();
        }
    }
}



void sendAsync(int *buf, int c){

    canal[c] = *buf;
    return;
}

void receive(int *buf, int c){

    while (canal[c] == -1);
    *buf = canal[c];
    canal[c] = -1;
    return;
}

int main(){

	pthread_t thread_idA;
	pthread_t thread_idB;
	pthread_t thread_idC;
	int status;

	status = pthread_create(&thread_idA, NULL, TA, NULL); 
    	if (status != 0)
	{
		printf("Erro ao tentar criar a Thread A.\n");
		return 0;
	}

	status = pthread_create(&thread_idB, NULL, TB, NULL);
        if (status != 0)
	{
		printf("Erro ao tentar criar a Thread B.\n");
		return 0;
	}

	status = pthread_create(&thread_idC, NULL, TC, NULL);
        if (status != 0)
	{
		printf("Erro ao tentar criar a Thread C.\n");
		return 0;
	}

	pthread_exit(NULL);
	return 0;
}
