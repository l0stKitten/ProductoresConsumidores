#include<stdio.h>
#include<thread>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
#include<queue>
#include<mutex>

using namespace std;

//letras a producir
const char letras[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

//mutex en para el buffer uno para consumidores otro para productores
mutex flagP;

mutex flagC;

mutex print;

//indicador de queue full
#define queueFull 26

//contador de elementos procesados
int eleProdu = 0;

//contador de elementos consumidos
int eleConsu = 0;

//contador de desperdicio cuando no hay espacio en el buffer por productor
int desperdicioB [] = {0, 0, 0, 0, 0};

//contador de desperdicio cuando está vacío el buffer
int desperdicioV []= {0, 0, 0, 0, 0};

//función para imprimir los contador de consumo y producción
void printContadores (int pro, int cons);

//función para imprimir los desperdicios
void printDesperdicio (int nombre, bool proOcon);

class Monitor {
	public:
		void addtoQueue (queue<char>* cola, int nomPro){

			int numRandom;
			for (int i = 1; true; i++){
				flagP.lock();
				if (cola -> size() != queueFull){
					numRandom = rand() % 26;
					char letra = letras[numRandom];
					cola -> push(letra);
					printf ("(%d) Productor-%d estoy produciendo : %c \n", i, nomPro, letra);
					cout << "Tamaño de la cola " << cola -> size() << endl;

					eleProdu += 1;
					printContadores(eleProdu, eleConsu);


					sleep(1);
				} else {
					desperdicioB[nomPro] += 1;
					cout << "--------- Buffer lleno ---------" << endl;
					printDesperdicio(nomPro, false);
					sleep(1);
				}
				flagP.unlock();
			}
		}

		void popofQueue (queue<char>* cola, int nomCon){

			for (int i = 1; true; i++){
				flagC.lock();
				if (cola -> empty() == false){
					printf ("(%d) Consumidor-%d estoy consumiendo : %c \n", i, nomCon, cola->front());
					cola -> pop();
					cout << "Tamaño de la cola " << cola -> size() << endl;

					eleConsu += 1;
					printContadores(eleProdu, eleConsu);
					sleep(1);
				} else {
					desperdicioV[nomCon] += 1;
					cout << "+++++++++++ No hay productos +++++++++++" << endl;
					printDesperdicio(nomCon, true);
					sleep(1);
				}
				flagC.unlock();
			}
		}

};

class Productor {
	private:
		Monitor* monitor;
		thread t;
		int nomPro;
		queue<char>* cola;

		//función que ejecuta el thread
		void run_thread() {
			monitor -> addtoQueue(cola, nomPro);
		}

	public:
		Productor (int nom, queue<char>* buff, Monitor* mon){
			nomPro = nom;
			cola = buff;
			monitor = mon;
			t = thread(&Productor::run_thread, this);
		}

		//unir los threads
		void join_thread () {
			t.join();
		}
};

class Consumidor {
	private:
		Monitor* monitor;
		thread t;
		int nomCon;
		queue<char> *cola;

		//función que ejecuta el thread
		void run_thread() {
			monitor -> popofQueue(cola, nomCon);
		}

	public:
		Consumidor (int nom, queue<char>* buff, Monitor* mon){
			nomCon = nom;
			cola = buff;
			monitor = mon;
			t = thread(&Consumidor::run_thread, this);
		}

		//unir los threads
		void join_thread () {
			t.join();
		}
};

int main(void) {

	//inicializamos el número de consumidores
	int numCon;
	cout << "¿Cuántos consumidores necesitas?" << endl;
	cin >> numCon;
	cout << "Creando consumidores: " << numCon << endl;

	//declaramos array de consumidores
	Consumidor* consumidores[numCon];

	//inicializamos el número de productores
	int numProd;
	cout << "¿Cuántos productores necesitas?" << endl;
	cin >> numProd;
	cout << "Creando productores: " << numProd << endl;

	//declaramos la clase productor
	Productor* productores[numProd];
	
	//declaramos la clase monitor
	Monitor* mo;

	//declaración de la cola
	queue<char> buffer;

	int i;
	//iniciamos los threads de los productores
	for (i=0; i < numProd; i++){
		productores[i] = new Productor(i, &buffer, mo);
	}


	//iniciamos los threads de los consumidores
	for (i=0; i < numCon; i++){
		consumidores[i] = new Consumidor(i, &buffer, mo);
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

void printContadores (int pro, int cons){
	//print.lock();
	cout << "	--------------------------------" << endl;
	cout << "	    producción: " << pro << endl;
	cout << "	    consumo: " << cons << endl;
	cout << "	--------------------------------" << endl;
	//print.unlock();
}

void printDesperdicio (int nombre, bool proOcon){
	if (proOcon == false){
		cout << "\n+---------------------------+" << endl;
		cout << "  despercidio Prod. " << nombre << ": " << desperdicioB[nombre] << endl;
		cout << "+---------------------------+\n" << endl;
	} else {
		cout << "\n+---------------------------+" << endl;
		cout << "  despercidio Cons. " << nombre << ": " << desperdicioV[nombre] << endl;
		cout << "+---------------------------+\n" << endl;
	}
}
