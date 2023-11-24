#include "./sim/progargs.hpp"

#include <gtest/gtest.h>
using namespace std;

// Tests para la funcion llamada comprobacion_num_param
TEST(progargs_num_params_test, num_correcto_params) {
  int const num_params = 4;
  int const funcion    = comprobacion_num_param(num_params);
  EXPECT_EQ(0, funcion);
}

TEST(progargs_num_paramas_test, num_incorrecto_params_1) {
  int const num_params = 1;
  int const funcion    = comprobacion_num_param(num_params);
  EXPECT_EQ(-1, funcion);
}

TEST(progargs_num_params_test, num_incorrecto_params_2) {
  int const num_params = 2;
  int const funcion    = comprobacion_num_param(num_params);
  EXPECT_EQ(-1, funcion);
}

TEST(progargs_num_params_test, num_incorrecto_params_3) {
  int const num_params = 3;
  int const funcion    = comprobacion_num_param(num_params);
  EXPECT_EQ(-1, funcion);
}

// Tests para la funcion llamada comprobacion_primer_argumento
TEST(progargs_primer_argumento_test, primer_argumento_int) {
  string const param_int = "2000";
  int const funcion      = comprobacion_primer_argumento(param_int);
  EXPECT_EQ(0, funcion);
}

TEST(progargs_primer_argumento_test, primer_argumento_no_int) {
  string const param_str = "hola";
  int const funcion      = comprobacion_primer_argumento(param_str);
  EXPECT_EQ(-1, funcion);
}

// Tests para la funcion llamada comprobacion_pasos_tiempo
TEST(progargs_pasos_tiempo_test, num_pasos_tiempo_correcto) {
  string const param_num = "100";
  int const funcion      = comprobacion_pasos_tiempo(param_num);
  EXPECT_EQ(0, funcion);
}

TEST(progargs_pasos_tiempo_test, num_pasos_tiempo_incorrecto) {
  string const param_num_negativo = "-300";
  int const funcion               = comprobacion_pasos_tiempo(param_num_negativo);
  EXPECT_EQ(-2, funcion);
}

// Tests para la comprobacion_archivo_lectura
TEST(progargs_archivo_lectura_test, archivo_lectura_valido) {
  // Para la realizacion de la prueba, se pasa como string la ruta del archivo existente
  string const archivo_valido = "../../files/out/small-2.fld";
  int const funcion           = comprobacion_archivo_lectura(archivo_valido);
  EXPECT_EQ(0, funcion);
}

TEST(progargs_archivo_lectura_test, archivo_lectura_no_valido) {
  // A diferencia de la anterior, como simplemente no debe existir el archivo, se inventara un
  // nombre para un txt, en vez de pasar la ruta
  string const archivo_no_valido = "malo.txt";
  int const funcion              = comprobacion_archivo_lectura(archivo_no_valido);
  EXPECT_EQ(-3, funcion);
}

// Tests para la funcion llamada comprobacion_archivo_escritura
TEST(progargs_archivo_escritura_test, archivo_escritura_valido) {
  string const archivo_valido = "../../files/archivos_pruebas/test_escritura.fld";
  int const funcion           = comprobacion_archivo_escritura(archivo_valido);
  EXPECT_EQ(0, funcion);
}

TEST(progargs_archivo_escritura_test, archivo_escritura_no_valido) {
  string const archivo_no_valido = "pepe.fld";
  int const funcion              = comprobacion_archivo_escritura(archivo_no_valido);
  EXPECT_EQ(-4, funcion);
}

// Tests para la funcion llamada iniciacion_simulacion
TEST(progargs_iniciacion_simulacion_test, num_particulas_valido) {
  int const num_particles_valido = 100;
  int const funcion              = iniciacion_simulacion(num_particles_valido);
  EXPECT_EQ(0, funcion);
}

// Tests para la funcion llamada iniciacion_simulacion
TEST(progargs_iniciacion_simulacion_test, num_particulas_no_valido_zero) {
  int const num_particles_no_valido_cero = 0;
  int const funcion                      = iniciacion_simulacion(num_particles_no_valido_cero);
  EXPECT_EQ(-5, funcion);
}

TEST(progargs_iniciacion_simulacion_test, num_particulas_no_valido_neg) {
  int const num_particles_no_valido_negativo = -100;
  int const funcion = iniciacion_simulacion(num_particles_no_valido_negativo);
  EXPECT_EQ(-5, funcion);
}

// Tests para la funcion llamada comparacion_cantidad_particulas
TEST(progargs_comp_cant_particulas_test, num_particulas_coincide) {
  int const num_particles      = 100;
  int const particles_contadas = 100;
  int const funcion            = comparacion_cantidad_particulas(num_particles, particles_contadas);
  EXPECT_EQ(0, funcion);
}

TEST(progargs_comp_cant_particulas_test, num_particulas_no_coincide) {
  int const num_particles      = 150;
  int const particles_contadas = 100;
  int const funcion            = comparacion_cantidad_particulas(num_particles, particles_contadas);
  EXPECT_EQ(-5, funcion);
}
