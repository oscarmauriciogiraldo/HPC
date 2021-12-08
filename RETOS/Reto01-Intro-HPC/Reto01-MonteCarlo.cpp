/*
  ============================================================================
  Name        : Calculo PI Monte Carlo
  Author      : Oscar Mauricio Giraldo Herrera
  Version     : 5
  Description : RETO 01 OPTIMIZACIÓN EN ALGORITMO MONTE CARLO Y DARTBOARD CON HILOS Y COMPARACIÓN EN C++
  ============================================================================
*/


#include <bits/stdc++.h>
#include "../timer.hh"
  
// Defines precision for x and y values. More the 
// interval, more the number of significant digits 
#define INTERVAL 1000000
using namespace std; 
double get_RandomNum(double,double);
int main() 
{ 
    int interval, i; 
    double rand_x, rand_y, origin_dist, pi; 
    int circle_points = 0, square_points = 0; 
  
    // Initializing rand() 
    srand(time(NULL)); 
  
    // Total Random numbers generated = possible x 
    // values * possible y values
    Timer t;
    for (i = 0; i < (INTERVAL ); i++) { 
        // Randomly generated x and y values 
        rand_x = get_RandomNum(1.0,-1.0);
        //cout << "aleatorio x " << rand_x;
        rand_y = get_RandomNum(1.0,-1.0); 
   		//cout << "aleatorio y " << rand_y;
        // Distance between (x, y) from the origin 
        origin_dist = rand_x * rand_x + rand_y * rand_y; 
        // Checking if (x, y) lies inside the define 
        // circle with R=1
        //cout << origin_dist;
        if (origin_dist <= 1) 
            circle_points++; 
        // Total number of points generated 
        square_points++; 
        // estimated pi after this iteration 
        
        // For visual understanding (Optional) 
        /* cout << i << rand_x << " " << rand_y << " " << circle_points 
             << " " << square_points << " - " << pi << endl << endl; */ 
    }
    pi = double(4 * circle_points) / square_points; 
    cout << "Tiempo de ejecución: " << t.elapsed() << "ms" << endl;
    // Final Estimated Value 
    cout << "\nFinal Estimation of Pi = " << pi << endl; 
  
    return 0; 
} 

double get_RandomNum(double x, double y)
{
        return x + (y - x) *  rand() * (1.0 / RAND_MAX);
}