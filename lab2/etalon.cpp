#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

int main() {
    std::map<std::string, unsigned long long> rb;
    std::string in;
    unsigned long long i;
    std::ofstream out("res1.txt");
    auto begin = std::chrono::steady_clock::now();
    while (std::cin >> in) {
        if (in[0] == '+') {
            std::cin >> in;
            std::cin >> i;
            rb.insert(std::make_pair(in, i));
        } else if (in[0] == '-') {
            std::cin >> in;
            auto res = rb.find(in);
            if (res != rb.end()) {
                rb.erase(res);
            }
        } else {
            auto res = rb.find(in);
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";
}