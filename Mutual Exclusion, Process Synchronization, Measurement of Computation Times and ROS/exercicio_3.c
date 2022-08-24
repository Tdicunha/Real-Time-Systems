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
}dados;


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


dados * eliminar_val_negativos_e_proximos(dados * ficheiro, char * nomeficheiro){
	
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

	i = 0;
	matriz_reduzida->npoints = contador;
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
	
	printf("Nr. de pontos finais = %d\n", ficheiro->npoints);
	return ficheiro;	
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

	dados_1 = eliminar_val_negativos_e_proximos(dados_1, file_1);
	dados_2 = eliminar_val_negativos_e_proximos(dados_2, file_2);
	dados_3 = eliminar_val_negativos_e_proximos(dados_3, file_3);


	printf("\nPontos finais após descartar todos os pontos desnecessarios:\n");
	dados_1 = zona_de_conducao(dados_1, file_1);
	dados_2 = zona_de_conducao(dados_2, file_2);
	dados_3 = zona_de_conducao(dados_3, file_3);
	printf("\n");

	return 0;
}
