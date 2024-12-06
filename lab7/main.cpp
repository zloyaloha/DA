#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <chrono>

int getMaxArea(const std::vector<int> &vec) {
   std::stack<int> s;
    int res = 0;
    int top, cur, width;
    for (int i = 0; i < vec.size(); ++i) {
        while (!s.empty() && vec[s.top()] >= vec[i]) {
            top = s.top();
            s.pop();
            width = s.empty() ? i : i - s.top() - 1;
            res = std::max(res, vec[top] * width);
        }
        s.push(i);
    }

    while (!s.empty()) {
        top = s.top();
        s.pop();
        res = std::max(res, int(vec[top] * (s.empty() ? vec.size() : vec.size() - s.top() - 1)));
    }
    return res;
}

int biggestRectangle(std::vector<std::vector<int>> matrix, const int n, const int m) {
    int maxArea = getMaxArea(matrix[0]);
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] != 0) {
                matrix[i][j] = matrix[i - 1][j] + matrix[i][j];
            }
        }
        maxArea = std::max(maxArea, getMaxArea(matrix[i]));
    }
    return maxArea;
}

int main() {
    int n, m;
    std::string tmp;
    std::cin >> n >> m;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    for (int i = 0; i < n; ++i) {
        std::cin >> tmp;
        for (int j = 0; j < m; ++j) {
            matrix[i][j] = abs(int(tmp[j] - '1'));
        }
    }
    auto begin = std::chrono::steady_clock::now();
    std::cout << biggestRectangle(matrix, n, m);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";
    return 0;
}