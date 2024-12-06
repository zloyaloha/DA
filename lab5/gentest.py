import random
import string

def generate_test_case(length: int, alphabet_size: int = 26) -> str:

    alphabet = string.ascii_lowercase[:alphabet_size]  # Ограничиваем алфавит
    return ''.join(random.choice(alphabet) for _ in range(length))

for i in range(1):
    print(generate_test_case(random.randrange(1, 2500000)))