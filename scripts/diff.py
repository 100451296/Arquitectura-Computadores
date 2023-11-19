import sys
import struct
import shutil

class Particle:
    def __init__(self):
        self.id = 0
        self.posX = 0.0
        self.posY = 0.0
        self.posZ = 0.0
        self.smoothVecX = 0.0
        self.smoothVecY = 0.0
        self.smoothVecZ = 0.0
        self.velX = 0.0
        self.velY = 0.0
        self.velZ = 0.0

class GridComparator:
    def __init__(self):
        self.particles1 = []
        self.particles2 = []

    def compareFiles(self, file1, file2):
        self.readFile(file1, self.particles1)
        self.readFile(file2, self.particles2)
        different_particles = self.findDifferentParticles()
        return different_particles

    def readFile(self, input_file_name, particles_list):
        with open(input_file_name, 'rb') as input_file:
            grid = Grid()
            if not grid.readHeader(input_file):
                return -1

            if grid.num_particles <= 0:
                print("Número de partículas inválido")
                return -1

            for i in range(grid.num_particles):
                particle = Particle()
                if not grid.readParticle(input_file, particle, i):
                    return -1
                particles_list.append(particle)

    def findDifferentParticles(self):
        different_particles = []
        if len(self.particles1) != len(self.particles2):
          print("Error longitud archivos")
          return -1
        for i in range(len(self.particles1)):
          if not self.isSameParticle(self.particles1[i], self.particles2[i]):
              different_particles.append((self.particles1[i], self.particles2[i]))
          
        return different_particles

    def isSameParticle(self, particle1, particle2):
        return (
            particle1.posX == particle2.posX and
            particle1.posY == particle2.posY and
            particle1.posZ == particle2.posZ and
            particle1.smoothVecX == particle2.smoothVecX and
            particle1.smoothVecY == particle2.smoothVecY and
            particle1.smoothVecZ == particle2.smoothVecZ and
            particle1.velX == particle2.velX and
            particle1.velY == particle2.velY and
            particle1.velZ == particle2.velZ
        )

class Grid:
    def readHeader(self, input_file):
        self.ppm = struct.unpack('f', input_file.read(4))[0]
        self.num_particles = struct.unpack('i', input_file.read(4))[0]
        return True

    def readParticle(self, input_file, particle, index):
        buffer = input_file.read(36)  # 9 floats, each of 4 bytes
        if len(buffer) != 36:
            print("Error al leer las partículas del archivo")
            return False

        values = struct.unpack('9f', buffer)
        (
            particle.posX,
            particle.posY,
            particle.posZ,
            particle.smoothVecX,
            particle.smoothVecY,
            particle.smoothVecZ,
            particle.velX,
            particle.velY,
            particle.velZ
        ) = values

        particle.id = index
        return True

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Por favor, proporcione dos nombres de archivo como argumentos.")
        sys.exit(1)

    file1 = sys.argv[1]
    file2 = sys.argv[2]

    verbose = False
    if len(sys.argv) == 4 and (sys.argv[3] == "-v" or sys.argv[3] == "--verbose"):
        verbose = True


    comparator = GridComparator()
    results = comparator.compareFiles(file1, file2)
    terminal_width = shutil.get_terminal_size().columns

    if not verbose:
      print(f"Se encontraron {len(results)} partículas distintas de {len(comparator.particles1)}.")
    else:
      for particle in results:
          print("#" * terminal_width + "\n\n"
                f"Partícula {file1}: \n"
                f"     ID: {particle[0].id}\n"
                f"     Posición: ({particle[0].posX}, {particle[0].posY}, {particle[0].posZ})\n"
                f"     Vector Suavizado: ({particle[0].smoothVecX}, {particle[0].smoothVecY}, {particle[0].smoothVecZ})\n"
                f"     Velocidad: ({particle[0].velX}, {particle[0].velY}, {particle[0].velZ})\n\n"
                f"Partícula {file2}: \n"
                f"     ID: {particle[1].id}\n"
                f"     Posición: ({particle[1].posX}, {particle[1].posY}, {particle[1].posZ})\n"
                f"     Vector Suavizado: ({particle[1].smoothVecX}, {particle[1].smoothVecY}, {particle[1].smoothVecZ})\n"
                f"     Velocidad: ({particle[1].velX}, {particle[1].velY}, {particle[1].velZ})\n\n")


