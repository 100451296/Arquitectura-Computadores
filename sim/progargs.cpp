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
  if (comprobacion_num_param(num_params) == -1) { return -1; }       // Numero de parametros
  if (comprobacion_primer_argumento(num_iter) == -1) { return -2; }  // Numero de iteraciones
  if (comprobacion_pasos_tiempo(num_iter) == -2) { return -3; }      // Numero de iteraciones
  if (comprobacion_archivo_lectura(input) == -3) { return -4; }      // Archivo de entrada
  if (comprobacion_archivo_escritura(output) == -4) { return -5; }   // Archivo de salida

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

int writeFile(std::string const & output_file_name, float ppm, int num_particles,
              std::vector<Particle> const & particles) {
  std::ofstream output_file(output_file_name, std::ios::binary);
  if (!output_file.is_open()) {
    std::cerr << "Error al abrir el archivo de salida" << std::endl;
    return -1;
  }

  if (!writeHeader(output_file, ppm, num_particles)) { return -1; }

  if (!writeParticles(output_file, particles)) { return -1; }

  output_file.close();
  return 0;
}

bool writeHeader(std::ofstream & output_file, float ppm, int num_particles) {
  output_file.write(reinterpret_cast<char const *>(&ppm), sizeof(float));
  output_file.write(reinterpret_cast<char const *>(&num_particles), sizeof(int));
  return output_file.good();
}

bool writeParticles(std::ofstream & output_file, std::vector<Particle> const & particles) {
  for (unsigned int i = 0; i < particles.size(); i++) {
    if (!writeParticle(output_file, particles[i])) { return false; }
  }
  return true;
}

bool writeParticle(std::ofstream & output_file, Particle const & particle) {
  float buffer[9] = {
    static_cast<float>(particle.posX),       static_cast<float>(particle.posY),
    static_cast<float>(particle.posZ),       static_cast<float>(particle.smoothVecX),
    static_cast<float>(particle.smoothVecY), static_cast<float>(particle.smoothVecZ),
    static_cast<float>(particle.velX),       static_cast<float>(particle.velY),
    static_cast<float>(particle.velZ)};

  if (!output_file.write(reinterpret_cast<char const *>(buffer), sizeof(float) * 9)) {
    std::cerr << "Error al escribir las partículas en el archivo" << std::endl;
    return false;
  }

  return true;
}

void printParameters(int ppm, int num_particles) {
  // Calcula parametros y los imprime directamente
  std::cout << "Number of particles: " << num_particles << std::endl;
  std::cout << "Particles per meter: " << ppm << std::endl;
  std::cout << "Smoothing length: " << MULTIPLICADOR_RADIO / static_cast<double>(ppm) << std::endl;
  std::cout << "Particle mass: " << DENSIDAD_FLUIDO * std::pow(ppm, -3) << std::endl;
  std::cout << "Grid size: "
            << std::floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) /
                          (MULTIPLICADOR_RADIO / ppm))
            << " x "
            << std::floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) /
                          (MULTIPLICADOR_RADIO / ppm))
            << " x "
            << std::floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) /
                          (MULTIPLICADOR_RADIO / ppm))
            << std::endl;
  std::cout << "Number of blocks: "
            << std::floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) /
                          (MULTIPLICADOR_RADIO / ppm)) *
                   std::floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) /
                              (MULTIPLICADOR_RADIO / ppm)) *
                   std::floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) /
                              (MULTIPLICADOR_RADIO / ppm))
            << std::endl;
  std::cout << "Block size: "
            << (LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) /
                   std::floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) /
                              (MULTIPLICADOR_RADIO / ppm))
            << " x "
            << (LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) /
                   std::floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) /
                              (MULTIPLICADOR_RADIO / ppm))
            << " x "
            << (LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) /
                   std::floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) /
                              (MULTIPLICADOR_RADIO / ppm))
            << std::endl;
}
