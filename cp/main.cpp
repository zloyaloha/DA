#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>
#include <list>
#include <array>
#include <algorithm>
#include <cmath>

namespace {
    const int MAX_SEQ_SIZE = 255;
    const int ALPHAVITE_SIZE = 257;
}

class RLEEncoder {
    public:
        RLEEncoder(const std::string& input);
        std::vector<std::pair<int, char>> RLEEncode();
    private:
        std::vector<std::pair<int, char>> encoded;
};

RLEEncoder::RLEEncoder(const std::string& input) {
    int counter = 1;
    for (int i = 1; i < input.size(); ++i) {
        if (input[i] == input[i - 1]) {
            ++counter;
        } else {
            encoded.emplace_back(std::make_pair(counter, input[i - 1]));
            counter = 1;
        }
    }
    encoded.emplace_back(std::make_pair(counter, input[input.size() - 1]));
}

std::vector<std::pair<int, char>> RLEEncoder::RLEEncode() {
    return encoded;
}

class RLEDecoder {
    public:
        RLEDecoder(const std::vector<std::pair<int, char>>& symbs);
        std::string RLEDecode();
    private:
        std::string decoded;
};

RLEDecoder::RLEDecoder(const std::vector<std::pair<int, char>>& symbs) : decoded("") {
    for (const auto &[amount, symb]: symbs) {
        for (int i = 0; i < amount; ++i) {
            decoded += symb;
        }
    }
}

std::string RLEDecoder::RLEDecode() {
    return decoded;
}

class SuffixArray {
    private:
        bool FirstStep();
        std::vector<int> rank;
        std::string text;
    public:
        std::vector<int> indexes; 
        void Build();
        ~SuffixArray() = default;
        SuffixArray(const std::string& s);
};

bool SuffixArray::FirstStep() {
    int n = text.size();
    if (n == 0) {
        return false;
    }
    std::vector<int> helper(ALPHAVITE_SIZE, 0);

    for (char symb: text) {
        ++helper[symb];
    }
    for (int i = 1; i < ALPHAVITE_SIZE; ++i) {
        helper[i] += helper[i - 1];
    }

    for (int i = n - 1; i >= 0; --i) {
        indexes[--helper[text[i]]] = i;
    }

    rank[indexes[0]] = 0;
    for (int i = 1; i < n; ++i) {
        if (text[indexes[i]] == text[indexes[i - 1]]) {
            rank[indexes[i]] = rank[indexes[i - 1]];
        } else {
            rank[indexes[i]] = rank[indexes[i - 1]] + 1;
        }
    }
    return true;
}

void SuffixArray::Build() {
    if (!FirstStep()) {
        return;
    }
    int n = text.size();
    std::vector<int> ind(n, 0);
    std::vector<int> tmpRank(n, 0);
    std::vector<int> helper(n, 0);
    for (int step = 1; step < n; step = step << 1) {
        for (int i = 0; i < n; ++i) {
            ind[i] = (indexes[i] - step + n) % n; // индекс предыдущего элемента в циклическом смысле
        }
        // Сортировка подсчетом по классам эквивалентности.
        for (int i = 0; i < n; ++i) {
            tmpRank[i] = rank[ind[i]];
        }
        std::fill(helper.begin(), helper.end(), 0);
        for (int i = 0; i < n; ++i) {
            ++helper[tmpRank[i]];
        }
        for (int i = 1; i < n; ++i) {
            helper[i] += helper[i - 1];
        }
        for (int i = n - 1; i >= 0; --i) {
            indexes[--helper[tmpRank[i]]] = ind[i];
        }
        // Пересчитываем класс эквивалентности
        std::vector<std::pair<int, int>> rankNowAndPrev(n, {0, 0});
        for (int i = 0; i < n; ++i) {
            rankNowAndPrev[i].first = rank[indexes[i]]; 
            rankNowAndPrev[i].second = rank[(indexes[i] + step) % n];
        }
        rank[indexes[0]] = 0;
        bool flag = true;
        for (int i = 1; i < n; ++i) {
            if (rankNowAndPrev[i] == rankNowAndPrev[i - 1]) {
                rank[indexes[i]] = rank[indexes[i - 1]];
                flag = false;
            } else {
                rank[indexes[i]] = rank[indexes[i - 1]] + 1;
            }
        }
        if (flag) {
            return;
        }
    }
}

SuffixArray::SuffixArray(const std::string& s) : indexes(s.size(), 0), rank(s.size(), 0), text(s) {}

class BWTEncoder {
    public:
        BWTEncoder(const std::string& input);
        std::string BWTEncode();
    private:
        std::string text;
        std::string table;
        SuffixArray suf;
};

BWTEncoder::BWTEncoder(const std::string& input) : text(input + input), suf(text) {}

std::string BWTEncoder::BWTEncode() {
    suf.Build();
    std::string result = "";
    int real_size = text.size() / 2;
    for (int i = 0; i < suf.indexes.size(); ++i) {
        if (suf.indexes[i] >= real_size) continue;
        result += text.substr(suf.indexes[i], real_size)[real_size - 1];
    }
    return result;
}

class BWTDecoder {
    public:
        BWTDecoder(const std::string& input);
        std::string BWTDecode();
    private:
        std::string text;
        std::vector<std::string> table;
};

BWTDecoder::BWTDecoder(const std::string& input) : text(input.size(), ' ') {
    int lenBwt = input.length();
    std::string sortedBwt = input;
    int x = input.find('$');
    sort(sortedBwt.begin(), sortedBwt.end());
    std::vector<int> lShift(lenBwt, 0);
    std::vector<std::list<int>> arr(128);

    for (int i = 0; i < lenBwt; i++) {
        arr[input[i]].push_back(i);
    }

    for (int i = 0; i < lenBwt; i++) {
        lShift[i] = arr[sortedBwt[i]].front();
        arr[sortedBwt[i]].pop_front(); 
    }

    for (int i = 0; i < lenBwt - 1; i++) {
        x = lShift[x];
        text[i] = input[x];
    }
}

std::string BWTDecoder::BWTDecode() {
    return text;
}

class MTFEncoder {
    public:
        MTFEncoder(const std::string& str);
        std::string MTFEncode();
    private:
        std::list<char> alphabet;
        std::string text;
};

MTFEncoder::MTFEncoder(const std::string& str) : text(str) {
    alphabet.push_back('$');
    for (char i = 'a'; i <= 'z'; ++i) {
        alphabet.push_back(i);
    }
};

std::string MTFEncoder::MTFEncode() {
    std::string encoded = "";
    encoded.reserve(text.size());
    for (char c : text) {
        int position = 0;
        for (auto it = alphabet.begin(); it != alphabet.end(); ++it, ++position) {
            if (*it == c) {
                encoded.push_back(position);

                alphabet.erase(it);
                alphabet.push_front(c);
                break;
            }
        }
    }
    return encoded;
}

class MTFDecoder {
    public:
        MTFDecoder(const std::string& str);
        std::string MTFDecode();
    private:
        std::list<char> alphabet;
        std::string text;
};

MTFDecoder::MTFDecoder(const std::string& str) : text(str) {
    alphabet.push_back('$');
    for (char i = 'a'; i <= 'z'; ++i) {
        alphabet.push_back(i);
    }
};

std::string MTFDecoder::MTFDecode() {
    std::string encoded = "";
    encoded.reserve(text.size());
    for (char c : text) {
        int position = 0;
        for (auto it = alphabet.begin(); it != alphabet.end(); ++it, ++position) {
            if (position == c) {
                encoded += (*it);

                alphabet.erase(it);
                alphabet.push_front(*it);
                break;
            }
        }
    }
    return encoded;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::vector<std::pair<int, char>> result_rle, input;
    std::string text, type, str;
    std::cin >> type;
    if (type == "compress") {
        std::cin >> text;
        text += '$';
        BWTEncoder bwt(text);
        str = bwt.BWTEncode();
        MTFEncoder mwt(str);
        str = mwt.MTFEncode();
        RLEEncoder rle(str);
        result_rle = rle.RLEEncode();
        for (auto &[counter, symb]: result_rle) {
            std::cout << counter << ' ' << int(symb) << '\n';
        }
    }
    if (type == "decompress") {
        int tmp1, tmp2;
        while (std::cin >> tmp1 >> tmp2) {
            input.push_back(std::make_pair(tmp1, tmp2));
        }
        RLEDecoder rle(input);
        str = rle.RLEDecode();
        MTFDecoder mtf(str);
        str = mtf.MTFDecode();
        BWTDecoder bwt(str);
        str = bwt.BWTDecode();
        std::cout << str << '\n';
    }
}