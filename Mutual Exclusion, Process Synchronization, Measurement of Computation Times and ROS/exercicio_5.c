#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#define S_TO_NS 1E9


double maximo_x, maximo_y, maximo_z;
double minimo_x, minimo_y, minimo_z;
double media_x, media_y, media_z;
double desvio_x, desvio_y, desvio_z;


//MUTEX
struct timespec Aumenta_1_segundo(struct timespec tempo){

	struct timespec resultado;
			
	resultado.tv_nsec=tempo.tv_nsec + 100000000;

	if (resultado.tv_nsec > (int)S_TO_NS-1){
		
		resultado.tv_nsec = resultado.tv_nsec % (int)S_TO_NS;
		resultado.tv_sec = 1 + tempo.tv_sec;
	}
	
	else{
		resultado.tv_sec = 0 + tempo.tv_sec;
	}	
	
	return resultado;
}

pthread_mutex_t block[3];

// Semáforo para garantir que é a primeira que começa
sem_t semaforo, semaforo_2;
void *Thread1(void *);
void *Thread2(void *);
void *Thread3(void *);


typedef struct{
	double *x, *y, *z;
  	int npoints;
}dados;

double GetMS(struct timespec inicio, struct timespec fim){
	
	double aux1, aux2;

	aux1 = inicio.tv_sec * 1000000000 + inicio.tv_nsec;
	aux2 = fim.tv_sec * 1000000000 + fim.tv_nsec;
	aux1  = (aux2 - aux1) / 1000000.0;

	return aux1;
}


void leitura_ficheiro(char *nomeficheiro, dados *ficheiro){

	int i = 0;
	int numero_pontos = 0;
	char buffer_1[32];
	char buffer_2[32];
	char buffer_3[32];


	FILE * read = fopen(nomeficheiro, "r");

	if (read == NULL) {
        perror("Error");
        return 0;
    }

    else { 	
		char aux;
		fseek(read, 0, SEEK_SET);
	   	while(fscanf(read, "%s %s %s\n", buffer_1, buffer_2, buffer_3) != EOF){
			i++;
		}
	
	fclose(read);


	ficheiro->x = malloc(i * sizeof(double));
	ficheiro->y = malloc(i * sizeof(double));
	ficheiro->z = malloc(i * sizeof(double));
	ficheiro->npoints = 0;

	read = fopen(nomeficheiro, "r");

	while(fscanf(read, "%s %s %s\n", buffer_1, buffer_2, buffer_3) != EOF){
		ficheiro->x[numero_pontos] = atof(buffer_1);
		ficheiro->y[numero_pontos] = atof(buffer_2);
		ficheiro->z[numero_pontos] = atof(buffer_3);
		numero_pontos++;
	}

	ficheiro->npoints=i;

	fclose(read);


	//Valor máximo e do valor mínimo:
	maximo_x = ficheiro->x[0];
	maximo_y = ficheiro->y[0];
	maximo_z = ficheiro->z[0];
	minimo_x = ficheiro->x[0];
	minimo_y = ficheiro->y[0];
	minimo_z = ficheiro->z[0];

	int contador = 0;

	while(contador < i){
		if(ficheiro->x[contador] > maximo_x){
			maximo_x = ficheiro->x[contador];
		}

		if(ficheiro->y[contador] > maximo_y){
			maximo_y = ficheiro->y[contador];
		}
				
		if(ficheiro->z[contador] > maximo_z){
			maximo_z = ficheiro->z[contador];
		}
				
		if(ficheiro->x[contador] < minimo_x){
			minimo_x = ficheiro->x[contador];
		}
				
		if(ficheiro->y[contador] < minimo_y){
			minimo_y = ficheiro->y[contador];
		}
				
		if(ficheiro->z[contador] < minimo_z){
			minimo_z= ficheiro->z[contador];
		}	

		media_x += ficheiro->x[contador];
		media_y += ficheiro->y[contador];
		media_z += ficheiro->z[contador];		
		contador++;
	}

	//Cálculo da média:
	media_x = media_x / i;
	media_y = media_y / i;
	media_z = media_z / i;


	int contador_2 = 0;

	while(contador_2 < i){
		desvio_x += pow(ficheiro->x[contador_2] - media_x, 2);
		desvio_y += pow(ficheiro->y[contador_2] - media_y, 2);
		desvio_z += pow(ficheiro->z[contador_2] - media_z, 2);
		contador_2++;
	}	

	//Cálculo do desvio:
	desvio_x = sqrt(desvio_x / i);
	desvio_y = sqrt(desvio_y / i);
	desvio_z = sqrt(desvio_z / i);
	

	//Imprime no ecrã os valores calculados anteriormente:
	printf("\nNome do ficheiro: %s\n", nomeficheiro);
	printf("\nNumero total de pontos: %d\n",i);
	printf("\nValores de x:\n");
	printf("\n\tMaximo: %lf | Minimo: %lf | Media: %lf | Desvio: %lf \n", maximo_x, minimo_x, media_x, desvio_x);
	
	printf("\nValores de y:\n");
	printf("\n\tMaximo: %lf | Minimo: %lf | Media: %lf | Desvio: %lf \n", maximo_y, minimo_y, media_y, desvio_y);
	
	printf("\nValores de z:\n");
	printf("\n\tMaximo: %lf | Minimo: %lf | Media: %lf | Desvio: %lf \n", maximo_z, minimo_z, media_z, desvio_z);
	
	}
}


dados * eliminar_val_negativos_e_proximos(dados *ficheiro){
	
	dados * matriz_reduzida;
	int i = 0;
	int contador = 0;
	matriz_reduzida = (dados*)malloc(sizeof(dados));
	
	//Elimina valores negativos
	matriz_reduzida->x = malloc(ficheiro->npoints * sizeof(double));
	matriz_reduzida->y = malloc(ficheiro->npoints * sizeof(double));
	matriz_reduzida->z = malloc(ficheiro->npoints * sizeof(double));

	while(i < ficheiro->npoints){	  
		if(ficheiro->x[i] < 30 && ficheiro->x[i] >= 3 && abs(ficheiro->y[i]) <= 10 && ficheiro->z[i] < 4){
	  		matriz_reduzida->x[contador] = ficheiro->x[i];
	  		matriz_reduzida->y[contador] = ficheiro->y[i];
	  		matriz_reduzida->z[contador] = ficheiro->z[i];
	  		contador++;
	  	}
	  	i++;
	}

	printf("\nNr. de pontos iniciais = %d  |  Nr. de pontos finais = %d\n", i, contador);

	i = 0;
	matriz_reduzida->npoints = contador;

	return matriz_reduzida;
}



void zona_de_conducao(dados *ficheiro){

	int i = 0;
	double z_min = 150;
	double z_max = -350;
	double x_axis, y_axis;
	int contador;
	int a = 0;
	int b = 0;

	int guarda = ficheiro->npoints;

	for(x_axis = 3; x_axis < 30; x_axis = x_axis + 0.5){			//Percorremos todos os x de 3 a 30
	 	for(y_axis = -10; y_axis < 10; y_axis = y_axis + 0.5){		//Para cada x percorremos todos os y de -10 a 10
	 		
			dados * teste = (dados*)malloc(sizeof(dados));
	 
	 		teste->x = malloc(ficheiro->npoints * sizeof(double));
	 		teste->y = malloc(ficheiro->npoints * sizeof(double));
	  		teste->z = malloc(ficheiro->npoints * sizeof(double));		
	  		contador = 0;
	  		 
	  		i=0;	 
	  		  		 
	 		while(i < ficheiro->npoints){			//Vamos percorrendo o ficheiro
	    		
				if(ficheiro->x[i] > x_axis && ficheiro->x[i] < (x_axis + 0.5)){			//Verificamos se pertence ao quadrado
	    			
					if(ficheiro->y[i] > y_axis && ficheiro->y[i] < (y_axis + 0.5)){		//Verificamos se pertence ao quadrado
	    			
						//Se pertencer passa para a struct "teste" os valores desse quadrado
						teste->x[contador] = ficheiro->x[i];
	  					teste->y[contador] = ficheiro->y[i];
	  					teste->z[contador] = ficheiro->z[i];
	  				
	    				if(ficheiro->z[i] > z_max){			//Se existirem valores de z maiores o valor estabelecido de z_max
	    					z_max = ficheiro->z[i];			//z_max recebe esse valor
	    				}
	    				
						if(ficheiro->z[i] < z_min){			//Se existirem valores de z menores que o valor estabelecido de z_min
	    					z_min = ficheiro->z[i];			//z_min recebe esse valor
	    				}
	    				
						contador++;		//Incrementa para todos os valores dos quadrados			
	    			}
	    		}
	    		i++;
	    	}

	    	i=0;		//Efetuamos o reset para o quadrado seguinte
			int auxiliar = 0;
	    	
			if(z_max > 1.5 || (z_max-z_min) > 0.1){		//Verificamos se o valor de z se encontra dentro dos parâmetros pedidos
	    		
				a = 0;
	    		b = 0;
	    		while(a < ficheiro->npoints){			//Percorremos o ficheiro
	    			//Verificamos se o valor corresponde ao valor do quadrado a eliminar
					if(ficheiro->x[a] == teste->x[b] && ficheiro->y[a] == teste->y[b] && ficheiro->z[a] == teste->z[b]){	 
						
	    				auxiliar = a;
	  					while(auxiliar < ficheiro->npoints){  	
	    					ficheiro->x[auxiliar] = ficheiro->x[auxiliar+1];
	  						ficheiro->y[auxiliar] = ficheiro->y[auxiliar+1];
	  						ficheiro->z[auxiliar] = ficheiro->z[auxiliar+1];

	  						auxiliar++;
	  					}
	    				//Reduzimos os valores do ficheiro, pois estamos a eliminar valores dos quadrados que não cumprem os requisitos
						ficheiro->npoints--;
	    				b++;
	    				a--;	
	    			}
					//Incrementa com cada valor
	    			a++;
	    		}
	    	}
	    	//Reset para os valores iniciais
			z_min = 150;
	  		z_max = -350;
			//Usamos sempre a mesma struct "teste", logo só é alocado a memória necessária pelo ciclo
	    	free(teste);
	    	contador = 0;
	    }
	}	 
	
	printf("\nNr. de pontos iniciais = %d  |  Nr. de pontos finais = %d\n", guarda, ficheiro->npoints);
}


struct ThreadArg{
	struct dados *numero[3];
	struct timespec StartTime;
};

//******************************************************************************************************************************
// THREAD 1

void *Thread1(void *arg_thread){
	
	// Recebe os valores de inicialização
	struct ThreadArg *my_thread;
	my_thread = (struct ThreadArg *)arg_thread;
	struct timespec inicio, fim;

	// Recebe o tempo de entrada na thread para controlar o tempo de forma correta
  	struct timespec acordar = my_thread->StartTime;

	//double tempoiniciar, tempofim;
	double time;
	int i=0;
	

	for(i = 0; i < 3; i++){
  		
		// "Adormece" até chegar o tempo de "acordar"
		// Só começa a leitura do próximo ficheiro no tempo correto, com uma frequência de 10 Hz
   		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &acordar, NULL);
   		acordar = Aumenta_1_segundo(acordar);  //aumenta o tempo para esperar pelo próximo ficheiro
 
  		clock_gettime(CLOCK_MONOTONIC, &inicio);
  		printf("\n --> THREAD 1 comecou: %lfms\n", inicio);
  
  		//==thread 1 vai ler os ficheiros cada um no seu turno
   		//executa as funções criadas nos pontos anteriores
  		if(i==0){
  		
			dados * dados1 = malloc(sizeof(dados));
			dados1 = my_thread->numero[i];
  			leitura_ficheiro("point_cloud1.txt", dados1);
  		}

  		if(i==1){
  		
			dados * dados2 = malloc(sizeof(dados));
			dados2 = my_thread->numero[i];
			leitura_ficheiro("point_cloud2.txt", dados2);
		}

  		if(i==2){	 
  		
			dados * dados3 = malloc(sizeof(dados));
			dados3 = my_thread->numero[i];
			leitura_ficheiro("point_cloud3.txt", dados3);
		}
		
		clock_gettime(CLOCK_MONOTONIC, &acordar);
	
		// Sincronização
 		pthread_mutex_lock(&block[i]);
  		sem_post(&semaforo);						// A thread2 já se pode iniciar
  
  		clock_gettime(CLOCK_MONOTONIC, &fim);
		time = GetMS(inicio, fim);
		printf("\n --> THREAD 1 termina: %lfms\n", time);
		pthread_mutex_unlock(&block[i]);
	}

    pthread_exit(NULL);
}

//******************************************************************************************************************************
// THREAD 2

void *Thread2(void *arg_thread){

	// Recebe valores de inicialização
	struct ThreadArg *my_thread;
	my_thread = (struct ThreadArg *)arg_thread;

  	struct timespec acordar = my_thread->StartTime;
  	struct timespec inicio, fim, tempo_duracao;
	double tempoiniciar, tempofim, time;
	
	int i=0;
	
	
	for(i = 0 ; i < 3; i++){
  
		// Sincronização
		// Vai esperar que a thread 1 acabe para puder começar a processar os dados lidos
  		sem_wait(&semaforo);
  		pthread_mutex_lock(&block[i]);
   		
		clock_gettime(CLOCK_MONOTONIC, &inicio);
   		printf("\n --> THREAD 2 comecou: %lfms\n", inicio);

 		// Executa as funçoes criadas
 		if(i == 0){	
  			my_thread->numero[i] = eliminar_val_negativos_e_proximos(my_thread->numero[i]);
		}

  		if(i == 1){
			my_thread->numero[i] = eliminar_val_negativos_e_proximos(my_thread->numero[i]);
		}
		
  		if(i == 2){
			my_thread->numero[i] = eliminar_val_negativos_e_proximos(my_thread->numero[i]);
		}

		
		// Thread 2 terminou, logo a thread 3 pode iniciar
		sem_post(&semaforo_2);
 		
		 clock_gettime(CLOCK_MONOTONIC, &acordar);	 
   		clock_gettime(CLOCK_MONOTONIC, &fim);
		time = GetMS(inicio, fim);
		printf("\n --> THREAD 2 termina: %lfms\n", time);
		
		pthread_mutex_unlock(&block[i]); 
	}
 		
	pthread_exit(NULL);
}


//******************************************************************************************************************************
// THREAD 3

void *Thread3(void *arg_thread){

 	struct ThreadArg *my_thread;
	my_thread =(struct ThreadArg *)arg_thread;

 
  	struct timespec acordar=my_thread->StartTime;
 	struct timespec inicio,fim,tempo_duracao;
	double tempoiniciar, tempofim,time;
	
	int i=0;


	for(i = 0; i < 3; i++){

		// Sincronização
  		sem_wait(&semaforo_2);
  		pthread_mutex_lock(&block[i]);
  
   		// Vai esperar que a thread 2 acabe, pelo que o mesmo ficheiro vai correr as threads 1, 2 e 3 sequencialmente
   		clock_gettime(CLOCK_MONOTONIC, &inicio);
    	printf("\n --> THREAD 3 comecou: %lfms\n",inicio);

      	// Executa as funções criadas
  		if(i==0){
  			zona_de_conducao(my_thread->numero[i]);
		}

  		if(i==1){
			zona_de_conducao(my_thread->numero[i]);}
		
  		if(i==2){
			zona_de_conducao(my_thread->numero[i]);
		}

		clock_gettime(CLOCK_MONOTONIC, &acordar);
   	  	clock_gettime(CLOCK_MONOTONIC, &fim);
	  	time = GetMS(inicio,fim);
	  
	  	printf("\n --> THREAD 3 termina: %lfms\n", time);
	  	pthread_mutex_unlock(&block[i]); 
	}
 
 	pthread_exit(NULL);
}


//******************************************************************************************************************************
// Main

int main(){

	char *ficheiro1 = "point_cloud1.txt";
	char *ficheiro2 = "point_cloud2.txt";
	char *ficheiro3 = "point_cloud3.txt";
	int i = 0;
	
	struct dados * dadoss[3];
	struct ThreadArg * arg_thread;
	
	struct timespec inicio, inicio_prog, fim_prog;
	double tempo_final;
	
	pthread_t tid1;
	pthread_t tid2;
	pthread_t tid3;
	clock_gettime(CLOCK_MONOTONIC, &inicio_prog);
	
	arg_thread = malloc(3 * sizeof(dados));
	
  	
	for(i = 0; i < 3; i++){
  		pthread_mutex_init(&block[i], NULL);
  	}
  
  	// Inicializa os semáforos
  	sem_init(&semaforo, 0, 0);
	sem_init(&semaforo_2, 0, 0);

    for(i = 0; i < 3; i++){
    	arg_thread->numero[i] = dadoss[i];
    }//passa o ponteiro das structs a manipular
     
 	
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    arg_thread->StartTime = inicio;
  	
	// Inicializa as threads
 	pthread_create(&tid1, NULL, Thread1, &arg_thread);
  	pthread_create(&tid2, NULL, Thread2, &arg_thread);
  	pthread_create(&tid3, NULL, Thread3, &arg_thread); 
	
  	pthread_join(tid1, NULL);
  	pthread_join(tid2, NULL);
  	pthread_join(tid3, NULL);
  
    clock_gettime(CLOCK_MONOTONIC, &fim_prog);
  	tempo_final = GetMS(inicio_prog, fim_prog);
	printf("\n");
	
	return 0;
}



