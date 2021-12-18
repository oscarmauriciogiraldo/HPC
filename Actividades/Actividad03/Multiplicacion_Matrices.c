// Multiplicacion de matrices

#include <stdio.h>

void mult_matriz (tam){
	int t,f,c; // tamaño, filas, columnas
	t = tam;
	f = 0;
	c = 0;
	
	int mtrz_1 [t][t]; // Matriz x*x
	
	printf ("Matriz #1:\n\n");
	
	int i,j; 
	i = 0;
	j = 0;
	while (f<t){
		while (c<t){
			mtrz_1[i][j] = rand() % 11;
			printf("%d\t",mtrz_1[i][j]);
			j++;
			c++;
		}
		printf("\n");
		i++;
		f++;
		c=0;
		j=0;
	}
	
	f = 0;
	c = 0;
	
	int mtrz_2 [t][t]; // Matriz x*x
	
	printf ("\nMatriz #2:\n\n");
	
	int a,b; 
	a = 0;
	b = 0;
	while (f<t){
		while (c<t){
			mtrz_2[a][b] = rand() % 11;
			printf("%d\t",mtrz_2[a][b]);
			b++;
			c++;
		}
		printf("\n");
		a++;
		f++;
		c=0;
		b=0;
	}
	
	printf ("\nProducto:\n\n");
	
	f = 0;
	c = 0;
	
	int r=0, m=0, n=0, x=0, y=0;
	
	while (f<t){
		while (c<t){
			while(n<t){
				r = r + (mtrz_1[m][n] * mtrz_2[x][y]);
				n++;
				x++;
			}
			printf ("%i\t", r);
			c++;
			r=0;
			n=0;
			y++;
			x=0;
		}
		printf("\n");
		f++;
		c=0;
		m++;
		y=0;
	}
	
}

int main (){
	int tam;
	
	printf ("\nTamanio de Matriz: ");
	scanf("%i",&tam);
	printf ("\n");
	
	mult_matriz (tam);
	
	printf ("\n");
	
	system("pause");
	return 0;
}
