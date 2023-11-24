# Arquitectura-Computadores

## Video de ejemplo
  [Este vídeo](https://www.youtube.com/watch?v=FRoIgCHV93U) es una simulación de lo que vamos a programar. Se ve que las partículas se agrupan en bloques y se ubican dentro de la malla. En cada iteración los distintos parámetros afectan a como se colisionan y mueven los bloques de partículas.
  
## Compilacion
  1) Después de clonar el proyecto ejecuta ```bash build.sh```. Esto crea la carpeta build y genera una carpeta por cada componente. En esta carpeta se compila el codigo fuente.
  2) Dentro de la carpeta build ejecuta ```make```. Esto compila el proyecto entero. Si solo quieres compilar un componente, ejecuta ```make``` dentro de su carpeta en build.

## Utest 
  Ejecutar ./utest o ctest dentro de la carpeta de compilación

## Ftest
  Los test deben ejecutarse dentro de la carpeta ftest/ para que resuelva correctamente las rutas

## Scripts
  Los scripts dentro del directorio scripts/ para medir el rendimiento y energia de la aplicación deben ejecutarse en ese directorio ya que contiene rutas relativas desde esa ubicacion.

