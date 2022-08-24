#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>


typedef struct{
	double *x, *y, *z;
  	int npoints;
  } dados;
  

dados * leitura_ficheiro(char * nomeficheiro){

	dados * coordenada;
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
    
	else{ 	
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


		read=fopen(nomeficheiro, "r");

	    while(fscanf(read, "%s %s %s\n", buffer_1, buffer_2, buffer_3) != EOF){
			coordenada->x[numero_pontos] = atof(buffer_1);
			coordenada->y[numero_pontos] = atof(buffer_2);
			coordenada->z[numero_pontos] = atof(buffer_3);
			numero_pontos++;
		}
	
		coordenada->npoints=i;

		fclose(read);
	}
	return coordenada;
}


dados * eliminar_val_negativos(dados * ficheiro, char * nomeficheiro){
	dados * matriz_reduzida;
	int i = 0;
	int contador = 0;
	matriz_reduzida = (dados*)malloc(sizeof(dados));

	//Elimina valores negativos
	matriz_reduzida->x = malloc(ficheiro->npoints * sizeof(double));
	matriz_reduzida->y = malloc(ficheiro->npoints * sizeof(double));
	matriz_reduzida->z = malloc(ficheiro->npoints * sizeof(double));

	while(i < ficheiro->npoints){	  
		if(ficheiro->x[i] > 0){
	  		matriz_reduzida->x[contador] = ficheiro->x[i];
	  		matriz_reduzida->y[contador] = ficheiro->y[i];
	  		matriz_reduzida->z[contador] = ficheiro->z[i];
	  		contador++;
	  	}
	  	i++;
	}
	
	printf("\nNome do ficheiro: %s  |  Nr. de pontos iniciais = %d  |  Nr. de pontos finais = %d\n", nomeficheiro, i, contador);

	i = 0;
	matriz_reduzida->npoints = contador;
	return matriz_reduzida;
}


int main(){

	char * file_1 = "point_cloud1.txt";
	char * file_2 = "point_cloud2.txt";
	char * file_3 = "point_cloud3.txt";
	
	dados * dados_1;
	dados * dados_2;
	dados * dados_3;

	
	dados_1 = leitura_ficheiro(file_1);
	dados_2 = leitura_ficheiro(file_2);
	dados_3 = leitura_ficheiro(file_3);

	printf("\nRemocao dos valores localizados na traseira do veiculo:\n");
	dados_1 = eliminar_val_negativos(dados_1, file_1);
	dados_2 = eliminar_val_negativos(dados_2, file_2);
	dados_3 = eliminar_val_negativos(dados_3, file_3);
	printf("\n");
	
	return 0;
}
