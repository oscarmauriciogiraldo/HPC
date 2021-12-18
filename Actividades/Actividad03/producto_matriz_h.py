import threading
import time
import datetime
import sys
from timeit import default_timer
from random import *

matrz_r = []

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

def multiplicacion(t,fl,a,b):

    matrz_r.append([])

    f = fl
    c = 0
    lst_f = [] #Lista fila

    while (c<t):
        n = 0 #Numero resultado de la suma
        m = [] #Resultados de multiplicaciones
        ac = 0
        bc = 0
        bf = 0
        while (ac<t):
            m.append(a[f][ac]*b[bf][c])
            ac = ac + 1
            bf = bf + 1

        for i in m:
            n = n + i

        lst_f.append(n)

        c = c + 1

    matrz_r[f].append(lst_f)

    return lst_f


if __name__ == "__main__":
    tam = int(sys.argv[1])

    matriz_1 = def_matrices(tam)
    matriz_2 = def_matrices(tam)

    print("\nMatriz # 1\n")
    for i in matriz_1:
        print(i)

    print("\nMatriz # 2\n")
    for i in matriz_2:
        print(i)

    inicio = default_timer() #Captura de tiempo

    hilos = []
    i = 0
    while (i<tam):
        h = threading.Thread(name="hl", target=multiplicacion, args=(tam, i, matriz_1, matriz_2))
        hilos.append(h)

        h.start()
        h.join()

        i = i + 1

    fin = default_timer() #Captura de tiempo

    print("\nProducto\n")
    for i in matrz_r:
        print(i)

    print("\nTiempo de Usuario: " + str(fin - inicio))
