/* *********************************************************************************

 ITESM Querétato
 Olivia Araceli Morales Quezada A01707371
 Ricardo Núñez Alanís A01703259
 Cristian Leilael Rico Espinosa A01707023

 Basado en el Trabajo de Paul Rehkugler (https://github.com/paulrehkugler)
	Repositorio: https://github.com/paulrehkugler/Baboons

********************************************************************************* */

#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <pthread.h>
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;


#define CROSSING_TIME 1	// Segundos para cruzar la cuerda
#define AVERAGE_CREATION_TIME 3	// Promedio en segundos en los que los babuinos se crean en uno de los extremos
#define BABOONS_TO_CROSS 20	// Número total de babuinos.

/* Creación de Semáforos */

sem_t crossers_mut;											// Controla el acceso de los babuinos que cruzarán
sem_t rope_mut;												// Controla el acceso a la cuerda
sem_t write_mut;											// Gracias a este, se imprime en pantalla quiénes están cruzando la cuerda
int crossers = 0;											// Número de babuinos generados en el este, en caso de ser número negativo, se indicarían a los babuinos del oeste.

void cross_rope(int crosser){
	string direction;
	if (crosser > 0)
		direction = "Eastbound";
	else{
		direction = "Westbound";
		crosser = -crosser;
	}
	sem_wait(&write_mut);									// Apunta al semáforo de escritura
	cout << direction << " baboon crossing the rope...\t" << crosser << " " << direction << " baboon(s) are currently crossing." << endl;
	sem_post(&write_mut);
	sleep(CROSSING_TIME);									// Cruce de cuerda
}

void *eastbound_baboon(void*){
	bool done = false;
	int crossers_copy;
	while (!done){
		sem_wait(&crossers_mut);							// Apunta a los babuinos que cruzarán
		if (crossers >= 0){
			++crossers;										// Aumenta el número de babuinos del este en caso de que la cuerda les dé el flujo o esté desocupada.
			crossers_copy = crossers;						// Se guarda el número de babuinos a cruzar para después guardar 
			if (crossers == 1)
				sem_wait(&rope_mut);						// Acceso exclusivo a la cuerda en caso de ser el primer babuino a cruzar.
			sem_post(&crossers_mut);						// Quita el acceso a la cuerda.
			cross_rope(crossers_copy);						// Los babuinos anteriormente guardados cruzan la cuerda.
			sem_wait(&crossers_mut);						// Nuevamente accede a los que cruzan, una vez que los babuinos en la cuerda hayan cruzado.
			--crossers;										// El número de babuinos a cruzar decrece.
			if (crossers == 0)
				sem_post(&rope_mut);						// En caso de que quede un último babuino, quitarle el acceso a la cuerda
			done = true;									// Detiene el ciclo
		}
		else
			sleep(0);
		sem_post(&crossers_mut);
	}
	return 0;
}

/* Función similar a la anterior, solamente que para los babuinos del oeste */

void *westbound_baboon(void*){
	bool done = false;
	int crossers_copy;
	while (!done){
		sem_wait(&crossers_mut);
		if (crossers <= 0){
			--crossers;
			crossers_copy = crossers;
			if (crossers == -1)
				sem_wait(&rope_mut);
			sem_post(&crossers_mut);
			cross_rope(crossers_copy);
			sem_wait(&crossers_mut);
			++crossers;
			if (crossers == 0)
				sem_post(&rope_mut);
			done = true;
		}
		else
			sleep(0);
		sem_post(&crossers_mut);
	}
	return 0;
}


int main(){

	/* Inicialización de Mutexs */
	sem_init(&crossers_mut, 0, 1);
	sem_init(&rope_mut, 0, 1);
	sem_init(&write_mut, 0, 1);

	pthread_t baboons[BABOONS_TO_CROSS];
	srand(time(NULL));

	/* 
		Se crea vector con la dirección en la que se generarán los babuinos;
		los datos dentro del vector cambiarán de lugar de manera aleatoria con la función shuffle.
	*/

	vector<int> direction = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
 
    random_device rd;
    mt19937 g(rd());
 
    shuffle(direction.begin(), direction.end(), g);
 
    copy(direction.begin(), direction.end(), ostream_iterator<int>(std::cout, " "));

	// Se crean babuinos
	for (int i = 0; i < BABOONS_TO_CROSS; ++i){
		int thinking_time = (rand() % 5);
															// espera un lapso de tiempo random para crear un babuino
		sleep(thinking_time);
		
		if (direction[i] > 10){								// Crea un babuino en el este en caso de que el número del vector "direction" en su i-ésima posición sea mayor a 10.
			pthread_create(&baboons[i], NULL, eastbound_baboon, (void *)i);
		}
		else												// Crea un babuino en el oeste.
			pthread_create(&baboons[i], NULL, westbound_baboon, (void *)i);
	}

	// Se juntan los hilos para asegurar que los últimos babuinos crucen la cuerda.
	for (int i = 0; i < BABOONS_TO_CROSS; ++i){
		pthread_join(baboons[i], NULL);
	}
}
