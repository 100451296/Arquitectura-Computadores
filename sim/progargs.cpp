#include "progargs.hpp"
using namespace std;


int comprobacion_num_param(int argc){
    //Esta comprobacion se encarga de evaluar el numero de argumentos que se reciben como entrada
    if (argc != 4){
        //En caso de no ser exactamente tres argumentos, se generara un mensaje de error y se termina el programa devolviendo -1
        cout << "Error: Invalid number of arguments: " << argc << endl;
        return -1;
    }
    return 0;
}

int comprobacion_primer_argumento(const string & arg1){
    /*Esta comprobacion evalua si el primer argumento recibido no es un numero entero.
    En caso de no serlo, muestra un mensaje de error y devuelve -1*/
    for (char c : arg1){
        if (!isdigit(c)){
        cout << "Error: time steps must be numeric." << endl;
        return -1;
        }  
    }
    return 0;
}

int comprobacion_pasos_tiempo(const string & arg1){
    int pasos = stoi(arg1);
    /*Esta comprobacion evalua si el numero de pasos de tiempo (primer argumento recibido) es un numero negativo. 
    En caso de serlo, se genera un mensaje de error y se devuelve -2*/
    if (pasos< 0){
        cout << "Error: Invalid number of time steps." << endl;
        return -2;
    }
    return 0;
}

int comprobacion_archivo_lectura(const string & arg2){
    /*Esta comprobacion evalua si el archivo de entrada se puede abrir para lectura.
    En caso de que no se pueda, envia un mensaje de error y se devuelve -3 */
    ifstream entrada(arg2);
    if (!entrada.is_open()){
        cout << "Error: Cannot open " << arg2 << "for reading" << endl;
        return -3;
    }
    return 0;
}

int comprobacion_archivo_escritura(const string & arg3){
    /*Esta comprobacion evalua si el archivo de salida no se puede abrir para escritura.
    En caso de que no es evalue, envia un mensaje de error y se devuelve -4*/
    ofstream salida(arg3);
    if (!salida.is_open()){
        cout << "Error: Cannot open " << arg3 << "for writing" << endl;
        return -4;
    }
    return 0;
}

int iniciacion_simulacion(int num_particulas){
    if (num_particulas <= 0){
        cout << "Error: Invalid number of particles: " << num_particulas << endl;
        return -5;
    }
    return 0;
}

int comparacion_cantidad_particulas(int num_particles, int particles_contadas){
    /*Esta comprobacion evalua si el numero de particulas indicado en cabecera coincide con el numero de particulas leidas en el archivo de entrada.
    En caso de que no coincidan, envia un mensaje de error y se devuelve -5*/
    if (num_particles != particles_contadas){
        cout << "Error: Number of particles mismatch. Header: " << num_particles << ", Found: " << particles_contadas << endl;
        return -5;
    }
    return 0;
}

int leer_archivo_entrada(string input_file){
    ifstream fichero;
    fichero.open(input_file, ios::binary);
    int num_particles, particles_contadas;
    //Mueve el puntero del archivo. Nos saltamos el valor de ppm para la lectura
    fichero.seekg(sizeof(float),ios::cur);
    fichero.read(reinterpret_cast<char*>(&num_particles), sizeof(int));
    while (!fichero.eof()){
        //Desplazamos puntero cada 9 valores, contando asi una particula
        fichero.seekg(9 * sizeof(float),ios::cur);
        particles_contadas +=1;
    }
    if (comparacion_cantidad_particulas(num_particles, particles_contadas) == -5){
        return -5;
    }
    return 0;
}


/*
int leido_con_exito(const string & arg2){
    // Imprime parametros
    cout << "Number of particles: " << obtener_num_particles(arg2) << std::endl;
    cout << "Particles per meter: " << static_cast<int>(ppm) << std::endl;
    cout << "Smoothing length: " << h << std::endl;
    cout << "Particle mass: " << particle_mass << std::endl;
    cout << "Grid size: " << nx << " x " << ny << " x " << nz << std::endl;
    cout << "Number of blocks: " << num_blocks << std::endl;
    cout << "Block size: " << sx << " x " << sy << " x " << sz << std::endl;
}
*/

int progargs(int argc, const string & arg1, const string & arg2, const string & arg3) {
    // Se llama a cada una de las funciones en las que se realizan las comprobaciones
    //Para la primera comprobacion se recibe como parametro el numero de argumentos del mandato
    //Para la segunda comprobacion se recibe como parametro el segundo valor del mandato, que corresponderia con el primer argumento que nos interesa
    
    int comp1 = comprobacion_num_param(argc);
    if (comp1 == -1){
        return -1;
    }
    if (comprobacion_primer_argumento(arg1) == -1){
        return -1;
    }
    //Para la tercera comprobacion se vuelve a recibir como parametro el segundo valor del mandato, que son los pasos de tiempo (primer argumento)
    int comp3 = comprobacion_pasos_tiempo(arg1);
    if (comp3 == -1){
        return -1;
    }
    //Para la cuarta comprobacion se recibe como parametro el tercer valor del mandato, que corresponde al archivo de entrada
    int comp4 = comprobacion_archivo_lectura(arg2);
    if (comp4 == -1){
        return -1;
    }
    //Para la quinta comprobacion se recibe como parametro el cuarto valor del mandato, que corresponde al archivo de salida
    int comp5 = comprobacion_archivo_escritura(arg3);
    if (comp5 == -1){
        return -1;
    }

    return 0;
} 
