#include "progargs.hpp"

using namespace std;

int comprobacion_num_param(int num_params) {
  // Esta comprobacion se encarga de evaluar el numero de argumentos que se reciben como entrada
  if (num_params != 4) {
    // En caso de no ser exactamente tres argumentos, se generara un mensaje de error y se termina
    // el programa devolviendo -1
    cout << "Error: Invalid number of arguments: " << num_params << endl;
    return -1;
  }
  return 0;
}

int comprobacion_primer_argumento(string const & num_iter) {
  /*Esta comprobacion evalua si el primer argumento recibido no es un numero entero.
  En caso de no serlo, muestra un mensaje de error y devuelve -1*/
  for (char c : num_iter) {
    if (!isdigit(c)) {
      cout << "Error: time steps must be numeric." << endl;
      return -1;
    }
  }
  return 0;
}

int comprobacion_pasos_tiempo(string const & num_iter) {
  int pasos = stoi(num_iter);
  /*Esta comprobacion evalua si el numero de pasos de tiempo (primer argumento recibido) es un
  numero negativo. En caso de serlo, se genera un mensaje de error y se devuelve -2*/
  if (pasos < 0) {
    cout << "Error: Invalid number of time steps." << endl;
    return -2;
  }
  return 0;
}

int comprobacion_archivo_lectura(string const & input) {
  /*Esta comprobacion evalua si el archivo de entrada se puede abrir para lectura.
  En caso de que no se pueda, envia un mensaje de error y se devuelve -3 */
  ifstream entrada(input);
  if (!entrada.is_open()) {
    cout << "Error: Cannot open " << input << "for reading" << endl;
    return -3;
  }
  return 0;
}

int comprobacion_archivo_escritura(string const & output) {
  /*Esta comprobacion evalua si el archivo de salida no se puede abrir para escritura.
  En caso de que no es evalue, envia un mensaje de error y se devuelve -4*/
  ofstream salida(output);
  if (!salida.is_open()) {
    cout << "Error: Cannot open " << output << "for writing" << endl;
    return -4;
  }
  return 0;
}

int iniciacion_simulacion(int num_particulas) {
  if (num_particulas <= 0) {
    cout << "Error: Invalid number of particles: " << num_particulas << endl;
    return -5;
  }
  return 0;
}

int comparacion_cantidad_particulas(int num_particles, int particles_contadas) {
  /*Esta comprobacion evalua si el numero de particulas indicado en cabecera coincide con el numero
  de particulas leidas en el archivo de entrada. En caso de que no coincidan, envia un mensaje de
  error y se devuelve -5*/
  if (num_particles != particles_contadas) {
    cout << "Error: Number of particles mismatch. Header: " << num_particles
         << ", Found: " << particles_contadas << endl;
    return -5;
  }
  return 0;
}

int leer_archivo_entrada(string num_iter_file) {
  ifstream fichero;
  fichero.open(num_iter_file, ios::binary);
  int num_particles, particles_contadas;
  // Mueve el puntero del archivo. Nos saltamos el valor de ppm para la lectura
  fichero.seekg(sizeof(float), ios::cur);
  fichero.read(reinterpret_cast<char *>(&num_particles), sizeof(int));
  while (!fichero.eof()) {
    // Desplazamos puntero cada 9 valores, contando asi una particula
    fichero.seekg(9 * sizeof(float), ios::cur);
    particles_contadas += 1;
  }
  if (comparacion_cantidad_particulas(num_particles, particles_contadas) == -5) { return -5; }
  return 0;
}

int proargs_validations(int num_params, string const & num_iter, string const & input,
                        string const & output) {
  // Comprobación numero parametros
  if (comprobacion_num_param(num_params) == -1) { return -1; }

  // Comprobación numero de iteraciones
  if (comprobacion_primer_argumento(num_iter) == -1) { return -1; }

  // Comprobacion numero
  if (comprobacion_pasos_tiempo(num_iter) == -1) { return -1; }

  // Comprobacion archivo de entrada
  if (comprobacion_archivo_lectura(input) == -1) { return -1; }

  // Comprobacion de archivo de salida
  if (comprobacion_archivo_escritura(output) == -1) { return -1; }

  return 0;
}

int readFile(std::string const & input_file_name, float & ppm, int & num_particles,
             std::vector<Particle> & particles) {
  std::ifstream input_file(input_file_name, std::ios::binary);
  if (!input_file.is_open()) {
    std::cerr << "Error al abrir el archivo de entrada" << std::endl;
    return -1;
  }

  if (!readHeader(input_file, ppm, num_particles)) { return -1; }

  if (num_particles <= 0) {
    std::cerr << "Número de partículas inválido" << std::endl;
    return -1;
  }

  if (!readParticles(input_file, particles, num_particles)) { return -1; }

  input_file.close();
  return 0;
}

bool readHeader(std::ifstream & input_file, float & ppm, int & num_particles) {
  input_file.read(reinterpret_cast<char *>(&ppm), sizeof(float));
  input_file.read(reinterpret_cast<char *>(&num_particles), sizeof(int));
  return input_file.good();
}

bool readParticles(std::ifstream & input_file, std::vector<Particle> & particles,
                   int num_particles) {
  particles.resize(num_particles);
  for (int i = 0; i < num_particles; i++) {
    if (!readParticle(input_file, particles[i], i)) { return false; }
  }
  return true;
}

bool readParticle(std::ifstream & input_file, Particle & particle, int index) {
  float buffer[9];
  if (!input_file.read(reinterpret_cast<char *>(buffer), sizeof(float) * 9)) {
    std::cerr << "Error al leer las partículas del archivo" << std::endl;
    return false;
  }

  particle.id         = index;
  particle.posX       = static_cast<double>(buffer[0]);
  particle.posY       = static_cast<double>(buffer[1]);
  particle.posZ       = static_cast<double>(buffer[2]);
  particle.smoothVecX = static_cast<double>(buffer[3]);
  particle.smoothVecY = static_cast<double>(buffer[4]);
  particle.smoothVecZ = static_cast<double>(buffer[5]);
  particle.velX       = static_cast<double>(buffer[6]);
  particle.velY       = static_cast<double>(buffer[7]);
  particle.velZ       = static_cast<double>(buffer[8]);

  return true;
}

void printParameters(int ppm, int num_particles) {
  // Calcula parametros
  double h             = MULTIPLICADOR_RADIO / ppm;
  double particle_mass = DENSIDAD_FLUIDO * std::pow(ppm, -3);
  int nx               = std::floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / h);
  int ny               = std::floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / h);
  int nz               = std::floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / h);
  int num_blocks       = nx * ny * nz;
  double sx            = (LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / nx;
  double sy            = (LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / ny;
  double sz            = (LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / nz;

  // Imprime parametros
  std::cout << "Number of particles: " << num_particles << std::endl;
  std::cout << "Particles per meter: " << static_cast<int>(ppm) << std::endl;
  std::cout << "Smoothing length: " << h << std::endl;
  std::cout << "Particle mass: " << particle_mass << std::endl;
  std::cout << "Grid size: " << nx << " x " << ny << " x " << nz << std::endl;
  std::cout << "Number of blocks: " << num_blocks << std::endl;
  std::cout << "Block size: " << sx << " x " << sy << " x " << sz << std::endl;
}
