import sys

def obtener_indices_desde_id(identificador, nx, ny, nz):
    x = identificador % nx
    y = (identificador // nx) % ny
    z = identificador // (nx * ny)
    return x, y, z

if len(sys.argv) != 5:
    print("Usage: python script.py <id> <nx> <ny> <nz>")
    sys.exit(1)

id_deseado = int(sys.argv[1]) - 1
nx = int(sys.argv[2])
ny = int(sys.argv[3])
nz = int(sys.argv[4])

if id_deseado < 0 or id_deseado >= nz * ny * nz:
    print("Error: El ID proporcionado está fuera del rango válido.", id_deseado, nz * ny * nz)
    sys.exit(1)

x_result, y_result, z_result = obtener_indices_desde_id(id_deseado, nx, ny, nz)

print(f"Las coordenadas para el ID {id_deseado} son: ({x_result}, {y_result}, {z_result})")

