#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "../sim/progargs.hpp"
#include "../sim/block.hpp"
#include "../sim/grid.hpp"

int main(int argc, char* argv[]) {
    using namespace std;

    // Código de la aplicación
    cout << "Programa main" << std::endl;

     // Comprueba parametros
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <iteraciones> <archivo_entrada> <archivo_salida>" << std::endl;
        return 1;
    }

    int resultado = progargs(argc, argv[1], argv[2], argv[3]); // Validaciones 

   
	
	// Inicializa variables
    //int num_iterations = std::stoi(argv[1]);
    std::string input_file_name = argv[2];
    std::string output_file_name = argv[3];
	
	// Valida archivo de entrada
    std::ifstream input_file(input_file_name, std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo de entrada. " << input_file_name << std::endl;
        return 1;
    }
	
	// Lee particulas x metro y numero de particulas
    float ppm;
    int num_particles;
    input_file.read(reinterpret_cast<char*>(&ppm), sizeof(float));
    input_file.read(reinterpret_cast<char*>(&num_particles), sizeof(int));
	
	// Lee particulas 
    std::vector<Particle> particles(num_particles);
    for (int i = 0; i < num_particles; ++i) {
        input_file.read(reinterpret_cast<char*>(&particles[i]), sizeof(Particle));
    }

    input_file.close();

	// Calcula parametros
    double h = MULTIPLICADOR_RADIO / ppm;
    double particle_mass = DENSIDAD_FLUIDO * std::pow(ppm, -3);
    int nx = std::floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / h);
    int ny = std::floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / h);
    int nz = std::floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / h);
    int num_blocks = nx * ny * nz;
    double sx = (LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / nx;
    double sy = (LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / ny;
    double sz = (LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / nz;

	// Imprime parametros
    std::cout << "Number of particles: " << num_particles << std::endl;
    std::cout << "Particles per meter: " << static_cast<int>(ppm) << std::endl;
    std::cout << "Smoothing length: " << h << std::endl;
    std::cout << "Particle mass: " << particle_mass << std::endl;
    std::cout << "Grid size: " << nx << " x " << ny << " x " << nz << std::endl;
    std::cout << "Number of blocks: " << num_blocks << std::endl;
    std::cout << "Block size: " << sx << " x " << sy << " x " << sz << std::endl;

    
    Grid grid(nx, ny, nz);
    
    
    
    
    
    
    
    
    // Valida archivo de salida
    std::ofstream output_file(output_file_name, std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo de salida." << std::endl;
        return 1;
    }
	
	// Escribe la cabecera (ppm y numero de particulas)
    output_file.write(reinterpret_cast<char*>(&ppm), sizeof(float));
    output_file.write(reinterpret_cast<char*>(&num_particles), sizeof(int));

	// Escribe particulas
    for (int i = 0; i < num_particles; ++i) {
        output_file.write(reinterpret_cast<char*>(&particles[i]), sizeof(Particle));
    }

    output_file.close();
    
    
    return resultado;
}


/*
int main(){
	using namespace std;
	block();
	cout << "eeeee" << endl;
	return 0;
}
*/