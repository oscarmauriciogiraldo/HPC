/* C++ program for estimation of Pi using Monte 
   Carlo Simulation */
#include <bits/stdc++.h>
#include <omp.h>
#include ".../timer.hh"

// Defines precision for x and y values. More the
// interval, more the number of significant digits
#define INTERVAL 1000000
using namespace std;
double get_RandomNum(double,double);
void monteCarlo(int &circlePoints, int &squarePoints, double &pi, int minRange, int maxRange)
{
    double randX, randY, originDist;
    for (size_t i = minRange; i < maxRange; i++)
    {
        // Randomly generated x and y values
        randX = get_RandomNum(1.0,-1.0);
        //cout << "aleatorio x " << randX;
        randY = get_RandomNum(1.0,-1.0);
        //cout << "aleatorio y " << randY;
        // Distance between (x, y) from the origin
        originDist = randX * randX + randY * randY;
        // Checking if (x, y) lies inside the define
        // circle with R=1
        //cout << originDist;
        if (originDist <= 1)
            circlePoints++;
        // Total number of points generated
        squarePoints++;
        // estimated pi after this iteration
        pi = double(4 * circlePoints) / squarePoints;
        // For visual understanding (Optional)
        /*cout << i << randX << " " << randY << " " << circlePoints
             << " " << squarePoints << " - " << pi << endl
             << endl;*/
    }
}

double get_RandomNum(double x, double y)
{
        return x + (y - x) *  rand() * (1.0 / RAND_MAX);
}

int main()
{
    int interval, i;
    double pi;
    int circlePoints = 0, squarePoints = 0;
    int cantidadHilos;
    cout << "Ingrese la cantidad de hilos:" << endl;
    cin >> cantidadHilos;
    // Initializing rand()
    srand(time(NULL));
    int cantidadMuestra = round((INTERVAL) / cantidadHilos);
    int minRange = 0;
    int maxRange = cantidadMuestra;
    Timer t;
    #pragma omp parallel for num_threads(cantidadHilos)
    for (size_t i = 0; i < cantidadHilos; i++)
    {
        /* cout << "Minimo Rango: " << minRange << endl;
        cout << "Maximo Rango: " << maxRange << endl; */
        monteCarlo(ref(circlePoints), ref(squarePoints), ref(pi), minRange, maxRange);
        minRange += cantidadMuestra;
        if (i == (cantidadHilos - 1))
        {
            maxRange = INTERVAL;
        }
        else
        {
            maxRange += cantidadMuestra;
        }
    }
    cout << "Tiempo de ejecution: " << t.elapsed() << "ms" << endl;
    // Final Estimated Value
    cout << "\nFinal Estimation of Pi = " << pi << endl;

    return 0;
}