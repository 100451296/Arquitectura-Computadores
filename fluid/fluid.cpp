#include <iostream>

int main(int argc, char* argv[]) {
	using namespace std;
    // Código de la aplicación
	std::cout << "Programa main" << std::endl;
	for (int i = 0; i < argc; i++){
		cout << argv[i] << endl;
	}
    return 0;
}
