#ifndef CONSUMIDOR_H
#define CONSUMIDOR_H

#include "MonitorBuffer.h"
#include <thread>

class Consumidor {
	private:
		MonitorBuffer* monitor;
		thread t;
		int nomCon;

		//función que ejecuta el thread
		void run_thread() {
			monitor -> popofQueue(nomCon);
		}

	public:
		Consumidor (int nom, MonitorBuffer* mon);
		void join_thread ();
};

#endif