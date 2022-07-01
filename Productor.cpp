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

//void productor (int nomPro);

class Productor {
	private:
		thread t;
		int nomPro;
		//queue<char> *cola;

		//función que ejecuta el thread
		void run_thread() {
			//queue<char> cola = (queue<char> *)arg;
			int numRandom;
			for (int i = 1; true; i++){
				numRandom = rand() % 26;
				char letra = letras[numRandom];
				/*cola.push(letra);*/
				printf ("(%d) Productor-%d estoy produciendo : %c \n", i, nomPro, letra);
				/*printQueue(cola);*/
				sleep(1);
			}
		}
	public:
		Productor (int nom/*, queue<char> &buff*/){
			nomPro = nom;
			/*cola = buff;*/
			t = thread(&Productor::run_thread, this);
		}

		//unir los threads
		void join_thread () {
			t.join();
		}
};

int main(void){

	//inicializamos el número de productores
	int numProd;
	cout << "¿Cuántos productores necesitas?" << endl;
	cin >> numProd;
	cout << numProd << endl;
	
	//instanciamos la clase productor
	Productor* productores[numProd];

	//thread productores[numProd];

	int i;
	
	//declaración de la cola
	//queue<char> buffer;

	//iniciamos los threads de los productores
	for (i=0; i < numProd; i++){
		productores[i] = new Productor(i/*, &buffer*/);
	}

	//establecemos que los threads se unan
	for (i=0; i < numProd; i++){
		productores[i] -> join_thread();
	}

	return 0;
}

void printQueue(queue<char> q){
	while (!q.empty()){
		cout << " " << q.front();
		q.pop();
	}
}
