#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>

std::vector<long long> changeCoins(const long long N, long long p, long long M) {
    std::vector<long long> coins(N);
    std::vector<long long> result(N, 0);
    coins[0] = 1;
    for (long long i = 1; i < N; ++i) {
        coins[i] = coins[i - 1] * p; 
    }
    for (long long i = coins.size() - 1; i >= 0; --i) {
        result[i] += M / coins[i];
        M %= coins[i];
    }
    return result;
}

int main() {
    long long N, p, M;
    std::cin >> N >> p >> M;
    auto begin = std::chrono::steady_clock::now();
    std::vector<long long> res(changeCoins(N, p, M));
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";
    for (long long i = 0; i < res.size(); ++i) {
        std::cout << res[i] << '\n';
    }
}