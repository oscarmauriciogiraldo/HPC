import threading
import time
import datetime
import sys
from timeit import default_timer
from random import *

def def_matrices(tam):
    #-------Creando Matriz #1-------
    mtrz_1 = []
    f = 0
    while (f<tam):
        mtrz_1.append([])
        c = 0
        while (c<tam):
            n = randint(0,9)
            mtrz_1[f].append(n)
            c = c + 1
        f = f + 1

    return mtrz_1

def multiplicacion(t,a,b):
    mtz = []

    f = 0
    while (f<t):
        c = 0
        mtz.append([])
        while (c<t):
            m = []
            ac = 0
            bc = 0
            bf = 0
            while (ac<t):
                m.append(a[f][ac]*b[bf][c])
                ac = ac + 1
                bf = bf + 1

            #mtz.append([])
            n = 0
            for i in m:
                n = n + i

            mtz[f].append(n)

            #print("\n")
            #print(str(m)+" = "+str(n))

            c = c + 1
        f = f + 1

    return mtz


if __name__ == "__main__":
    tam = int(sys.argv[1])

    matriz_1 = def_matrices(tam)
    matriz_2 = def_matrices(tam)

    print("\nMatriz #1")
    for i in matriz_1:
        print(i)

    print("\nMatriz #2")
    for i in matriz_2:
        print(i)

    inicio = default_timer() #Captura de tiempo

    matriz = multiplicacion(tam, matriz_1, matriz_2)

    fin = default_timer() #Captura de tiempo

    print("\nProducto")
    for i in matriz:
        print(i)

    print("\nTiempo de Usuario: " + str(fin - inicio))
