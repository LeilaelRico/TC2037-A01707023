/**************************************************
ITESM Campus Querétaro
Olivia Araceli Morales Quezada
Cristian Leilael Rico Espinosa
**************************************************/

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"
#include <math.h>

using namespace std;


const int SIZE = 5000000; //5e9
const int THREADS = 8;


/* ======================== Ejecución Secuencial ======================== */

bool esPrimo (int var){
    if (var < 2)
        return false;
    for (int i = 2; i <= sqrt(var); i++) {
        if (var % i == 0) {
            return false;
        }
    }
    return true;
}


double sumaPrimo (int size) {
    double acum = 0;

    for (int i = 0; i <= size; i++) {
        if(esPrimo(i) == true){
        acum = acum + i;
        }
    }
    return acum;
}

/* ======================================================================= */


/* ======================== Ejecución en Paralelo ======================== */

typedef struct {
  int start, end; // [start, end)
  int *arr;
} Block;

void* parsumPrim(void* param) {

  double *acum;
  Block *block;
  int i;


  block = (Block *) param;
  acum =  new double;
  (*acum) = 0;

  for (i = block->start; i < block->end; i++) {
    if(esPrimo(i) == true){
        (*acum) = (*acum) + i;
    }

  }
  return ( (void**) acum );


}

/* ======================================================================= */

int main (int argc, char* argv[]) {

    
    int *a, i, block_size;
    double tiempoP, tiempoS, resultadoP, resultadoS, *acum;

    Block blocks[THREADS];
    pthread_t tids[THREADS];

    a = new int[SIZE];
    //fill_array(a, SIZE);
    //display_array("a", a);

    /* ===================== Llamada de función secuencial ===================== */
    
    start_timer();
    resultadoS = sumaPrimo(SIZE);
    tiempoS = stop_timer();
    cout << "\n==================================================================" << endl;
    cout << "Resultado en Secuencial = " << setprecision(5) << resultadoS << "\n";
    cout << "Tiempo Promedio =  " << setprecision(5) << ( tiempoS / N ) << "\n";

    /* ========================================================================= */

    /* ===================== Llamada de función en paralelo ==================== */


    block_size = SIZE / THREADS;
    for (i = 0; i < THREADS; i++) {
        blocks[i].arr = a;
        blocks[i].start = i * block_size;
        if (i != (THREADS - 1)) {
            blocks[i].end = (i + 1) * block_size;
        } else {
            blocks[i].end = SIZE;
        }
    }
    start_timer();
    tiempoP = 0;
    resultadoP = 0;
     
     for (i = 0; i < THREADS; i++) {
         pthread_create(&tids[i], NULL, parsumPrim, (void*) &blocks[i]);
     }
     
     for (i = 0; i < THREADS; i++) {
         pthread_join(tids[i], (void**) &acum);
         resultadoP += (*acum);
         delete acum;
     }
    
    tiempoP = stop_timer();
    cout << "\n******************************************************************" << endl;
    cout << "Resultado en Paralelo = " << setprecision(5) << resultadoP << "\n";
    cout << "Tiempo Promedio =  " << setprecision(5) << ( tiempoP / N ) << "\n";

    delete [] a;

    /* ================================================================ */

    /* ===================== Llamada de función en paralelo ==================== */

    double speedup = tiempoS / tiempoP;
    cout << "\n====**********************************************************====" << endl;
    cout << "Speedup =  " << setprecision(5) << speedup << "\n";


    /* ================================================================ */

    return 0;
}
