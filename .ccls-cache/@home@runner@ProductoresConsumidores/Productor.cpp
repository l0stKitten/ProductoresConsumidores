#include "Productor.h"

Productor::Productor (int nom, /*queue<char>* buff,*/ MonitorBuffer* mon){
	nomPro = nom;
	//cola = buff;
	monitor = mon;
	t = thread(&Productor::run_thread, this);
}

//unir los threads
void Productor::join_thread () {
	t.join();
}