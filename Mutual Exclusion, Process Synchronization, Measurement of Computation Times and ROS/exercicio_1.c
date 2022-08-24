#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>


double maximo_x, maximo_y, maximo_z;
double minimo_x, minimo_y, minimo_z;
double media_x, media_y, media_z;
double desvio_x, desvio_y, desvio_z;


typedef struct{
	double *x, *y, *z;
  	int npoints;
}dados;
  

dados *leitura_ficheiro(char *nomeficheiro){

	dados *coordenada;
	coordenada = (dados*)malloc(sizeof(dados));
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

}

int main(){

	char *file_1 = "point_cloud1.txt";
	char *file_2 = "point_cloud2.txt";
	char *file_3 = "point_cloud3.txt";
	
	leitura_ficheiro(file_1);
	leitura_ficheiro(file_2);
	leitura_ficheiro(file_3);

	return 0;
}
