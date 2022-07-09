# ProductoresConsumidores
![alt text](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/21690602-c77c-4975-9aba-bb6d5266af97/2.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220709%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220709T053529Z&X-Amz-Expires=86400&X-Amz-Signature=75b92260ec3f7881976b0edb6522524f4420b2f590e2f6309c0a4408cb8be73d&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%222.png%22&x-id=GetObject)

-	El productor intenta insertar datos en una ranura vacía del buffer. 
-	El consumidor intenta eliminar datos de una ranura llena del buffer. 
-	El Productor no debe insertar datos cuando el buffer está lleno. 
-	El Consumidor no debe eliminar datos cuando el buffer está vacío. 
-	El Productor y el Consumidor no deben insertar y eliminar datos simultáneamente.

## Parámetros de los Casos

-	Productores, Consumidores son eternos. El buffer es eterno solo para versión con GUI.
-	El buffer tiene funcionamiento de cola, el primero en insertarse en el primero en salir.
-	El productor y el consumidor se pueden pausar en cualquier momento, en la implem4rentación de la interfaz gráfica.
-	Mostrar en variables lo consumido y lo producido y contrastar ambas variables.
-	Mostrar el desperdicio del consumidor dado si el buffer está vacío.
-	Implementación en C++.
-	Una constante con el número de productores y consumidores:
-	**Caso 1:** En modo consola tenemos solo un productor y un consumidor. Debe evidenciar los consumido y los desperdicios que se generen. Se tendrá 1 solo productor y 3 consumidores. Estrés del Consumidor.
-	**Caso 2:** Al inicio todo está detenido. En la GUI tendremos 2 productores y 2 consumidores, desperdicio y el tamaño del buffer. Se busca primero estresar la producción y luego a los consumidores. Cada productor y consumidor tienen botones individuales.

## Implementación
### Monitor & Buffer
Los monitores nos permiten regular la sincronización de hilos. Dentro de una clase monitor tenemos los procedimientos los cuales son funciones que indican el funcionamiento de los hilos. Si el procedimiento necesita de variables externas, estas deberán estar especificadas dentro del monitor. El monitor asegura que solo 1 proceso a la vez estará activo dentro del monitor. Para lograrlo necesitamos de exclusión mutua para las zonas críticas, necesita de constructor condicional, para ello haremos uso de variables cuyas operaciones son wait() y signal(). [4]
Wait() nos indica que esperemos cuando la condición sea negativa, hasta  que tengamos  una señal para realizar las operaciones. Signal () indica que una vez terminado el proceso del hilo se enviemos una señal de aviso para que alguno o todos los hilos esperando puedan acceder y reanudar sus operaciones.
Dado que el problema será resuelto con monitores estaremos colocando todas las variables globales como las funciones de los threads en el monitor. Esto hace que el buffer, que puede ser representado como una cola o un array, pase a ser un atributo del monitor. Esto permite instanciar solo un monitor tanto para la clase productor como consumidor.
Dado que el problema será resuelto con monitores estaremos colocando todas las variables globales como las funciones de los threads en el monitor. Esto hace que el buffer, que puede ser representado como una cola o un array, pase a ser un atributo del monitor. Esto permite instanciar solo un monitor tanto para la clase productor como consumidor.

####	Librerías:
Queue nos permitirá usar colas, mutex para la exclusión, condition_variable para crear variables condicionales, time y unstd que nos permita usar funciones como sleep en Linux, y como siempre añadiremos stdio, stdlib y iostream.

```C++
#include<queue>
#include<mutex>
#include<condition_variable> 
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
```

####	Namespace:
```C++
using namespace std;
```

#### Atributos del monitor: 
Al tener todas las variables globales necesarias aquí colocaremos el desperdicio, el buffer, contador de elementos consumidos, variables condicionales, las letras que producirá el productor, la capacidad máxima del buffer y 2 variables tipo bool que permitirán modelar el uso del buffer.
```C++
    bool bufferListoC = false;
    char letras[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    //Tiene que ser true para que el productor produzca al inicio
    bool bufferListoP = true; 
    queue <char> buffer;
    mutex flag;
    condition_variable notEmpty, notFull;

    //contador de desperdicio cuando no hay espacio 
    //en el buffer por productor
    int desperdicioB [5] = {0, 0, 0, 0, 0}; 
    //contador de desperdicio cuando está vacío el buffer
    int desperdicioV [5] = {0, 0, 0, 0, 0};

    //Elementos producidos
    int eleProdu;
    //Elementos consumidos
    int eleConsu;

    //Cantidad Máxima de Buffer
    int queueFull;
```
#### Procedimientos del monitor: 
Aquí estará el constructor del monitor como los procedimientos de los hilos addtoQueue para el productor, popofQueue para el consumidor. También tenemos funciones para imprimir los contadores y el desperdicio.
```C++
    void addtoQueue (int nomPro);
    void popofQueue (int nomCon);
    MonitorBuffer (int full);

    //función para imprimir los contadores de consumo y producción
    void printContadores (int pro, int cons);
    
    //función para imprimir los desperdicios
    void printDesperdicio (int nombre, bool proOcon);
```
#### Header MonitorBuffer:
Al final el header para el monitor debería quedar así:
```C++
#ifndef BUFFER_H
#define BUFFER_H

#include<queue>
#include<mutex>
#include<condition_variable> 
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<unistd.h>

using namespace std;

class MonitorBuffer {
  private:
    //declaración de variables compartidas
    bool bufferListoC = false;
    char letras[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    //Tiene que ser true para que el productor produzca al inicio
    bool bufferListoP = true; 
    queue <char> buffer;
    mutex flag;
    condition_variable notEmpty, notFull;

    //contador de desperdicio cuando no hay espacio 
    //en el buffer por productor
    int desperdicioB [5] = {0, 0, 0, 0, 0}; 
    //contador de desperdicio cuando está vacío el buffer
    int desperdicioV [5] = {0, 0, 0, 0, 0};

    //Elementos producidos
    int eleProdu;
    //Elementos consumidos
    int eleConsu;

    //Cantidad Máxima de Buffer
    int queueFull;

  public:
    void addtoQueue (int nomPro);
    void popofQueue (int nomCon);
    MonitorBuffer (int full);

    //función para imprimir los contadores de consumo y producción
    void printContadores (int pro, int cons);
    
    //función para imprimir los desperdicios
    void printDesperdicio (int nombre, bool proOcon);
    
};
#endif
```
####	Implementación MonitorBuffer:
La implementación de MonitorBuffer debe considerar las declaciones previas de en el header. 
```C++
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
```
Con ello culminado podemos pasar a diseñar las demás clases

### Clase Productor
La clase productor será la encargada extraer un elemento random de la lista de letras que insertará en el buffer, esto aumentará el contador de producción. Cuando el buffer no pueda insertar más elementos debido a que no hay espacio se incrementará el desperdicio según el nombre del buffer en la lista desperdiciosB. Su funcionamiento está previamente modelado en el monitor.

####	Librerías y Namespace:
Requerirá únicamente la librería del buffer y los threads, también usa el mismo namespace del buffer.
```C++
using namespace std;
```
####  Atributos:
Los atributos principales son el thread, el monitor y el nombre del productor.
```C++
MonitorBuffer* monitor;
thread t;
int nomPro;
```
####	Funciones:
Tenemos el run, el constructor de la clase y una función join.
```C++
Productor (int nom, MonitorBuffer* mon);
void join_thread ();
void run_thread();
```
####	Header:
Para evitar problemas dentro del header tenemos la implementación de la clase run, de lo contrario nos da error dado que el monitor no ha sido declarado en ese ámbito.
```C++
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
```
####	Implementación:
```C++
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
```

### Clase Consumidor
Esta clase es la encargada de quitar el primer elemento de la cola, cada que vez consume incrementa el contador de consumo. Cuando no hay productos se incrementa el desperdicio según su nombre del consumidor en la lista desperdicioV. Su funcionamiento está previamente modelado en el monitor.

####	Librerías:
```C++
#include "MonitorBuffer.h"
#include <thread>
```
####	Atributos:
```C++
MonitorBuffer* monitor;
thread t;
int nomCon;
```
####	Funciones:
```C++
Consumidor (int nom, MonitorBuffer* mon);
void join_thread ();
void run_thread();
```
####	Header:
```C++
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
```
####	Implementación:
```C++
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
```

### Modo Consola
Para el modo en consola en un archivo cpp implementamos la clase main. En esta declaramos las todas las clases previamente construidas. Primero preguntaremos al usuario cuántos consumidor y productores quiere, según las cantidades se declararán dos arreglos tipo puntero donde guardaremos la inicialización de los productores y consumidores respectivamente.

Primero crearemos un monitor instanciado especificando la cantidad máxima que tendrá el buffer. Luego por medio de ciclos vamos instanciando las productores y consumidores dándoles como nombre el índice que ocupan en sus arreglos y como monitor el que previamente declaramos.
Finalmente hacemos que todos los hilos se unan nuevamente con ciclos. Dadas las clases anteriormente expuestas podemos iniciar con las pruebas en el main.

####	Main:

##### Librerías
```C++
#include "MonitorBuffer.h"
#include "MonitorBuffer.cpp"
#include "Consumidor.h"
#include "Consumidor.cpp"
#include "Productor.h"
#include "Productor.cpp"
```
##### Main()
```C++
int main(void) {

	//inicializamos el número de consumidores
	int numCon;
	cout << "¿Cuántos consumidores necesitas?" << endl;
	cin >> numCon;
	cout << "Creando consumidores: " << numCon << endl;

	//inicializamos el número de productores
	int numProd;
	cout << "¿Cuántos productores necesitas?" << endl;
	cin >> numProd;
	cout << "Creando productores: " << numProd << endl;

  //declaramos la clase monitor
  int maxC = 5;
	MonitorBuffer mo(maxC);
  MonitorBuffer* momo = & mo;
  
	//declaramos la clase productor
	Productor* productores[numProd];
  //declaramos array de consumidores
	Consumidor* consumidores[numCon];


	int i;
	//iniciamos los threads de los productores
	for (i=0; i < numProd; i++){
	  productores[i] = new Productor(i, momo);
	}

	//iniciamos los threads de los consumidores
	for (i=0; i < numCon; i++){
	  consumidores[i] = new Consumidor(i, momo);
	}

	//establecemos que los threads se unan
	for (i=0; i < numCon; i++){
		consumidores[i] -> join_thread();
	}
	//establecemos que los threads se unan
	for (i=0; i < numProd; i++){
		productores[i] -> join_thread();
	}

	return 0;
}
```

### Interfaz Gráfica
Con el código previo según POO podemos pasarlo a interfaz gráfica con QT, esto permitirá evidenciar el comportamiento de los hilos de forma más interactiva. El principal reto para QT es adaptar las librerías que usamos a las del formato de QT dado que esta cuenta con sus propias librerías para hilos, colas y variables condicionales. Esto supone un cambio en el código, pero no muy sustancial.
QT es el framework más conocido para poder diseñar e implementar GUI en C++. También permite trabajar por consola, está más orientado a objetos. Cuenta con una gran variedad de widgets y documentación.

#### Librerías en QT
Estas al estar más orientadas a objetos tienen otra sintaxis y algunas funciones diferentes, en particular necesitaremos la de cola, mutex y variables condicionales. En esta implementación no se pudo mantener el concepto de unique _lock, pero consultando en foros igual era posible remplazarlo con las variables mutex normales.

##### Variables condicionales
Al igual que la implementación anterior la librería permite el uso de las funciones wait, signal es reemplazado por notify pero el funcionamiento es el mismo. Para declarar una variable de este tipo debemos colocar QWaitCondition como tipo.
```C++
#include <QWaitCondition>
void	notify_one()
void	wakeOne()

QWaitCondition notEmpty, notFull;
```
##### Mutex
Al igual que en la librería anterior tenemos el funcionamiento de lock y unlock. Solo varía la declaración, colocamos QMutex como tipo.
```C++
#include <QMutex>
void	lock()
void	unlock()

QMutex flag;
```

##### Queue
En el caso de colas también cambia el tipo a QQueue, en este caso las funciones se llaman diferente, en vez de pop tenemos dequeue y en vez de front tenemos head.
```C++
#include <QQueue>
T	dequeue()
T &	head()

QQueue<char> buffer;
```
##### Threads
En este caso las declaraciones varian un poco, sobretodo hubo problemas en la asignación de la función dado que esto no está permitido. Por lo que buscando el foror la solución sería la siguiente:

```C++
moveToThread(&t);
connect(&t, SIGNAL(started()), this, SLOT(run_thread()));
```
Donde asociamos una señal cuando el thread esté activo a un SLOT que sería la función que tenemos en el monitor. Esta clase nos permite usar las funciones start y wait para regular el comportamiento del hilo iniciándolo o pausándolo. Además cuenta con señales propias que indican si el thread inició su funcionamiento o si lo terminó.
```C++
#include <QThread>

bool	wait(unsigned long time)
void	start(QThread::Priority priority = InheritPriority)

//señales propias
void	finished()
void	started()

QThread t;
```
#### Señales y Slots
Indagando sobre este tema las señales vienen a ser el desencadenante de una acción en específica. Por ejemplo, al clicar en un botón queremos que se visualice algo en la pantalla, por lo tanto, dicho botón cuenta con su propia señal que indica que ha sido presionado y esto ejecuta una acción determina como escribir “Hello World” en una etiqueta. Las acciones son los SLOTS.
Las implementaciones de ambos son similares, se declaran funciones y como parámetros están el mensaje a comunicar al usuario, como una variable tipo int, char, string. Como vemos, varios widgets de QT cuentan con sus propias señales, pero es posible que las clases también tengan señales.
Para el caso de hilos en el mismo hilo tendremos que emitir las variables que queremos mostrar en la interfaz, esto se consigue con la palabra reservada emit seguida de esta colocamos la señal y pasamos el valor por parámetro. Pero para poder evidenciar todo ello en GUI debemos realizar la conexión con la clase que está asociada a la interfaz de usuario. Esto se realiza con la función connect en la que especificamos la clase de la viene la señal, la declaración de la señal y luego lo conectamos con el SLOT para que desencadene una acción. Para esto declaramos la clase a la que pertenece el SLOT y su declaración.
Para conectar una interfaz con hilos se revisó la referencia [10], para saber más sobre los SLOTS y las señales se revisó la referencia [13].

#### Diseño de la Interfaz Gráfica
La interfaz gráfica fue sumamente fácil de crear, en ella se incluyó una barra de progreso para ver el desenvolvimiento del buffer, group boxes para los consumidores y productores junto con sus respectivos botones de start y stop, también una etiqueta para ver lo que consumieron o produjeron. Finalmente tenemos una list view que para visualizar el contenido en la cola.

Para el funcionamiento de los botones implementamos:

```C++
//clase productor.cpp
void Productor::run_thread(){
    monitor -> addtoQueue();
}

void Productor::Stop(){
    stopThread = false;
    t.start();
}

void Productor::Start(){
    stopThread = true;
    t.wait();
}

//clase dialog.cpp
void Dialog::prodPproduce(char letra){
    ui->label_10->setText(QString(letra));
}

void Dialog::prodSProduce(char letra){
    ui->label_11->setText(QString(letra));
}

void Dialog::on_pushButton_5_clicked()
{
    primero->Start();
}


void Dialog::on_pushButton_6_clicked()
{
    primero->Stop();
}


void Dialog::on_pushButton_7_clicked()
{
    segundo->Start();
}


void Dialog::on_pushButton_8_clicked()
{
    segundo->Stop();
}
```

#### Problemas encontrados
El principal problema por el que no se pudo continuar con la implementación se debe al uso de señales en Qt, generalmente se pueden emitir los datos desde el thread pero no fue posible dado que la implementación del thread forma parte del Monitor. Por lo tanto, se requería la instancia del thread en el monitor, tuve algunas ideas para implementar como pasar la referencia del hilo en sus respectivos procedimientos sin embargo el tiempo jugó en contra. Por ello los resultados del caso 2 estarán en el modo consola y la implementación gráfica quedó a medias.

## Conclusiones
-	El problema de productores consumidores aplicando monitores nos permite aprender y evidenciar cómo se puede sincronizar y modelar el comportamiento de diferentes hilos.
-	El uso de exclusión mutua puede ser complementado con el uso de condiciones variables que permiten la comunicación entre los hilos. Esto optimiza el funcionamiento porque a través de este permitimos que por detrás los hilos se ordenen como en una cola.
-	QT es una herramienta que requiere tiempo para ser aprendida y comprendida en toda su extensión, sobre todo a la hora de implementar problemas complejos. Sin embargo, la interfaz para la creación de GUI es amigable y fácil de usar.

## Cuestionario de Preguntas
-	Según el trabajo expuesto: Presente una solución en caso de querer emitir una señal de una clase a otra.
-	¿Cuáles son las ventajas de usar monitores?
-	¿Es necesario utilizar variables condicionales en los monitores?
-	¿Qué otros usos tienen las variables condicionales?
-	¿Cómo conectamos hilos en una GUI en QT?
- ¿Se puede usar QT con Pyhton?

## Video
https://youtu.be/ZNlo6MJR180

## Referencias
1.	https://www.youtube.com/watch?v=Qx3P2wazwI0&t=396s
2.	https://www.geeksforgeeks.org/producer-consumer-problem-and-its-implementation-with-c/
3.	https://www.geeksforgeeks.org/core-dump-segmentation-fault-c-cpp/
4.	https://www.youtube.com/watch?v=ufdQ0GR855M
5.	https://levelup.gitconnected.com/producer-consumer-problem-using-condition-variable-in-c-6c4d96efcbbc
6.	https://www.youtube.com/watch?v=XZDx41woDoI
7.	https://andrew128.github.io/ProducerConsumer/
8.	https://www.youtube.com/watch?v=PR6wVy7Et1A
9.	https://doc.qt.io/qt-6/qtcore-threads-waitconditions-example.html
10.	https://www.bogotobogo.com/Qt/Qt5_QThreads_GuiThread.php
11.	https://www.youtube.com/watch?v=k5tIk7w50L4
12.	https://stackoverflow.com/questions/6368473/is-it-possible-to-implement-polling-with-qthread-without-subclassing-it
13.	https://www.youtube.com/watch?v=JakMj5XEBoc
14.	https://doc.qt.io/qt-6/qwaitcondition.html
15.	https://doc.qt.io/qt-6/qmutex.html
16.	https://doc.qt.io/qt-6/qqueue.html
17.	https://www.bogotobogo.com/Qt/Qt5_QThreads_QSemaphore_Producer_Consumer_GuiThread.php
18.	https://doc.qt.io/qt-6/threads.html
19.	https://forum.qt.io/topic/114672/keeping-track-of-threads
20.	https://forum.qt.io/topic/32652/solved-run-a-function-in-another-thread/3
21.	https://www.youtube.com/watch?v=8Xd_Kc7g1KA
22.	https://stackoverflow.com/questions/6382937/qt-how-to-put-my-function-into-a-thread
23.	https://stackoverflow.com/questions/6368473/is-it-possible-to-implement-polling-with-qthread-without-subclassing-it
24.	https://pages.mtu.edu/~shene/NSF-3/e-Book/MONITOR/ProducerConsumer-1/MON-example-buffer-1.html
25.	https://forum.qt.io/topic/79671/qthread-sleep-question
26.	https://forum.qt.io/topic/102357/calling-a-function-when-a-signal-is-emitted-from-another-class/3
27.	https://stackoverflow.com/questions/15176458/how-can-i-emit-a-signal-from-another-class
