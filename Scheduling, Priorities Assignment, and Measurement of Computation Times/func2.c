/* Miguel Varandas - 20162198982 e Telmo Cunha - 2018308321 */

#include "func2.h"
#include <stdio.h>
#define NSECTSEC 1000000000

float f1(int CLASS,int GROUP){

	struct timespec tempo,tempo2,inicio,final;
	tempo2.tv_sec=(int)0;
	tempo2.tv_nsec=(float)(0.029*NSECTSEC); //guarda o tempo em que tem de terminar a função
	clock_gettime(CLOCK_MONOTONIC,&inicio);
	clock_gettime(CLOCK_MONOTONIC,&tempo); //mede o tempo


	tempo2.tv_sec=tempo.tv_sec+tempo2.tv_sec+(tempo.tv_nsec+tempo2.tv_nsec)/NSECTSEC;//soma os segundos e os nano segundos
	tempo2.tv_nsec=(tempo.tv_nsec+tempo2.tv_nsec)%NSECTSEC; //soma só os nano segundos


	double decorrido;
	decorrido=(double)(tempo2.tv_nsec-tempo.tv_nsec)/NSECTSEC; //está a resetar o tempo que passou antes da função, guardando o tempo inicial da função em decorrido
	decorrido=decorrido+(double)(tempo2.tv_sec-tempo.tv_sec);


	while(decorrido>0){  //enquanto o tempo medido for menor que tempo esperado continua a "executar" a função
	clock_gettime(CLOCK_MONOTONIC,&tempo); //mede o tempo que passou em tempo
	decorrido=(double)(tempo2.tv_nsec-tempo.tv_nsec)/NSECTSEC; //subtrai o tempo que mediu ao tempo esperado para terminar e guarda em decorrido
	decorrido=decorrido+(double)(tempo2.tv_sec-tempo.tv_sec);
	}


	clock_gettime(CLOCK_MONOTONIC,&final); //guarda o tempo em que acabou a função
	double tempofunc;
	tempofunc=(double)(final.tv_nsec-inicio.tv_nsec)/1000000000;    // tira o tempo inicial ao tempo em que acabou para ficar só com a duração da função
	tempofunc+=(double)(final.tv_sec-inicio.tv_sec);
	return tempofunc; //retorna a duração da tarefa
	

}
float f2(int CLASS,int GROUP){

	struct timespec tempo,tempo2,inicio,final;
	tempo2.tv_sec=(int)0;
	tempo2.tv_nsec=(float)(0.047*NSECTSEC); //guarda o tempo em que tem de terminar a função
	clock_gettime(CLOCK_MONOTONIC,&inicio);
	clock_gettime(CLOCK_MONOTONIC,&tempo); //mede o tempo


	tempo2.tv_sec=tempo.tv_sec+tempo2.tv_sec+(tempo.tv_nsec+tempo2.tv_nsec)/NSECTSEC;//soma os segundos e os nano segundos
	tempo2.tv_nsec=(tempo.tv_nsec+tempo2.tv_nsec)%NSECTSEC; //soma só os nano segundos


	double decorrido;
	decorrido=(double)(tempo2.tv_nsec-tempo.tv_nsec)/NSECTSEC; //está a resetar o tempo que passou antes da função, guardando o tempo inicial da função em decorrido
	decorrido=decorrido+(double)(tempo2.tv_sec-tempo.tv_sec);


	while(decorrido>0){  //enquanto o tempo medido for menor que tempo esperado continua a "executar" a função
	clock_gettime(CLOCK_MONOTONIC,&tempo); //mede o tempo que passou em tempo
	decorrido=(double)(tempo2.tv_nsec-tempo.tv_nsec)/NSECTSEC; //subtrai o tempo que mediu ao tempo esperado para terminar e guarda em decorrido
	decorrido=decorrido+(double)(tempo2.tv_sec-tempo.tv_sec);
	}


	clock_gettime(CLOCK_MONOTONIC,&final); //guarda o tempo em que acabou a função
	double tempofunc;
	tempofunc=(double)(final.tv_nsec-inicio.tv_nsec)/1000000000;    // tira o tempo inicial ao tempo em que acabou para ficar só com a duração da função
	tempofunc+=(double)(final.tv_sec-inicio.tv_sec);
	return tempofunc; //retorna a duração da tarefa
	

}
float f3(int CLASS,int GROUP){

	struct timespec tempo,tempo2,inicio,final;
	tempo2.tv_sec=(int)0;
	tempo2.tv_nsec=(float)(0.079*NSECTSEC); //guarda o tempo em que tem de terminar a função
	clock_gettime(CLOCK_MONOTONIC,&inicio);
	clock_gettime(CLOCK_MONOTONIC,&tempo); //mede o tempo


	tempo2.tv_sec=tempo.tv_sec+tempo2.tv_sec+(tempo.tv_nsec+tempo2.tv_nsec)/NSECTSEC;//soma os segundos e os nano segundos
	tempo2.tv_nsec=(tempo.tv_nsec+tempo2.tv_nsec)%NSECTSEC; //soma só os nano segundos


	double decorrido;
	decorrido=(double)(tempo2.tv_nsec-tempo.tv_nsec)/NSECTSEC; //está a resetar o tempo que passou antes da função, guardando o tempo inicial da função em decorrido
	decorrido=decorrido+(double)(tempo2.tv_sec-tempo.tv_sec);


	while(decorrido>0){  //enquanto o tempo medido for menor que tempo esperado continua a "executar" a função
	clock_gettime(CLOCK_MONOTONIC,&tempo); //mede o tempo que passou em tempo
	decorrido=(double)(tempo2.tv_nsec-tempo.tv_nsec)/NSECTSEC; //subtrai o tempo que mediu ao tempo esperado para terminar e guarda em decorrido
	decorrido=decorrido+(double)(tempo2.tv_sec-tempo.tv_sec);
	}


	clock_gettime(CLOCK_MONOTONIC,&final); //guarda o tempo em que acabou a função
	double tempofunc;
	tempofunc=(double)(final.tv_nsec-inicio.tv_nsec)/1000000000;    // tira o tempo inicial ao tempo em que acabou para ficar só com a duração da função
	tempofunc+=(double)(final.tv_sec-inicio.tv_sec);
	return tempofunc; //retorna a duração da tarefa
	

}
