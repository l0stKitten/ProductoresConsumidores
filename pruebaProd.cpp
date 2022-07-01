#include<stdio.h>
#include<thread>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<unistd.h>

using namespace std;

const char letras[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

void productor (int nomPro);

int main(void){

	//establecemos el número de productores
	int numProd;
	cout << "¿Cuántos productores necesitas?" << endl;
	cin >> numProd;
	cout << numProd << endl;

	thread productores[numProd];

	int i;

	//iniciamos los threads de los productores
	for (i=0; i < numProd; i++){
		productores[i] = thread(productor, i);
	}

	//establecemos que los threads se unan
	for (i=0; i < numProd; i++){
		productores[i].join();
	}

	return 0;
}

void productor (int nomPro) {
	int aleatorio;
	for (int i=1; true; i++) {
		aleatorio = rand() % 26;
		printf ("(%d) Productor-%d estoy produciendo : %c \n", i, nomPro, letras[aleatorio]);
		sleep(1);
	}
}
