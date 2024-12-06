import random
import sys

def generate_random_matrix(m, n):
    return [[random.choice([0, 1]) for _ in range(n)] for _ in range(m)]

def write_matrix_to_file(matrix, filename):
    with open(filename, 'w') as f:
        f.write(f"{len(matrix)} {len(matrix[0])}\n")  # Записываем размеры матрицы
        for row in matrix:
            f.write(''.join(map(str, row)) + '\n')  # Записываем строки матрицы

def main():
    if len(sys.argv) != 4:
        print("Usage: python script.py <rows> <columns> <filename>")
        return
    
    try:
        m = int(sys.argv[1])
        n = int(sys.argv[2])
        filename = sys.argv[3]

        if m <= 0 or n <= 0:
            raise ValueError("Размеры матрицы должны быть положительными целыми числами.")

    except ValueError as e:
        print(f"Ошибка: {e}")
        return

    matrix = generate_random_matrix(m, n)
    write_matrix_to_file(matrix, filename)
    print(f"Матрица размером {m}x{n} записана в файл '{filename}'.")

if __name__ == "__main__":
    main()
