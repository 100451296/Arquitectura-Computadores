#include <gtest/gtest.h>
#include "./sim/progargs.hpp"
using namespace std;

/*
// Esta prueba se enfoca en 
TEST(progargs, com_num_param) {
    int num_params;
    string const num_iter;
    string const input;
    string const output;
    int funcion = proargs_validations(num_params,num_iter,input,output);

    EXPECT_EQ(-1, funcion);
}
*/

// Tests para la funcion llamada comprobacion_num_param
TEST(progargs_num_params_test, num_correcto_params) {
    int num_params = 4;
    int funcion = comprobacion_num_param(num_params);
    EXPECT_EQ(0, funcion);
}

TEST(progargs_num_paramas_test, num_incorrecto_params_1) {
    int num_params = 1;
    int funcion = comprobacion_num_param(num_params);
    EXPECT_EQ(-1, funcion);
}

TEST(progargs_num_params_test, num_incorrecto_params_2) {
    int num_params = 2;
    int funcion = comprobacion_num_param(num_params);
    EXPECT_EQ(-1, funcion);
}

TEST(progargs_num_params_test, num_incorrecto_params_3) {
    int num_params = 3;
    int funcion = comprobacion_num_param(num_params);
    EXPECT_EQ(-1, funcion);
}

// Tests para la funcion llamada comprobacion_primer_argumento
TEST(progargs_primer_argumento_test, primer_argumento_int) {
    string param_int = "2000";
    int funcion = comprobacion_primer_argumento(param_int);
    EXPECT_EQ(0, funcion);
}

TEST(progargs_primer_argumento_test, primer_argumento_no_int) {
    string param_str = "hola";
    int funcion = comprobacion_primer_argumento(param_str);
    EXPECT_EQ(-1, funcion);
}

// Tests para la funcion llamada comprobacion_pasos_tiempo
TEST(progargs_pasos_tiempo_test, num_pasos_tiempo_correcto) {
    string param_num = "100";
    int funcion = comprobacion_pasos_tiempo(param_num);
    EXPECT_EQ(0, funcion);
}

TEST(progargs_pasos_tiempo_test, num_pasos_tiempo_incorrecto) {
    string param_num_negativo = "-300";
    int funcion = comprobacion_pasos_tiempo(param_num_negativo);
    EXPECT_EQ(-2, funcion);
}

// Tests para la comprobacion_archivo_lectura
TEST(progargs_archivo_lectura_test, archivo_lectura_valido) {
    //Para la realizacion de la prueba, se pasa como string la ruta del archivo existente
    string archivo_valido = "../../files/out/small-2.fld";
    int funcion = comprobacion_archivo_lectura(archivo_valido);
    EXPECT_EQ(0, funcion);
}

TEST(progargs_archivo_lectura_test, archivo_lectura_no_valido) {
    //A diferencia de la anterior, como simplemente no debe existir el archivo, se inventara un nombre para un txt, en vez de pasar la ruta
    string archivo_no_valido = "malo.txt";
    int funcion = comprobacion_archivo_lectura(archivo_no_valido);
    EXPECT_EQ(-3, funcion);
}

// Tests para la funcion llamada comprobacion_archivo_escritura
TEST(progargs_archivo_escritura_test, archivo_escritura_valido) {
    string archivo_valido = "../../files/out/large-1.fld";
    int funcion = comprobacion_archivo_escritura(archivo_valido);
    EXPECT_EQ(0, funcion);
}

TEST(progargs_archivo_escritura_test, archivo_escritura_no_valido) {
    string archivo_no_valido = "pepe.fld";
    int funcion = comprobacion_archivo_escritura(archivo_no_valido);
    EXPECT_EQ(-4, funcion);
}

// Tests para la funcion llamada iniciacion_simulacion
TEST(progargs_iniciacion_simulacion_test, num_particulas_valido) {
    int num_particles_valido = 100;
    int funcion = iniciacion_simulacion(num_particles_valido);
    EXPECT_EQ(0, funcion);
}

//Tests para la funcion llamada iniciacion_simulacion
TEST(progargs_iniciacion_simulacion_test, num_particulas_no_valido_zero) {
    int num_particles_no_valido_cero = 0;
    int funcion = iniciacion_simulacion(num_particles_no_valido_cero);
    EXPECT_EQ(-5, funcion);
}

TEST(progargs_iniciacion_simulacion_test, num_particulas_no_valido_neg) {
    int num_particles_no_valido_negativo = -100;
    int funcion = iniciacion_simulacion(num_particles_no_valido_negativo);
    EXPECT_EQ(-5, funcion);
}

// Tests para la funcion llamada comparacion_cantidad_particulas
TEST(progargs_comp_cant_particulas_test, num_particulas_coincide) {
    int num_particles = 100;
    int particles_contadas = 100;
    int funcion = comparacion_cantidad_particulas(num_particles, particles_contadas);
    EXPECT_EQ(0, funcion);
}

TEST(progargs_comp_cant_particulas_test, num_particulas_no_coincide) {
    int num_particles = 150;
    int particles_contadas = 100;
    int funcion = comparacion_cantidad_particulas(num_particles, particles_contadas);
    EXPECT_EQ(-5, funcion);
}
