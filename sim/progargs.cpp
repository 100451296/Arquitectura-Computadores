#include "progargs.hpp"


using namespace std;

int comprobacion_num_param(int num_params) {
  // Esta comprobacion se encarga de evaluar el numero de argumentos que se reciben como entrada
  if (num_params != 4) {
    // En caso de no ser exactamente tres argumentos, se generara un mensaje de error y se termina
    // el programa devolviendo -1
    //Se resta 1 a num_params porque no se cuenta el comando de ejecucion fluid
    num_params -= 1;
    cerr << "Error: Invalid number of arguments: " << num_params  << endl;
    return -1;
  }
  return 0;
}

int comprobacion_primer_argumento(string const & num_iter) {
  /*Esta comprobacion evalua si el primer argumento recibido no es un numero entero.
  En caso de no serlo, muestra un mensaje de error y devuelve -1*/
  bool primer_caracter = true; // Variable para identificar el primer carácter

  for (const char caracter_actual : num_iter) {
    if (primer_caracter) {
      // Si es el primer carácter, puede ser un número o un signo negativo
      if (!isdigit(static_cast<unsigned char>(caracter_actual)) && caracter_actual != '-') { //unsigned char evita problemas con valores negativos de char. Se realiza una comparacion explicita
        cerr << "Error: time steps must be numeric." << endl;
        return -1;
      }
      primer_caracter = false; // Ya no es el primer carácter
    } else {
      // Para los caracteres subsiguientes, deben ser dígitos
      if (!isdigit(static_cast<unsigned char>(caracter_actual))) { //Unsigned char aporta la misma ventaja que para el anterior condicional
        cerr << "Error: time steps must be numeric." << endl;
        return -1;
      }
    }
  }
  return 0;
}


int comprobacion_pasos_tiempo(string const & num_iter) {
  const int pasos = stoi(num_iter);
  /*Esta comprobacion evalua si el numero de pasos de tiempo (primer argumento recibido) es un
  numero negativo. En caso de serlo, se genera un mensaje de error y se devuelve -2*/
  if (pasos < 0) {
    cerr << "Error: Invalid number of time steps." << endl;
    return -2;
  }
  return 0;
}

int comprobacion_archivo_lectura(string const & input) {
  /*Esta comprobacion evalua si el archivo de entrada se puede abrir para lectura.
  En caso de que no se pueda, envia un mensaje de error y se devuelve -3 */
  ifstream entrada(input);
  if (!entrada.is_open()) {
    cerr << "Error: Cannot open " << input << " for reading" << endl;
    return -3;
  }
  return 0;
}

int comprobacion_archivo_escritura(string const & output) {
  /*Esta comprobacion evalua si el archivo de salida no se puede abrir para escritura.
  En caso de que no es evalue, envia un mensaje de error y se devuelve -4*/
  // Primero verifica si el archivo existe, en caso de que no, devuelve un error en lugar de crear un archivo nuevo
    if (!filesystem::exists(output)) {
        cerr << "Error: Cannot open " << output << " for writing" << endl;
        return -4;
    }
  ofstream salida(output);
  if (!salida.is_open()) {
    cerr << "Error: Cannot open " << output << " for writing" << endl;
    return -4;
  }
  return 0;
}

int iniciacion_simulacion(int num_particulas) {
  //Esta comprobacion evalua si el numero de particulas encontrado en la cabecera es 0 o un numero negativo
  const int error_particulas_invalidas = -5;
  if (num_particulas <= 0) {
    cerr << "Error: Invalid number of particles: " << num_particulas << endl;
    return error_particulas_invalidas;
  }
  return 0;
}

int comparacion_cantidad_particulas(int num_particles, int particles_contadas) {
  /*Esta comprobacion evalua si el numero de particulas indicado en cabecera coincide con el numero
  de particulas leidas en el archivo de entrada. En caso de que no coincidan, envia un mensaje de
  error y se devuelve -5*/
  const int error_particulas_invalidas1 = -5;
  if (num_particles != particles_contadas) {
    cerr << "Error: Number of particles mismatch. Header: " << num_particles << ", Found: " << particles_contadas << endl;
    return error_particulas_invalidas1;
  }
  return 0;
}

int leer_cabecera_archivo(ifstream& fichero, float& ppm, int& num_particles){
  //Esta funcion se empleara en leer_archivo_entrada y se usa para leer los datos especificados en la cabecera de un archivo de entrada
  fichero.read(reinterpret_cast<char *>(&ppm), sizeof(ppm)); //Lectura del particulas per meter
  fichero.read(reinterpret_cast<char *>(&num_particles), sizeof(num_particles)); //Lectura del numero de particulas
  const int resultado_comp_cabecera = iniciacion_simulacion(num_particles);//Comprobacion de iniciacion_simulacion y se devuelve el resultado
  return resultado_comp_cabecera;
}

int leer_particulas_archivo(ifstream& fichero, int num_particles){
  // Cada particula equivale a 9 * 4 (tamaño de float) bytes. A continuacion se va a comenzar con la lectura de particulas del archivo
  const size_t size_of_particle = 9 * sizeof(float); 
  int particles_contadas = 0; 
  array<float,9> particle_data; 
  while (fichero.read(reinterpret_cast<char *>(particle_data.data()), size_of_particle)) { 
    particles_contadas++; } //Con un bucle vamos guardando en un contador el numero real de particulas que se cuentan en el archivo
  fichero.close(); 
  const int resultado_comp_particulas = comparacion_cantidad_particulas(num_particles, particles_contadas); //Comprobacion de la cantidad de particulas
  return resultado_comp_particulas;
}

int leer_archivo_entrada(const string& num_iter_file) { 
  ifstream fichero(num_iter_file, ios::binary); 
  if (!fichero) { 
    cerr << "No se pudo abrir el archivo." << endl; 
    return -1; } 
  float ppm = 0.0f;
  int num_particles = 0;
  int resultado_comprobacion1 = leer_cabecera_archivo(fichero, ppm, num_particles);
  if (resultado_comprobacion1 != 0){
    return resultado_comprobacion1;
  }
  int resultado_comprobacion2 = leer_particulas_archivo(fichero, num_particles);
  if (resultado_comprobacion2 != 0){
    return resultado_comprobacion2;
  }
  
  return 0; 
  }


int proargs_validations(string const & num_iter, string const & input,string const & output) {    
  const int error_archivo_escritura = -5; //Definido porque 6 es un numero magico
  const int error_num_particulas = -6; //Definido porque 6 es un numero magico     
  if (comprobacion_primer_argumento(num_iter) == -1) { return -2; }  // Numero de iteraciones
  if (comprobacion_pasos_tiempo(num_iter) == -2) { return -3; }      // Numero de iteraciones
  if (comprobacion_archivo_lectura(input) == -3) { return -4; }      // Archivo de entrada
  if (comprobacion_archivo_escritura(output) == -4) { return error_archivo_escritura; }   // Archivo de salida
  if (leer_archivo_entrada(input) == error_archivo_escritura) { return error_num_particulas; } //Numero de particulas 

  return 0;
}

void printParameters(int ppm, int num_particles) {
  // Calcula parametros y los imprime directamente
  cout << "Number of particles: " << num_particles << endl;
  cout << "Particles per meter: " << ppm << endl;
  cout << "Smoothing length: " << MULTIPLICADOR_RADIO / static_cast<double>(ppm) << endl;
  cout << "Particle mass: " << DENSIDAD_FLUIDO * pow(ppm, -3) << endl;
  cout << "Grid size: "
            << floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / (MULTIPLICADOR_RADIO / ppm))
            << " x "
            << floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / (MULTIPLICADOR_RADIO / ppm))
            << " x "
            << floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / (MULTIPLICADOR_RADIO / ppm)) << endl;
  cout << "Number of blocks: "
            << floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / (MULTIPLICADOR_RADIO / ppm)) * floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / (MULTIPLICADOR_RADIO / ppm)) * floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / (MULTIPLICADOR_RADIO / ppm)) << endl;
  cout << "Block size: "
            << (LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / (MULTIPLICADOR_RADIO / ppm))
            << " x "
            << (LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / (MULTIPLICADOR_RADIO / ppm))
            << " x "
            << (LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / (MULTIPLICADOR_RADIO / ppm)) << endl;
}
