#include<stdio.h>
#include<thread>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
#include<queue>
#include<mutex>

using namespace std;

mutex flagBuffer;

class Monitor {
	public:
		void addtoQueue (queue<char>* cola, int nomPro, char letras []){
			flagBuffer.lock();
			//queue<char> cola = (queue<char> *) arg;
			int numRandom;
			for (int i = 1; true; i++){
				//queue<char> cola = (queue<char> *)arg;
				numRandom = rand() % 26;
				char letra = letras[numRandom];
				cola -> push(letra);
				printf ("(%d) Productor-%d estoy produciendo : %c \n", i, nomPro, letra);
				//printQueue(&cola);
				cout << "Tamaño de la cola " << cola -> size() << endl;
				sleep(1);
			}
			flagBuffer.unlock();
		}

		void popofQueue (queue<char>* cola, int nomCon){
			flagBuffer.lock();
			int numRandom;

			for (int i = 1; true; i++){
				//numRandom = rand() % 26;
				//char letra = letras[numRandom];
				printf ("(%d) Consumidor-%d estoy consumiendo : %c \n", i, nomCon, cola->front());
				cola -> pop();
				cout << "Tamaño de la cola " << cola -> size() << endl;
				sleep(1);
			}
			flagBuffer.unlock();
		}
};


