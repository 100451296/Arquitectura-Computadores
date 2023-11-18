#include "progargs.hpp"


using namespace std;

int comprobacion_num_param(int num_params) {
  // Esta comprobacion se encarga de evaluar el numero de argumentos que se reciben como entrada
  if (num_params != 4) {
    // En caso de no ser exactamente tres argumentos, se generara un mensaje de error y se termina
    // el programa devolviendo -1
    //Se resta 1 a num_params porque no se cuenta el comando de ejecucion fluid
    num_params -= 1;
    cout << "Error: Invalid number of arguments: " << num_params  << endl;
    return -1;
  }
  return 0;
}

int comprobacion_primer_argumento(string const & num_iter) {
  /*Esta comprobacion evalua si el primer argumento recibido no es un numero entero.
  En caso de no serlo, muestra un mensaje de error y devuelve -1*/
  bool primer_caracter = true; // Variable para identificar el primer carácter

  for (char c : num_iter) {
    if (primer_caracter) {
      // Si es el primer carácter, puede ser un número o un signo negativo
      if (!isdigit(c) && c != '-') {
        cout << "Error: time steps must be numeric." << endl;
        return -1;
      }
      primer_caracter = false; // Ya no es el primer carácter
    } else {
      // Para los caracteres subsiguientes, deben ser dígitos
      if (!isdigit(c)) {
        cout << "Error: time steps must be numeric." << endl;
        return -1;
      }
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
    cout << "Error: Cannot open " << input << " for reading" << endl;
    return -3;
  }
  return 0;
}

int comprobacion_archivo_escritura(string const & output) {
  /*Esta comprobacion evalua si el archivo de salida no se puede abrir para escritura.
  En caso de que no es evalue, envia un mensaje de error y se devuelve -4*/
  // Primero verifica si el archivo existe, en caso de que no, devuelve un error en lugar de crear un archivo nuevo
    if (!filesystem::exists(output)) {
        cout << "Error: Cannot open " << output << " for writing" << endl;
        return -4;
    }
  ofstream salida(output);
  if (!salida.is_open()) {
    cout << "Error: Cannot open " << output << " for writing" << endl;
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

int leer_archivo_entrada(const string& num_iter_file) { 
  ifstream fichero(num_iter_file, ios::binary); 
  if (!fichero) { 
    cout << "No se pudo abrir el archivo." << endl; 
    return -1; } 
  // Cada particula equivale a 9 * 4 (tamaño de float) bytes
  const size_t size_of_particle = 9 * sizeof(float); 
  int num_particles, particles_contadas = 0; 
  fichero.read(reinterpret_cast<char *>(&num_particles), sizeof(int));  
  float particle_data[9]; 
  while (fichero.read(reinterpret_cast<char *>(&particle_data), size_of_particle)) { 
    particles_contadas++; 
    } 
  if (fichero.eof()) { 
    cout << "Fin del archivo alcanzado." << endl; 
    } 
  else if(fichero.fail()) { 
    cout << "Error de lectura del archivo antes de llegar al final." << endl;
  } fichero.close(); 
  if (particles_contadas != num_particles) { 
    cout << "El número de partículas contadas no coincide con el esperado." << endl; 
    return -5; 
    } 
  return 0; 
  }


int proargs_validations(string const & num_iter, string const & input,string const & output) {         
  if (comprobacion_primer_argumento(num_iter) == -1) { return -2; }  // Numero de iteraciones
  if (comprobacion_pasos_tiempo(num_iter) == -2) { return -3; }      // Numero de iteraciones
  if (comprobacion_archivo_lectura(input) == -3) { return -4; }      // Archivo de entrada
  if (comprobacion_archivo_escritura(output) == -4) { return -5; }   // Archivo de salida
  int resultado = leer_archivo_entrada(input);
  cout << "resultado:" << resultado << endl;

  return 0;
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
