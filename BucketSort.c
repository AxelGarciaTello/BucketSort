#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

//Estructura para las canastas
typedef struct canasta{
	int numeros[3500];
} CANASTA;

//Hilo de ordenamiento (INSERTION SORT)
void *ordenamiento(void *arg){
	int i = 0,
			j = 0,
			k = 0;
	CANASTA *canastaP;
	canastaP = (CANASTA *)arg;
	for(i=1; i<3500; i++){
		j = i;
		while(j>=0 && (canastaP->numeros[j])<(canastaP->numeros[j-1])){
			k = (canastaP->numeros[j]);
			(canastaP->numeros[j]) = (canastaP->numeros[j-1]);
			(canastaP->numeros[j-1]) = k;
			j--;
		}
	}
	pthread_exit(NULL);
}

//Escribe en un archivo de texto los resultados
void escribirArchivo(int numeros[3500], CANASTA *canastas, int noHilos){
  FILE* fp;
  int i,
  	  j;
  fp=fopen("Numeros.txt","w");
  if(fp==NULL)
    exit(-1);

	//Imprimimos los números a lazar
  fprintf(fp, "Los números a lazar son:\n");
  for(i=0; i<3500; i++){
    fprintf(fp, "%d\t", numeros[i]);
  }
  fprintf(fp, "\n\n\n");

	//Imprimimos las canastas
  fprintf(fp, "\n\nLas canasta quedarian inicializadas con:\n");
  for(i=0; i<noHilos; i++){
  	fprintf(fp,"Números de la canasta %d:\n", i);
  	for(j=0; j<3500; j++){
			if((*(canastas+i)).numeros[j]!=0){
	  			fprintf(fp, "%d\t", (*(canastas+i)).numeros[j]);
			}
  	}
		fprintf(fp, "\n\n");
  }
  fclose(fp);
}

//Imprimimos los números de las canastas una vez ordenados
void escribirArchivoOrdenado(CANASTA *canastas, int noHilos){
	FILE* fp;
	int i,
			j;
	fp=fopen("Numeros.txt","a");
	if(fp==NULL)
		exit(-1);

	fprintf(fp, "\nLas canastas ordenadas son:\n");
	for(i=0; i<noHilos; i++){
		fprintf(fp, "Números de la canasta %d:\n", i);
		for(j=0; j<3500; j++){
			if((*(canastas+i)).numeros[j]!=0){
				fprintf(fp, "%d\t", (*(canastas+i)).numeros[j]);
			}
		}
		fprintf(fp, "\n\n");
	}
	fclose(fp);
}

//Programa principal
int main(void) {
  srand(time(NULL));
  int numeros[3500],
      i,
      j,
      noHilos=0,
      rango=0,
      rangoPrincipal=0,
      rangoAnterior=0,
      *contadores,
			error=0;
  CANASTA *canastas=NULL;
	pthread_t *hilos;

	//Generamos los números random
  for(i=0; i<3500; i++){
    numeros[i]= rand()%1000;
  }
	//Pedimos los hilos a ejecutar y apartamos memoria
  printf("¿Cuántos hilos quieres ejecutar?\n");
  scanf("%d", &noHilos);
  rango=1000/noHilos;
  rangoPrincipal=rango;
	hilos = (pthread_t *) malloc(noHilos*sizeof(pthread_t));
  canastas = (CANASTA *) malloc(noHilos*sizeof(CANASTA));
  contadores = (int *) malloc(noHilos*sizeof(int));

	//Separa los números en las canastas correspondientes
  for(i=0; i<noHilos; i++){
  	*(contadores+i)=0;
  	for(j=0; j<3500; j++){
  	  	(*(canastas+i)).numeros[j]=0;
  		if(numeros[j]<rango && numeros[j]>=rangoAnterior){
  			(*(canastas+i)).numeros[(*(contadores+i))]=numeros[j];
  			(*(contadores+i))++;
  		}
  	}
		rangoAnterior=rango;
  	rango+=rangoPrincipal;
  }

	//Imprimimos las canastas
  escribirArchivo(numeros, canastas, noHilos);

	//Generamos los hilos
	for(i=0; i<noHilos; i++){
		error = pthread_create((hilos+i), NULL, ordenamiento, (canastas+i));
		if(error){
			fprintf(stderr,"Error %d\n", error);
			exit(-1);
		}
	}

	//Esperamos por la finalización de los hilos
	for(i=0; i<noHilos; i++){
		error=pthread_join(*(hilos+i), NULL);
		if(error){
			fprintf(stderr,"Error %d\n", error);
		}
		else{
			printf("Hilo %d terminado\n", i);
		}
	}

	//Imprimimos las canastas ordenadas
	escribirArchivoOrdenado(canastas, noHilos);
  return 0;
}
