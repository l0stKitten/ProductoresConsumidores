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
        
    /*if (!bufferListoP || buffer.size() == queueFull){
      desperdicioB[nomPro] += 1;
      cout << "+++++++++++ No hay espacio +++++++++++" << endl;
      printDesperdicio(nomPro, false);
      notFull.wait(unique_lock);
    }*/

    notFull.wait(unique_lock, [this] (){
      desperdicioB += 1;
      cout << "+++++++++++ No hay espacio +++++++++++" << endl;
      printDesperdicio(false);
      return bufferListoP != false;
    });
        
		int numRandom = rand() % 26;
		char letra = letras[numRandom];
		buffer.push(letra);
    bufferListoC = true;
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
    
    /*if (!bufferListoC || buffer.size() == 0){
      desperdicioV[nomCon] += 1;
      cout << "+++++++++++ No hay productos +++++++++++" << endl;
	  	printDesperdicio(nomCon, true);
      notEmpty.wait(unique_lock);
    }*/

    notEmpty.wait(unique_lock, [this](){
      desperdicioV += 1;
      cout << "+++++++++++ No hay productos +++++++++++" << endl;
	  	printDesperdicio(true);
      return bufferListoC != false;
    });

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

void MonitorBuffer::printDesperdicio (bool proOcon){
	if (proOcon == false){
		cout << "\n+---------------------------+" << endl;
		cout << "  despercidio Prod. "  << desperdicioB << endl;
		cout << "+---------------------------+\n" << endl;
	} else {
		cout << "\n+---------------------------+" << endl;
		cout << "  despercidio Cons. "  << desperdicioV << endl;
		cout << "+---------------------------+\n" << endl;
	}
}