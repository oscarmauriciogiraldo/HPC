#include <iostream>
#include <cmath>
#include <cstdlib>
#include <omp.h>
#include ".../timer.hh"

#define NUMDARTS 1000000
using namespace std;
double get_RandomNum(double,double);
void dartBoard(int &numHits, int &cont, int minRange, int maxRange)
{
    double x_cor = 0;
    double y_cor = 0;
    double x_sum = 0;
    double y_sum = 0;
    double x_y_total = 0;
    if (NUMDARTS != 0)
    {
        for (int i = minRange; i < maxRange; i++)
        {
            x_cor = get_RandomNum(0,1.0);
            y_cor = get_RandomNum(0,1.0);
            x_sum = (x_cor * x_cor);
            y_sum = (y_cor * y_cor);
            x_y_total = (x_cor * x_cor) + (y_cor * y_cor);
            if (x_y_total < 1)
            {
                numHits++;
            }
            else
            {
                cont++;
            }
        }
    }
}

double get_RandomNum(double x, double y)
    {
        return x + (y - x) *  rand() * (1.0 / RAND_MAX);
    }
    
int main()
{
    int numHits = 0;
    int cont = 0;
    double piAprox = 0;
    int cantidadHilos, cantidadMuestra;
    cout << "Ingrese la cantidad de hilos: " << endl;
    cin >> cantidadHilos;
    cantidadMuestra = round(NUMDARTS / cantidadHilos);
    int minRange = 0;
    int maxRange = cantidadMuestra;
    Timer t;
    #pragma omp parallel for num_threads(cantidadHilos)
    for (size_t i = 0; i < cantidadHilos; i++)
    {
        dartBoard(ref(numHits), ref(cont), minRange, maxRange);
        minRange += cantidadMuestra;
        if (i == (cantidadHilos - 1))
        {
            maxRange = NUMDARTS;
        }
        else
        {
            maxRange += cantidadMuestra;
        }
    }
    cout << "Tiempo de ejecution: " << t.elapsed() << "ms" << endl;
    piAprox = double(4 * numHits) / NUMDARTS;
    cout << " El valor de pi es: " << piAprox << endl;
    cout << cont << endl;
    return 0;
}