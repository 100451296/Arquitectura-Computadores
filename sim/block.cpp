#include "block.hpp"

#include <iostream>

using namespace std;

// coment

// Función para probar los metodos
int block() {
  // Crear dos arreglos de partículas
  Particle particles1[3];
  Particle particles2[3];

  // Inicializar las partículas con algunos valores de ejemplo
  for (int i = 0; i < 3; i++) {
    particles1[i].id = i;
    particles1[i].posX = i * 1.0;
    particles1[i].posY = i * 1.0;
    particles1[i].posZ = i * 1.0;

    particles2[i].id = i + 3;  // Asegurarse de que las ID sean diferentes
    particles2[i].posX = (i + 3) * 1.0;
    particles2[i].posY = (i + 3) * 1.0;
    particles2[i].posZ = (i + 3) * 1.0;
  }

  // Crear dos bloques
  Block block1;
  Block block2;

  // Añadir las partículas del primer arreglo al primer bloque
  for (int i = 0; i < 3; i++) {
    block1.addParticle(&particles1[i]);
  }

  // Añadir las partículas del segundo arreglo al segundo bloque
  for (int i = 0; i < 3; i++) {
    block2.addParticle(&particles2[i]);
  }

  // Realizar otras operaciones o pruebas con los bloques aquí
  block1.densityIncrease(block2);

  return 0;
}

// METODOS de BLOCK

void Block::initVectors() {
  fill(this->accelerations.begin(), this->accelerations.end(), 0);
  fill(this->densities.begin(), this->densities.end(), 0);
}

// Función para añadir una partícula al vector
void Block::addParticle(Particle* particle) {
  this->particlesId.push_back(particle);
  this->accelerations.push_back(0.0);  // Agrega una entrada 0 a accelerations
  this->densities.push_back(0.0);      // Agrega una entrada 0 a densities
}

void Block::densityIncrease(Block& contiguousBlock) {
  // Funcion encarga de modificar el vector de densidades del propio bloque y
  // del contiguo incluyendo la transformación lineal

  Block aux = contiguousBlock;  // Borrar linea

  cout << "Incremento de densidades" << endl;
  // Tu código aquí
}

void Block::accelerationTransfer(Block& contiguousBlock) {
  // Funcion que se encarga de actualizar el vector de aceleraciones del propio
  // bloque y debe calcular la distancia y el incremento

  Block aux = contiguousBlock;  // Borrar linea

  cout << "Transferencia de aceleración" << endl;
  // Tu código aquí
}

void Block::collisionsX(unsigned int cx) {
  // Debe actualizar la componente x del vector de aceleración para cada
  // particula del bloque

  cout << "Colisiones en la dirección X con valor de cx: " << cx << endl;
  // Tu código aquí
}

void Block::collisionsY(unsigned int cy) {
  // Debe actualizar la componente y del vector de aceleración para cada
  // particula del bloque

  cout << "Colisiones en la dirección Y con valor de cy: " << cy << endl;
  // Tu código aquí
}

void Block::collisionsZ(unsigned int cx) {
  // Debe actualizar la componente z del vector de aceleración para cada
  // particula del bloque

  cout << "Colisiones en la dirección Z con valor de cx: " << cx << endl;
  // Tu código aquí
}

void Block::particleMotion() {
  // Debe actualizar la posicion, velocidad y vector de suavizado de cada
  // particula

  cout << "Movimiento de partículas" << endl;
  // Tu código aquí
}

void Block::interactionsX(unsigned int cx) {
  // Debe actualizar la posicion, velocidad y vector de suavizado de cada
  // particula, cuando dx sea negativo

  cout << "Interacciones en la dirección X con valor de cx: " << cx << endl;
  // Tu código aquí
}

void Block::interactionsY(unsigned int cy) {
  // Debe actualizar la posicion, velocidad y vector de suavizado de cada
  // particula, cuando dy sea negativo

  cout << "Interacciones en la dirección Y con valor de cy: " << cy << endl;
  // Tu código aquí
}

void Block::interactionsZ(unsigned int cz) {
  // Debe actualizar la posicion, velocidad y vector de suavizado de cada
  // particula, cuando dz sea negativo

  cout << "Interacciones en la dirección Z con valor de cz: " << cz << endl;
  // Tu código aquí
}
