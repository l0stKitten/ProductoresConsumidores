#ifndef PRODUCTOR_H
#define PRODUCTOR_H

#include "MonitorBuffer.h"
#include <thread>

using namespace std;
class Productor {
	private:
		MonitorBuffer* monitor;
		thread t;
		int nomPro;

		//función que ejecuta el thread
		void run_thread() {
			monitor -> addtoQueue(nomPro);
		}

	public:
		Productor (int nom, MonitorBuffer* mon);
		void join_thread ();
};

#endif