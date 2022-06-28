#include<iostream>
#include<thread>
#include<mutex>

CFLAGS += -std=c++11

using namespace std;

#define NUM_HILOS 10

int cont = 0;
mutex flag;

class Monitor{
	public:
	void inc(){
		flag.lock();
		for (int i = 0; i < 10; i++){
			cont++;
		}
		flag.unlock();
	}
};

class Hilo {
	private:
		Monitor* monitor;
		thread t;
		void run_thread(){
			monitor->inc();
			cout<<cont<<endl;
		}
	public:
		Hilo(){
			monitor = new Monitor;
			t = thread(&Hilo::run_thread, this);
		}
		/*void run(){
			//llamamos monitor para incrementar la variable
			//hay que instanciar el monitor
			monitor->inc();
			cout<<cont<<endl;
			
		}*/
		void join_thread(){
			t.join();
		}
};

int main(){
	
	Hilo* hilos[NUM_HILOS];
	
	int i;
	for (i = 0; i < NUM_HILOS; i++){
		hilos[i] =  new Hilo;
	}

	for (i = 0; i < NUM_HILOS; i++){
		hilos[i] -> join_thread();
	}
	return 0;
}
