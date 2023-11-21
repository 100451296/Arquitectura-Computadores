#include "progargs.hpp"

using namespace std;

int comprobacion_num_param(int num_params) {
  // Esta comprobacion se encarga de evaluar el numero de argumentos que se reciben como entrada
  if (num_params != 4) {
    // En caso de no ser exactamente tres argumentos, se generara un mensaje de error y se termina
    // el programa devolviendo -1
    // Se resta 1 a num_params porque no se cuenta el comando de ejecucion fluid
    num_params -= 1;
    cerr << "Error: Invalid number of arguments: " << num_params << "\n";
    return -1;
  }
  return 0;
}

int comprobacion_primer_argumento(string const & num_iter) {
  /*Esta comprobacion evalua si el primer argumento recibido no es un numero entero.
  En caso de no serlo, muestra un mensaje de error y devuelve -1*/
  bool primer_caracter = true;  // Variable para identificar el primer carácter

  for (char c : num_iter) {
    if (primer_caracter) {
      // Si es el primer carácter, puede ser un número o un signo negativo
      if (!isdigit(c) && c != '-') {
        cerr << "Error: time steps must be numeric."
             << "\n";
        return -1;
      }
      primer_caracter = false;  // Ya no es el primer carácter
    } else {
      // Para los caracteres subsiguientes, deben ser dígitos
      if (!isdigit(c)) {
        cerr << "Error: time steps must be numeric."
             << "\n";
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
    cerr << "Error: Invalid number of time steps."
         << "\n";
    return -2;
  }
  return 0;
}

int comprobacion_archivo_lectura(string const & input) {
  /*Esta comprobacion evalua si el archivo de entrada se puede abrir para lectura.
  En caso de que no se pueda, envia un mensaje de error y se devuelve -3 */
  ifstream entrada(input);
  if (!entrada.is_open()) {
    cerr << "Error: Cannot open " << input << " for reading"
         << "\n";
    return -3;
  }
  return 0;
}

int comprobacion_archivo_escritura(string const & output) {
  /*Esta comprobacion evalua si el archivo de salida no se puede abrir para escritura.
  En caso de que no es evalue, envia un mensaje de error y se devuelve -4*/
  // Primero verifica si el archivo existe, en caso de que no, devuelve un error en lugar de crear
  // un archivo nuevo
  if (!filesystem::exists(output)) {
    cerr << "Error: Cannot open " << output << " for writing"
         << "\n";
    return -4;
  }
  ofstream salida(output);
  if (!salida.is_open()) {
    cerr << "Error: Cannot open " << output << " for writing"
         << "\n";
    return -4;
  }
  return 0;
}

int iniciacion_simulacion(int num_particulas) {
  // Esta comprobacion evalua si el numero de particulas encontrado en la cabecera es 0 o un numero
  // negativo
  if (num_particulas <= 0) {
    cerr << "Error: Invalid number of particles: " << num_particulas << "\n";
    return -5;
  }
  return 0;
}

int comparacion_cantidad_particulas(int num_particles, int particles_contadas) {
  /*Esta comprobacion evalua si el numero de particulas indicado en cabecera coincide con el numero
  de particulas leidas en el archivo de entrada. En caso de que no coincidan, envia un mensaje de
  error y se devuelve -5*/
  if (num_particles != particles_contadas) {
    cerr << "Error: Number of particles mismatch. Header: " << num_particles
         << ", Found: " << particles_contadas << "\n";
    return -5;
  }
  return 0;
}

int leer_archivo_entrada(string const & num_iter_file) {
  ifstream fichero(num_iter_file, ios::binary);
  if (!fichero) {
    cerr << "No se pudo abrir el archivo."
         << "\n";
    return -1;
  }
  float ppm;
  int num_particles;
  fichero.read(reinterpret_cast<char *>(&ppm), sizeof(float));  // Lectura del particulas per meter
  fichero.read(reinterpret_cast<char *>(&num_particles),
               sizeof(int));                             // Lectura del numero de particulas
  int resultado = iniciacion_simulacion(num_particles);  // Comprobacion de iniciacion_simulacion
  if (resultado != 0) {
    cerr << "Error: Invalid number of particles: " << num_particles << "\n";
    return resultado;
  }
  // Cada particula equivale a 9 * 4 (tamaño de float) bytes. A continuacion se va a comenzar con la
  // lectura de particulas del archivo
  size_t const size_of_particle = 9 * sizeof(float);
  int particles_contadas        = 0;
  float particle_data[9];
  while (fichero.read(reinterpret_cast<char *>(&particle_data), size_of_particle)) {
    particles_contadas++;
  }
  fichero.close();
  int resultado_comp = comparacion_cantidad_particulas(
      num_particles, particles_contadas);  // Comprobacion de la cantidad de particulas
  if (resultado_comp != 0) {
    cerr << "Error: Number of particles mismatch. Header: " << num_particles
         << ", Found: " << particles_contadas << "\n";
    return resultado_comp;
  }
  return 0;
}

int proargs_validations(string const & num_iter, string const & input, string const & output) {
  if (comprobacion_primer_argumento(num_iter) == -1) { return -2; }  // Numero de iteraciones
  if (comprobacion_pasos_tiempo(num_iter) == -2) { return -3; }      // Numero de iteraciones
  if (comprobacion_archivo_lectura(input) == -3) { return -4; }      // Archivo de entrada
  if (comprobacion_archivo_escritura(output) == -4) { return -5; }   // Archivo de salida
  if (leer_archivo_entrada(input) == -5) { return -6; }              // Numero de particulas

  return 0;
}

void printParameters(int ppm, int num_particles) {
  // Calcula parametros y los imprime directamente
  cout << "Number of particles: " << num_particles << "\n";
  cout << "Particles per meter: " << ppm << "\n";
  cout << "Smoothing length: " << MULTIPLICADOR_RADIO / static_cast<double>(ppm) << "\n";
  cout << "Particle mass: " << DENSIDAD_FLUIDO * pow(ppm, -3) << "\n";
  cout << "Grid size: "
       << floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) /
                (MULTIPLICADOR_RADIO / ppm))
       << " x "
       << floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) /
                (MULTIPLICADOR_RADIO / ppm))
       << " x "
       << floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) /
                (MULTIPLICADOR_RADIO / ppm))
       << "\n";
  cout << "Number of blocks: "
       << floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) /
                (MULTIPLICADOR_RADIO / ppm)) *
              floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) /
                    (MULTIPLICADOR_RADIO / ppm)) *
              floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) /
                    (MULTIPLICADOR_RADIO / ppm))
       << "\n";
  cout << "Block size: "
       << (LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) /
              floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) /
                    (MULTIPLICADOR_RADIO / ppm))
       << " x "
       << (LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) /
              floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) /
                    (MULTIPLICADOR_RADIO / ppm))
       << " x "
       << (LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) /
              floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) /
                    (MULTIPLICADOR_RADIO / ppm))
       << "\n";
}
