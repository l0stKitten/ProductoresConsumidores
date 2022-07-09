#include "MonitorBuffer.h"
#include "MonitorBuffer.cpp"
#include "Consumidor.h"
#include "Consumidor.cpp"
#include "Productor.h"
#include "Productor.cpp"

int main(void) {

	//inicializamos el número de consumidores
	int numCon;
	cout << "¿Cuántos consumidores necesitas?" << endl;
	cin >> numCon;
	cout << "Creando consumidores: " << numCon << endl;

	//inicializamos el número de productores
	int numProd;
	cout << "¿Cuántos productores necesitas?" << endl;
	cin >> numProd;
	cout << "Creando productores: " << numProd << endl;

  //declaramos la clase monitor
  MonitorBuffer* momo = new MonitorBuffer(5);
  
	//declaramos la clase productor
	Productor* productores[numProd];
  //declaramos array de consumidores
	Consumidor* consumidores[numCon];


	int i;
	//iniciamos los threads de los productores
	for (i=0; i < numProd; i++){
	  productores[i] = new Productor(i, momo);
	}

	//iniciamos los threads de los consumidores
	for (i=0; i < numCon; i++){
	  consumidores[i] = new Consumidor(i, momo);
	}

	//establecemos que los threads se unan
	for (i=0; i < numCon; i++){
		consumidores[i] -> join_thread();
	}
	//establecemos que los threads se unan
	for (i=0; i < numProd; i++){
		productores[i] -> join_thread();
	}

	return 0;
}