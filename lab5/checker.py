import string
import sys

def minimal_lexicographic_cut_cyclic(s: str) -> str:
    n = len(s)
    s = s + s
    min_cut = s[:n]
    for i in range(1, n):
        current_cut = s[i:i + n]
        if current_cut < min_cut:
            min_cut = current_cut
    
    return min_cut

def process_input():

    input_data = sys.stdin.read()  # Считываем весь поток ввода до EOF
    lines = input_data.strip().splitlines()  # Разделяем ввод на строки

    for line in lines:
        print(minimal_lexicographic_cut_cyclic(line))

# Запуск функции обработки ввода
if __name__ == "__main__":
    process_input()