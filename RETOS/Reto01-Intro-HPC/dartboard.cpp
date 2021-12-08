#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include "../timer.hh"

#define NUMDARTS 1000000
void rand_seed();
double get_RandomNum(double,double);
using namespace std;
int main()
{
 	
    double x_cor = 0;
    double y_cor = 0;
    double x_sum = 0;
    double y_sum = 0;
    double x_y_total = 0;
    int num_hits = 0;
    double pi_approx = 0;
    int cont = 0;
    Timer t;
    srand(time(NULL)); 
    if ( NUMDARTS != 0  )
    {
        for ( int i = 0; i < NUMDARTS; i++) 
        {
        	
            x_cor = get_RandomNum(0,1.0);
            y_cor = get_RandomNum(0,1.0);
            x_sum = (x_cor * x_cor); 
			y_sum = (y_cor * y_cor);
            x_y_total = (x_cor * x_cor) + (y_cor * y_cor);
            if (x_y_total < 1)
            {
            	num_hits++;
            }else{
            	cont++;
			}
        }
    }
    pi_approx = double(4 * num_hits)  / NUMDARTS;
        cout << " El valor de pi es: " << pi_approx << endl;   
    cout << cont << endl;
    cout << "Tiempo de ejecution: " << t.elapsed() << "ms" << endl;
    return 0;
}


double get_RandomNum(double x, double y)
    {
        return x + (y - x) *  rand() * (1.0 / RAND_MAX);
    }