#include <iostream>
#include <string>
const int SAMPLE_SIZE = 1000000;

std::string generateString() {
    std::string res = "";
    for (int i = 0; i < rand() % 2048; ++i) {
        res.push_back(char('!' + rand() % ('~' - '!' + 1)));
    }
    return res;
}

void generate() {
    for (int i = 0; i < SAMPLE_SIZE; ++i) {
        if (rand() % 100 < 10) {
            std::cout << '\n';
        }
        char p1 = char('A' + rand() % ('Z' - 'A' + 1));
        std::string p2{char('0' + rand() % ('9' - '0' + 1)), char('0' + rand() % ('9' - '0' + 1)), char('0' + rand() % ('9' - '0' + 1))};
        std::string p3{char('A' + rand() % ('Z' - 'A' + 1)), char('A' + rand() % ('Z' - 'A' + 1))};
        std::cout << p1 << ' ' << p2 << ' ' << p3 << '\t' << generateString() << '\n';
    }
}

int main() {
    srand(time(NULL));
    generate();
}