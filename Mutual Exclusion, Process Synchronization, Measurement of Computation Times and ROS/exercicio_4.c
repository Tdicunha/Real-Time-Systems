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


double maximo_x, maximo_y, maximo_z;
double minimo_x, minimo_y, minimo_z;
double media_x, media_y, media_z;
double desvio_x, desvio_y, desvio_z;


typedef struct{
	double *x, *y, *z;
  	int npoints;
}dados;

double GetMS(struct timespec inicio, struct timespec fim){
	
	double aux1, aux2;

	aux1 = inicio.tv_sec * 1000000000 + inicio.tv_nsec;
	aux2 = fim.tv_sec * 1000000000 + fim.tv_nsec;
	aux1  = (aux2 - aux1)/1000000.0;

	return aux1;
}


dados *leitura_ficheiro(char *nomeficheiro){

	dados *coordenada;
	coordenada = (dados*)malloc(sizeof(dados));
	int i = 0;
	int numero_pontos = 0;
	char buffer_1[32];
	char buffer_2[32];
	char buffer_3[32];


	struct timespec inicio,fim,tempo_duracao;
	double tempoiniciar, tempofim,time;
	clock_gettime(CLOCK_MONOTONIC, &inicio);
	tempoiniciar = (inicio.tv_sec);
	tempoiniciar += (inicio.tv_nsec) / 1000000000.0;



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


	coordenada->x = malloc(i * sizeof(double));
	coordenada->y = malloc(i * sizeof(double));
	coordenada->z = malloc(i * sizeof(double));
	coordenada->npoints = 0;

	read = fopen(nomeficheiro, "r");

	while(fscanf(read, "%s %s %s\n", buffer_1, buffer_2, buffer_3) != EOF){
		coordenada->x[numero_pontos] = atof(buffer_1);
		coordenada->y[numero_pontos] = atof(buffer_2);
		coordenada->z[numero_pontos] = atof(buffer_3);
		numero_pontos++;
	}

	coordenada->npoints=i;

	fclose(read);


	//Valor máximo e do valor mínimo:
	maximo_x = coordenada->x[0];
	maximo_y = coordenada->y[0];
	maximo_z = coordenada->z[0];
	minimo_x = coordenada->x[0];
	minimo_y = coordenada->y[0];
	minimo_z = coordenada->z[0];

	int contador = 0;

	while(contador < i){
		if(coordenada->x[contador] > maximo_x){
			maximo_x = coordenada->x[contador];
		}

		if(coordenada->y[contador] > maximo_y){
			maximo_y = coordenada->y[contador];
		}
				
		if(coordenada->z[contador] > maximo_z){
			maximo_z = coordenada->z[contador];
		}
				
		if(coordenada->x[contador] < minimo_x){
			minimo_x = coordenada->x[contador];
		}
				
		if(coordenada->y[contador] < minimo_y){
			minimo_y = coordenada->y[contador];
		}
				
		if(coordenada->z[contador] < minimo_z){
			minimo_z= coordenada->z[contador];
		}	

		media_x += coordenada->x[contador];
		media_y += coordenada->y[contador];
		media_z += coordenada->z[contador];		
		contador++;
	}

	//Cálculo da média:
	media_x = media_x / i;
	media_y = media_y / i;
	media_z = media_z / i;


	int contador_2 = 0;

	while(contador_2 < i){
		desvio_x += pow(coordenada->x[contador_2] - media_x, 2);
		desvio_y += pow(coordenada->y[contador_2] - media_y, 2);
		desvio_z += pow(coordenada->z[contador_2] - media_z, 2);
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
	printf("\n\tMaximo: | %lf  \n\tMinimo: | %lf \n\tMedia:  | %lf \n\tDesvio: | %lf \n", maximo_x, minimo_x, media_x, desvio_x);
	
	printf("\nValores de y:\n");
	printf("\n\tMaximo: | %lf  \n\tMinimo: | %lf \n\tMedia:  | %lf \n\tDesvio: | %lf \n", maximo_y, minimo_y, media_y, desvio_y);
	
	printf("\nValores de z:\n");
	printf("\n\tMaximo: | %lf  \n\tMinimo: | %lf \n\tMedia:  | %lf \n\tDesvio: | %lf \n", maximo_z, minimo_z, media_z, desvio_z);
	}

	clock_gettime(CLOCK_MONOTONIC, &fim);
	time=GetMS(inicio,fim);
	printf("\nTempo de execucao: %lfms\n\n",time);

	return coordenada;
}


dados * eliminar_val_negativos_e_proximos(dados * ficheiro, char * nomeficheiro){
	dados * matriz_reduzida;
	int i = 0;
	int contador = 0;
	matriz_reduzida = (dados*)malloc(sizeof(dados));
	
	struct timespec inicio,fim,tempo_duracao;
	double tempoiniciar, tempofim,time;
	clock_gettime(CLOCK_MONOTONIC, &inicio);
	tempoiniciar = (inicio.tv_sec);
	tempoiniciar += (inicio.tv_nsec) / 1000000000.0;
	
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

	printf("\nNome do ficheiro: %s  |  Nr. de pontos iniciais = %d  |  Nr. de pontos finais = %d | ", nomeficheiro, i, contador);

	i = 0;
	matriz_reduzida->npoints = contador;

	clock_gettime(CLOCK_MONOTONIC, &fim);
	time=GetMS(inicio,fim);
	printf("Tempo de execucao: %lfms\n",time);

	return matriz_reduzida;
}


dados * zona_de_conducao(dados * ficheiro, char * nomeficheiro){


	int i = 0;
	double z_min = 150;
	double z_max = -350;
	double x_axis, y_axis;
	int contador;
	int a = 0;
	int b = 0;
	  
	struct timespec inicio,fim,tempo_duracao;
	double tempoiniciar, tempofim, time;
	clock_gettime(CLOCK_MONOTONIC, &inicio);
	
	printf("\nNome do ficheiro: %s  |  Nr. de pontos iniciais = %d  | ", nomeficheiro, ficheiro->npoints);

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

	clock_gettime(CLOCK_MONOTONIC, &fim);
	time=GetMS(inicio,fim);
	printf(" Nr. de pontos finais = %d  |  Tempo de execucao: %lf ms\n", ficheiro->npoints, time);
	
	return ficheiro;
}


int main(){
	char * file_1 = "point_cloud1.txt";
	char * file_2 = "point_cloud2.txt";
	char * file_3 = "point_cloud3.txt";
	
	dados * dados_1;
	dados * dados_2;
	dados * dados_3;

	double tempoiniciar, tempofim,time;
	struct timespec iniciomain, inicio_func,fim_func;

	printf("\nTempos da funcao do exercicio 1:\n");

	dados_1 = leitura_ficheiro(file_1);
	dados_2 = leitura_ficheiro(file_2);
	dados_3 = leitura_ficheiro(file_3);

	printf("\n--------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("Tempos da funcao do exercicio 2:\n");

	dados_1 = eliminar_val_negativos_e_proximos(dados_1, file_1);
	dados_2 = eliminar_val_negativos_e_proximos(dados_2, file_2);
	dados_3 = eliminar_val_negativos_e_proximos(dados_3, file_3);

	printf("\n--------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("Tempos da funcao do exercicio 3:\n");

	dados_1 = zona_de_conducao(dados_1, file_1);
	dados_2 = zona_de_conducao(dados_2, file_2);
	dados_3 = zona_de_conducao(dados_3, file_3);

	return 0;
}
