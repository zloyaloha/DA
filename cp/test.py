import random
import string

def generate_random_strings(filename, num_strings=1, max_length=100):
    with open(filename, 'w') as file:
        # Записываем "compress" в начале файла
        file.write("compress\n")
        for _ in range(num_strings):
            # Случайная длина строки от 0 до max_length
            length = random.randint(0, max_length)
            # Генерация строки из случайных маленьких латинских букв
            random_string = ''.join(random.choices(string.ascii_lowercase, k=length))
            # Запись строки в файл с новой строкой
            file.write(random_string + '\n')

# Генерация файла test.txt с 10 случайными строками
generate_random_strings('test.txt', num_strings=2, max_length=10000)
