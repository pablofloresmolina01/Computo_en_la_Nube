#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

#define N 10000
#define chunk 1
#define mostrar 10

void imprimeArreglo(float* d, int n);

int main()
{
	std::cout << "Sumando Arreglos en Paralelo con OpenMP!\n";

	float a[N], b[N], c[N];
	int quien_lo_hizo[N];

	int i, cantidad;
	char opcion;
	char repetir;

	srand((unsigned int)time(NULL));

	// INICIO DEL CICLO: bloque se repetirá.
	do {
		std::cout << "\n--------------------------------------------------\n";
		std::cout << "Nueva Ejecucion\n";

		do {
			std::cout << "Ingrese la cantidad de elementos a sumar (max " << N << "): ";
			std::cin >> cantidad;
			if (cantidad <= 0 || cantidad > N) {
				std::cout << "Error: La cantidad debe estar entre 1 y " << N << ".\n";
			}
		} while (cantidad <= 0 || cantidad > N);

		std::cout << "Desea llenar los arreglos Manualmente (m) o Aleatoriamente (a)? ";
		std::cin >> opcion;

		if (opcion == 'm' || opcion == 'M')
		{
			std::cout << "Introduce los valores para el arreglo A:\n";
			for (i = 0; i < cantidad; i++) {
				std::cout << "A[" << i << "]: ";
				std::cin >> a[i];
			}
			std::cout << "Introduce los valores para el arreglo B:\n";
			for (i = 0; i < cantidad; i++) {
				std::cout << "B[" << i << "]: ";
				std::cin >> b[i];
			}
		}
		else
		{
			for (i = 0; i < cantidad; i++)
			{
				a[i] = (float)(rand() % 100);
				b[i] = (float)(rand() % 100);
			}
			std::cout << "Arreglos llenados con numeros aleatorios.\n";
		}

		int pedazos = chunk;

#pragma omp parallel
		{
#pragma omp master
			{
				std::cout << "--- Iniciando calculo paralelo con " << omp_get_num_threads() << " hilos ---" << std::endl;
			}
		}

#pragma omp parallel for \
		shared(a,b,c,quien_lo_hizo,pedazos) private(i) \
		schedule(static, pedazos)
		for (i = 0; i < cantidad; i++)
		{
			c[i] = a[i] + b[i];
			quien_lo_hizo[i] = omp_get_thread_num();
		}

		std::cout << "Imprimiendo los primeros " << mostrar << " valores de arreglo a: " << std::endl;
		imprimeArreglo(a, cantidad);

		std::cout << "Imprimiendo los primeros " << mostrar << " valores de arreglo b: " << std::endl;
		imprimeArreglo(b, cantidad);

		std::cout << "Imprimiendo los primeros " << mostrar << " valores de arreglo c (Suma) y Hilo responsable: " << std::endl;

		int limite = (cantidad < mostrar) ? cantidad : mostrar;
		for (int x = 0; x < limite; x++)
			std::cout << c[x] << " (Hilo " << quien_lo_hizo[x] << ") - ";
		std::cout << std::endl;

		std::cout << "\n>>> Desea realizar otra operacion con una cantidad diferente? (s/n): ";
		std::cin >> repetir;

		// FIN DEL CICLO: Si 'repetir' es 's', regresa al inicio del 'do'. Si no, termina el programa.
	} while (repetir == 's' || repetir == 'S');

	std::cout << "Finalizando programa...\n";
	system("pause");
	return 0;
}

void imprimeArreglo(float* d, int n)
{
	int limite = (n < mostrar) ? n : mostrar;

	for (int x = 0; x < limite; x++)
		std::cout << d[x] << " - ";
	std::cout << std::endl;
}