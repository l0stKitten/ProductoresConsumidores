#include<iostream>

using namespace std;

#define NUM_HILOS 2

int cont = 0;

class Monitor{
	public:
	void inc(){
		for (int i = 0; i < 10; i++){
			cont++;
		}
	}


};

class Hilo {
	private:
		Monitor* monitor;
	public:
		Hilo(){
			monitor = new Monitor;
		}
		void run(){
			//llamamos monitor para incrementar la variable
			//hay que instanciar el monitor
			monitor->inc();
			cout<<cont<<endl;
		}
};

int main(){
	
	Hilo* hilos[NUM_HILOS];
	
	int i;
	for (i = 0; i < NUM_HILOS; i++){
		hilos[i] =  new Hilo;
	}

	for (i = 0; i < NUM_HILOS; i++){
		hilos[i] -> run();
	}
	return 0;
}
