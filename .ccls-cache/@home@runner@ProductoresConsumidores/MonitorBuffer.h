#ifndef BUFFER_H
#define BUFFER_H

#include<queue>
#include<mutex>
#include<condition_variable> 
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<unistd.h>

using namespace std;

class MonitorBuffer {
  private:
    //declaración de variables compartidas
    bool bufferListoC = false;
    char letras[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    //Tiene que ser true para que el productor produsca al inicio
    bool bufferListoP = true; 
    queue <char> buffer;
    mutex flag;
    condition_variable notEmpty, notFull;

    //contador de desperdicio cuando no hay espacio 
    //en el buffer por productor
    int desperdicioB [5] = {0, 0, 0, 0, 0}; 
    //contador de desperdicio cuando está vacío el buffer
    int desperdicioV [5] = {0, 0, 0, 0, 0};

    //Elementos producidos
    int eleProdu;
    //Elementos consumidos
    int eleConsu;

    //Cantidad Máxima de Buffer
    int queueFull;

  public:
    void addtoQueue (int nomPro);
    void popofQueue (int nomCon);
    MonitorBuffer (int full);

    //función para imprimir los contador de consumo y producción
    void printContadores (int pro, int cons);
    
    //función para imprimir los desperdicios
    void printDesperdicio (int nombre, bool proOcon);
    
};
#endif