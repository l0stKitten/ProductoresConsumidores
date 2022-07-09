#include "Consumidor.h"

Consumidor::Consumidor (int nom, /*queue<char>* buff,*/ MonitorBuffer* mon){
	nomCon = nom;
	//cola = buff;
	monitor = mon;
	t = thread(&Consumidor::run_thread, this);
}

//unir los threads
void Consumidor::join_thread () {
	t.join();
}