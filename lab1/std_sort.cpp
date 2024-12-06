#include <iostream>
#include <algorithm>
#include <string>
#include <chrono>

std::string makeString(std::string &str) {
    std::string res = "";
    for (int i = 0; i < 8; ++i) {
        if (str[i] != ' ') {
            res += str[i];
        }
    }
    return res;
} 

int main() {
    std::vector<std::pair<std::string, int>> nums;
    std::string tmp;
    int counter = 0;
    while (std::getline(std::cin, tmp)) {
        nums.push_back(std::make_pair(makeString(tmp), counter++));
    }
    auto begin = std::chrono::steady_clock::now();
    std::sort(nums.begin(), nums.end());
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";
}