#include "block.hpp"

#include <iostream>

using namespace std;

int block() {
  // Código de las pruebas unitarias
  Block block;

  block.densityIncrease(block);

  return 0;
}

void Block::initVectors() {
  fill(this->accelerations.begin(), this->accelerations.end(), 0);
  fill(this->densities.begin(), this->densities.end(), 0);
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
