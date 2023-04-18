#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <bitset>
using namespace std;

// Nodos del arbol
struct Nodo {
	char Caracter;
	int Frecuencia;
	Nodo *izq, *der;
};

// Asignamos un nuevo nodo al arbol
Nodo* getNodo(char Caracter, int Frecuencia, Nodo* izq, Nodo* der) {
	Nodo* nodo = new Nodo();
	nodo->Caracter = Caracter;
	nodo->Frecuencia = Frecuencia;
	nodo->izq = izq;
	nodo->der = der;

	return nodo;
}
// struct de comparacion para ordenar la cola
struct Comp {
	bool operator()(Nodo* izquierda, Nodo* derecha) {
		//retorna la frecuencia mas baja en la cola
		return izquierda->Frecuencia > derecha->Frecuencia;
	}
};
// Revisa el arbol a generar, asigna y guarda los codigos de cada caracter con unordered_map
void Comprimir(Nodo* raiz, string str, unordered_map<char, string>& CodigoHuffman) {
	if (raiz == nullptr) {
		return;
	}
	// Si hay nodos recorre cada uno para agregar 0 y 1
	if (!raiz->izq && !raiz->der) {
		CodigoHuffman[raiz->Caracter] = str;
	}
	Comprimir(raiz->izq, str + "0", CodigoHuffman);
	Comprimir(raiz->der, str + "1", CodigoHuffman);
}

// Funcion para crear el arbol
void Arbol(string texto) {
	// Cuenta las frecuencias de cada caracter en el texto y los guarda con unordened_map
	unordered_map<char, int> Frecuencia;
	for (char Caracter : texto) {
		Frecuencia[Caracter]++;
	}
	// Priority_queue para guardar los nodos del arbol
	priority_queue<Nodo*, vector<Nodo*>, Comp> Cola;
	// Se agregan los nodos a la cola
	for (auto pair : Frecuencia) {
		// first son los valores char y second los valores int de Frecuencia
		Cola.push(getNodo(pair.first, pair.second, nullptr, nullptr));
	}
	// Formamos los nodos con las frecuencias mas bajas
	while (Cola.size() != 1) {
		// izq y der son las frecuencias mas bajas, las saca de la cola con pop
		Nodo* izq = Cola.top(); 
		Cola.pop();
		Nodo* der = Cola.top();
		Cola.pop();
		// sum es la suma de las frecuencias mas bajas, con push(getNodo) formamos el nodo resultante
		// de las frecuencias izq y der.
		int sum = izq->Frecuencia + der->Frecuencia;
		Cola.push(getNodo('\0', sum, izq, der));
		// El ciclo seguira hasta que el size de la cola sea igual a 1, lo que significa que habra un solo nodo raiz resultante.
	}
	// raiz almacenara el arbol resultante
	Nodo* raiz = Cola.top();

	// Llamamos la funcion Comprimir para asignar los 0 y 1 al nodo raiz resultante
	unordered_map<char, string> CodigoHuffman;
	Comprimir(raiz, "", CodigoHuffman);

	// ------- PRINTS ------
	// Imprimimos el texto comprimido resultante en consola y escribimos los codigos de compresion en un nuevo .txt
	ofstream Comprimido("comprimido.txt", std::ofstream::out);
	cout << "Los codigos de compresion Huffman son: " << endl;
	for (auto pair : CodigoHuffman) {
		cout << pair.first << ": " << pair.second << endl;
		Comprimido << pair.second;
	}
	// Descomprimimos los codigos de Huffman para ver que concuerda con el texto original.
	cout << "El texto original es: " << endl;
	cout << texto << endl;
	cout << "El texto comprimido es: " << endl;
	string codificado = "";
	for (char Caracter : texto) {
		codificado += CodigoHuffman[Caracter];
	}
	cout << codificado << endl;
	Comprimido.close();
}

int main() {
	ifstream texto;
	texto.open("test.txt", ios::in);;
	string Texto;
	texto >> Texto; 

	Arbol(Texto);
	texto.close();
	return 0;
}