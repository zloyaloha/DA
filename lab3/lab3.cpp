#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <sstream>
#include <algorithm>

const unsigned short MAX_WORD_SIZE = 16;

struct TWord {
    TWord() : size(0), stringId(0), wordId(0) {}
    char word[MAX_WORD_SIZE];
    int size;
    unsigned int stringId, wordId;
};

bool equal(const TWord &s1, const TWord &s2) {
    if (s1.size != s2.size) {
        return false;
    }
    for (int i = 0; i < s1.size; ++i) {
        if (s1.word[i] != s2.word[i]) {
            return false;
        }
    }
    return true;
}

std::vector<int> ZFuncKMP(const std::string &str) {
    std::vector<int> z(str.size(), 0);
    int left = 0, right = 0;
    for (int i = 1; i < str.size(); ++i) {
        if (i <= right) {
            z[i] = std::min(right - i + 1, z[i - left]);
        }
        while (i + z[i] < str.size() && str[z[i]] == str[i + z[i]]) {
			++z[i];
        }
		if (i + z[i] - 1 > right) {
			left = i;
            right = i + z[i] - 1;
        }
    }
    return z;
}

std::vector<int> PFunc(const std::string &str) {
    std::vector<int> p(str.size(), 0);
    std::vector<int> z(ZFuncKMP(str));
    for (int i = 0; i < str.size(); ++i) {
        for (int j = z[i] - 1; j >= 0; --j) {
            if (p[i + j] > 0) {
                break;
            } else {
                p[i + j] = j + 1;
            }
        }
    }
    return p;
}

std::vector<int> ZFunc(const std::vector<TWord> &str) {
    std::vector<int> z(str.size(), 0);
    int left = 0, right = 0;
    for (int i = 1; i < str.size(); ++i) {
        if (i <= right) {
            z[i] = std::min(right - i + 1, z[i - left]);
        }
        while (i + z[i] < str.size() && equal(str[z[i]], str[i + z[i]])) {
			++z[i];
        }
		if (i + z[i] - 1 > right) {
			left = i;
            right = i + z[i] - 1;
        }
    }
    return z;
}
std::vector<int> kmp(const std::string &pattern, const std::string &text) {
    std::vector<int> res;
    std::string txtpat = pattern + '#' + text;
    std::vector<int> p = PFunc(txtpat);
    for (int i = 0; i < p.size(); ++i) {
        if (p[i] == pattern.size()) {
            res.push_back(i - 2 * pattern.size());
        }
    }
    return res;
}
void search(std::vector<TWord> pattern, const std::vector<TWord> &text) {
    int pSize = pattern.size();
    TWord sentinel;
    sentinel.word[0] = '|';
    sentinel.size = 1;
    pattern.push_back(sentinel);
    for (const auto& elem: text) {
        pattern.push_back(elem);
    }
    std::vector<int> z = ZFunc(pattern);
    for (int i = 0; i < z.size(); ++i) {
        if (z[i] == pSize) {
            std::cout << text[i - pSize - 1].stringId << ", " << text[i - pSize - 1].wordId << '\n';
        }
    }
}

int main() {
    std::vector<int> asd = kmp("aba", "abacaba");
    std::vector<TWord> text, pattern;
    char c = getchar();
    bool flag = 1;
    TWord cur;
	unsigned short j = 0;
	while (c > 0) {
		if (c == '\n') {
			if (j > 0) {
				text.push_back(cur);
                cur.size = 0;
				j = 0;
			}
			++cur.stringId;
			if (flag) {
				std::swap(pattern, text);
				flag = 0;
				cur.stringId = 1;
			}
			cur.wordId = 1;
		} else if (c == '\t' or c == ' ') {
			if (j > 0) {
				text.push_back(cur);
				cur.size = 0;
				j = 0;
				++cur.wordId;
			}
		} else {
			if ('A' <= c && c <= 'Z') {
				c = c + 'a' - 'A';
			}
			cur.word[j] = c;
			++j;
            ++cur.size;
		}
	    c = getchar();
	}
	if (j > 0) {
		text.push_back(cur);
	}
    search(pattern, text);
}