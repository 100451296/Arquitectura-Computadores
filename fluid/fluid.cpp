#include <iostream>
#include "../sim/progargs.hpp"

int main(int argc, char* argv[]) {
	using namespace std;
    // Código de la aplicación
	cout << "Programa main" << std::endl;
	//int resultado = progargs(argc, argv[1], argv[2], argv[3]);
	for (int i = 0; i < argc; i++){
		cout << argv[i] << endl;
	}
    return 0;
}
