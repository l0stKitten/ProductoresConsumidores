#include<stdio.h>
#include<thread>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
#include<queue>

using namespace std;

//letras a producir
const char letras[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

//función para imprimir la cola
void printQueue(queue<char> cola);

class Consumidor {
	private:
		thread t;
		int nomCon;
		//queue<char> *cola;

		//función que ejecuta el thread
		void run_thread() {
			//queue<char> cola = (queue<char> *)arg;
			int numRandom;
			for (int i = 1; true; i++){
				numRandom = rand() % 26;
				char letra = letras[numRandom];
				/*cola.push(letra);*/
				printf ("(%d) Consumidor-%d estoy consumiendo : %c \n", i, nomCon, letra);
				/*printQueue(cola);*/
				sleep(1);
			}
		}
	public:
		Consumidor (int nom/*, queue<char> &buff*/){
			nomCon = nom;
			/*cola = buff;*/
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

	int i;
	
	//declaración de la cola
	//queue<char> buffer;

	//iniciamos los threads de los consumidores
	for (i=0; i < numCon; i++){
		consumidores[i] = new Consumidor(i/*, &buffer*/);
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
