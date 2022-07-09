#include "MonitorBuffer.h"

MonitorBuffer::MonitorBuffer(int full){
  queueFull = full;
}

void MonitorBuffer::addtoQueue (int nomPro){      
  for (int i = 1; true; i++){
    unique_lock<mutex> unique_lock(flag);
        
    //Produce porque la hemos bloqueado con el flag.
    if (buffer.size() == queueFull){
      bufferListoP = false;
    }
        
    if (!bufferListoP){
      desperdicioB[nomPro] += 1;
      cout << "+++++++++++ No hay espacio +++++++++++" << endl;
      printDesperdicio(nomPro, false);
      sleep(1);
      notFull.wait(unique_lock);
    }
        
		int numRandom = rand() % 26;
		char letra = letras[numRandom];
		buffer.push(letra);
		printf ("(%d) Productor-%d estoy produciendo : %c \n", i, nomPro, letra);
		cout << "\nTamaño de la cola " << buffer.size() << endl;
		eleProdu += 1;
		printContadores(eleProdu, eleConsu);
    //termina la producción

    bufferListoC = true;
		sleep(1);
    notEmpty.notify_one();
	}
}

void MonitorBuffer::popofQueue (int nomCon){
  for (int i = 1; true; i++){
		unique_lock<mutex> unique_lock(flag);

    //Consume porque la hemos bloqueado con el flag.
    if (buffer.size() == 0){
      bufferListoC = false;
    }
    if (!bufferListoC){
      desperdicioV[nomCon] += 1;
      cout << "+++++++++++ No hay productos +++++++++++" << endl;
	  	printDesperdicio(nomCon, true);
      sleep(1);
      notEmpty.wait(unique_lock);
    }

    printf ("(%d) Consumidor-%d estoy consumiendo : %c \n", i, nomCon, buffer.front());
		buffer.pop();
		cout << "Tamaño de la cola " << buffer.size() << endl;

		eleConsu += 1;
		printContadores(eleProdu, eleConsu);
        //termina de consumir

    bufferListoP = true;
    sleep(1);
    notFull.notify_one();
	}
} 

void MonitorBuffer::printContadores (int pro, int cons){
	cout << "	--------------------------------" << endl;
	cout << "	    producción: " << pro << endl;
	cout << "	    consumo: " << cons << endl;
	cout << "	--------------------------------" << endl;
}

void MonitorBuffer::printDesperdicio (int nombre, bool proOcon){
	if (proOcon == false){
		cout << "\n+---------------------------+" << endl;
		cout << "  despercidio Prod. " << nombre << ": " << desperdicioB[nombre] << endl;
		cout << "+---------------------------+\n" << endl;
	} else {
		cout << "\n+---------------------------+" << endl;
		cout << "  despercidio Cons. " << nombre << ": " << desperdicioV[nombre] << endl;
		cout << "+---------------------------+\n" << endl;
	}
}