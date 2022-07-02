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

//función para imprimir la cola
void printQueue(queue<char> cola);

//mutex en para el buffer
mutex flag;

class Monitor {
		public:
			void popofQueue (queue<char>* cola, int nomCon){
				flag.lock();
				int numRandom;

				for (int i = 1; true; i++){
					numRandom = rand() % 26;
					char letra = letras[numRandom];
					printf ("(%d) Consumidor-%d estoy consumiendo : %c \n", i, nomCon, cola->front());
					cola -> pop();
					cout << "Tamaño de la cola " << cola -> size() << endl;
					sleep(1);
				}
				flag.unlock();
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

int main(void){

	//inicializamos el número de consumidores
	int numCon;
	cout << "¿Cuántos consumidores necesitas?" << endl;
	cin >> numCon;
	cout << numCon << endl;
	
	//declaramos array de consumidores
	Consumidor* consumidores[numCon];

	//declaramos la clase monitor
	Monitor* mo;

	int i;
	
	//declaración de la cola
	queue<char> buffer;

	for (i=0; i < sizeof(letras); i++){
		buffer.push(letras[i]);
	}

	//iniciamos los threads de los consumidores
	for (i=0; i < numCon; i++){
		consumidores[i] = new Consumidor(i, &buffer, mo);
	}

	//establecemos que los threads se unan
	for (i=0; i < numCon; i++){
		consumidores[i] -> join_thread();
	}

	return 0;
}

void printQueue(queue<char> q){
	while (!q.empty()){
		cout << " " << q.front();
		q.pop();
	}
}
