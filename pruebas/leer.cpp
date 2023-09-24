#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

struct Particula {
    double x, y, z;      // Posición
    double hvx, hvy, hvz;   // Vector de suavizado
    double vx, vy, vz;   // Velocidad
};

int main() {
    std::ifstream archivo("small.fld", std::ios::binary);
    if (!archivo) {
        std::cerr << "Error al abrir el archivo" << std::endl;
        return 1;
    }

    // Leer la cabecera del archivo
    float ppm;
    int num_particulas;
    archivo.read(reinterpret_cast<char*>(&ppm), sizeof(ppm));
    archivo.read(reinterpret_cast<char*>(&num_particulas), sizeof(num_particulas));

    // Calcular parámetros adicionales
    double h = 1.695 / ppm;
    double m = 1000 * std::pow(ppm, -3);
    double nx = std::floor((0.065 - (-0.065)) / h);
    double ny = std::floor((0.1 - (-0.08)) / h);
    double nz = std::floor((0.065 - (-0.065)) / h);
    double sx = (0.065 - (-0.065)) / nx;
    double sy = (0.1 - (-0.08)) / ny;
    double sz = (0.065 - (-0.065)) / nz;

    std::cout << "Number of particles: " << num_particulas << std::endl;
    std::cout << "Particles per meter: " << ppm << std::endl;
    std::cout << "Smoothing length: " << h << std::endl;
    std::cout << "Particle mass: " << m << std::endl;
    std::cout << "Grid size: " << nx << " x " << ny << " x " << nz << std::endl;
    std::cout << "Number of blocks: " << nx * ny * nz << std::endl;
    std::cout << "Block size: " << sx << " x " << sy << " x " << sz << std::endl;

    // Leer las partículas
    std::vector<Particula> particulas(num_particulas);
    for (int i = 0; i < num_particulas; ++i) {
        float x, y, z, hvx, hvy, hvz, vx, vy, vz;
        archivo.read(reinterpret_cast<char*>(&x), sizeof(float));
        archivo.read(reinterpret_cast<char*>(&y), sizeof(float));
        archivo.read(reinterpret_cast<char*>(&z), sizeof(float));
        archivo.read(reinterpret_cast<char*>(&hvx), sizeof(float));
        archivo.read(reinterpret_cast<char*>(&hvy), sizeof(float));
        archivo.read(reinterpret_cast<char*>(&hvz), sizeof(float));
        archivo.read(reinterpret_cast<char*>(&vx), sizeof(float));
        archivo.read(reinterpret_cast<char*>(&vy), sizeof(float));
        archivo.read(reinterpret_cast<char*>(&vz), sizeof(float));

        particulas[i].x = static_cast<double>(x);
        particulas[i].y = static_cast<double>(y);
        particulas[i].z = static_cast<double>(z);
        particulas[i].hvx = static_cast<double>(hvx);
        particulas[i].hvy = static_cast<double>(hvy);
        particulas[i].hvz = static_cast<double>(hvz);
        particulas[i].vx = static_cast<double>(vx);
        particulas[i].vy = static_cast<double>(vy);
        particulas[i].vz = static_cast<double>(vz);
    }

    archivo.close();

    // Imprimir las partículas en formato legible
    std::ofstream salida("salida.txt");
    salida << "Número de partículas: " << num_particulas << std::endl;
    salida << "Partículas por metro: " << ppm << std::endl;
    salida << "Longitud de suavizado: " << h << std::endl;
    salida << "Masa de la partícula: " << m << std::endl;
    salida << "Tamaño de malla: " << nx << " x " << ny << " x " << nz << std::endl;
    salida << "Tamaño de bloque de malla: " << sx << " x " << sy << " x " << sz << std::endl;
    salida << std::endl;

    for (int i = 0; i < num_particulas; ++i) {
        salida << "Partícula " << i << ":" << std::endl;
        salida << "Posición: (" << particulas[i].x << ", " << particulas[i].y << ", " << particulas[i].z << ")" << std::endl;
        salida << "Vector de suavizado: (" << particulas[i].hvx << ", " << particulas[i].hvy << ", " << particulas[i].hvz << ")" << std::endl;
        salida << "Velocidad: (" << particulas[i].vx << ", " << particulas[i].vy << ", " << particulas[i].vz << ")" << std::endl;
        salida << std::endl;
    }
    salida.close();

    return 0;
}
