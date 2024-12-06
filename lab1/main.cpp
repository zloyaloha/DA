#include <iostream>
#include <string.h>
#include <fstream>
#include <chrono>
#include <memory>

const short AMOUNT_OF_LETTERS = 26;
const short CAR_NUMBER_SIZE = 6;
const short AMOUNT_OF_DIGITS = 10;
const short MAX_STRING_SIZE = 2048;

#pragma region TVec
template <typename T>
class TVector {
    private:
        T *_container;
        size_t _size;
        size_t _capacity;
    public:
        friend std::ostream& operator<<(std::ostream &os, const TVector &vec) {
            for (size_t i = 0; i < vec.Size(); ++i) {
                os << vec[i];
            }
            return os;
        }
        TVector();
        TVector(const TVector &other);
        TVector(const size_t &capacity);
        TVector(const T* arr, size_t finish);
        TVector(TVector &&other) noexcept;
        void PrintString() const;
        ~TVector();
        const T& operator[](const size_t &idx) const;
        T& operator[](const size_t &idx);
        TVector<T> &operator=(const TVector &other);
        TVector<T> &operator=(TVector &&other) noexcept;
        void PushBack(const T &elem);
        void PushBack(T &&elem) noexcept;
        size_t Size() const;
        size_t Capacity() const;
        void Reserve(const size_t &size);
};

template <typename T>
size_t TVector<T>::Capacity() const {
    return _capacity;
}

template <typename T>
TVector<T>::TVector(const T* arr, size_t finish) : _size(finish), _capacity(finish), _container{new T[finish]} {
    memcpy(_container, arr, sizeof(T) * finish);
}

template <typename T>
TVector<T>::TVector(const size_t &capacity) : _size(0), _capacity(capacity), _container{new T[capacity]{}} {}

template <typename T>
TVector<T>::TVector() : _container(nullptr), _size(0), _capacity(0) {}

template <typename T>
void TVector<T>::Reserve(const size_t &capacity) {
    T* tmpContainer = new T[capacity];
    for (short i = 0; i < _size; ++i) {
        tmpContainer[i] = _container[i];
    }
    delete[] _container;
    _container = tmpContainer;
    _capacity = capacity;
}

template <typename T>
TVector<T>::TVector(TVector<T> &&other) noexcept :  _size(other._size), _capacity(other._capacity), _container{other._container} {
    other._size = 0;
    other._capacity = 0;
    other._container = nullptr;
}

template <typename T>
void TVector<T>::PrintString() const {
    printf("%s", _container);
}

template <typename T>
TVector<T>::TVector(const TVector<T> &other) : _size(other._size), _capacity(other._capacity) {
    delete[] _container;
    _container = new T[_capacity];
    memcpy(_container, other._container, sizeof(T) * other.Size());
}

template <typename T>
TVector<T>::~TVector() {
    _size = 0;
    _capacity = 0;
    delete[] _container;
    _container = nullptr;
}

template <typename T>
T& TVector<T>::operator[](const size_t &idx) {
    return _container[idx];
}

template <typename T>
const T& TVector<T>::operator[](const size_t &idx) const{
    return _container[idx];
}

template <typename T>
TVector<T> &TVector<T>::operator=(const TVector<T> &other) {
    if (_size >= other._size) {
        for (int i = 0; i < other.Size(); i++) {
            _container[i] = other[i];
        }
        _size = other._size;
    } else {
        delete[] _container;
        _size = other._size;
        _capacity = other._capacity;
        _container = new T[_capacity];
        memcpy(_container, other._container, sizeof(T) * other.Size());
    }
    return *this;
}

template <typename T>
TVector<T> &TVector<T>::operator=(TVector<T> &&other) noexcept{
    _size = other._size;
    _capacity = other._capacity;
    _container = other._container;
    other._size = 0;
    other._capacity = 0;
    other._container = nullptr;
    return *this;
}

template <typename T>
size_t TVector<T>::Size() const {
    return _size;
}

template <typename T>
void TVector<T>::PushBack(const T& elem) {
    if (_size < _capacity) {
        _container[_size] = elem;
    } else {
        _capacity = _capacity * 1.5 + 1;
        T* newArray = new T[_capacity];
        memcpy(newArray, _container, sizeof(T) * _size);
        delete[] _container;
        newArray[_size] = elem;
        _container = newArray;
    }
    ++_size;
}

template <typename T>
void TVector<T>::PushBack(T &&elem) noexcept {
    if (_size < _capacity) {
        _container[_size] = elem;
    } else {
        _capacity = _capacity * 1.5 + 1;
        T* newArray = new T[_capacity];
        for (int i = 0; i < _size; ++i) {
            newArray[i] = std::move(_container[i]);
        }
        delete[] _container;
        newArray[_size] = std::move(elem);
        _container = newArray;
    }
    ++_size;
}

#pragma endregion TVec

#pragma region Sorting

struct TItem {
    char num[6];
    size_t idx;
};

void SetZeros(TVector<int> &vec, short size) {
    for (int i = 0; i < size; ++i) {
        vec[i] = 0;
    }
}

bool IsDigit(char c) {
    if (c >= '0' && c <= '9') {
        return true;
    }
    return false;
}

void RadixSort(TVector<TItem> &vec) {
    if (vec.Size() == 0 || vec.Size() == 1) {
        return;
    }
    TVector<int> count(AMOUNT_OF_LETTERS);
    SetZeros(count, AMOUNT_OF_LETTERS);
    TVector<TItem> out(vec.Size());
    int j;
    char changerType;
    int i, sizeCount;
    for (i = CAR_NUMBER_SIZE - 1; i >= 0; --i) {
        if (IsDigit(vec[0].num[i])) {
            sizeCount = AMOUNT_OF_DIGITS;
            changerType = '0';
        } else {
            sizeCount = AMOUNT_OF_LETTERS;
            changerType = 'A';
        }
        SetZeros(count, sizeCount);
        for (j = 0; j < vec.Size(); ++j) {
            ++count[vec[j].num[i] - changerType];
        }
        for (j = 1; j < sizeCount; ++j) {
            count[j] += count[j - 1];
        }
        for (j = vec.Size() - 1; j >= 0; --j) {
            out[--count[vec[j].num[i] - changerType]] = std::move(vec[j]);
        }
        for (int i = 0; i < vec.Size(); i++) {
            vec[i] = std::move(out[i]);
        }
    }  
}

#pragma endregion Sorting

#pragma region ReadWrite
int ReadCarNum(char *tmpStr, char del) {
    char c;
    int i = 0;
    while (true) {
        c = getchar();
        if (c == EOF) {
            return -1;
        } else if (c == '\n') {
            break;
        } else if (c == del) {
            break;
        } else if (c == ' ') {
            continue;
        } else {
            tmpStr[i] = c;
        }
        ++i;
    }
    return i;
}

TVector<char> ReadData() {
    char tmpStr[MAX_STRING_SIZE];
    char c;
    int i = 0;
    while (true) {
        c = getchar();
        if (c == EOF) {
            break;
        } else if (c == '\n') {
            break;
        } else {
            tmpStr[i] = c;
        }
        ++i;
    }
    tmpStr[i] = '\0';
    return TVector<char>(tmpStr, i + 1);
}

void Print(const TVector<TItem> &carNums, const TVector<TVector<char>> &dataVec) {
    for (int i = 0; i < carNums.Size(); ++i) {
        putchar(carNums[i].num[0]);
        putchar(' ');
        putchar(carNums[i].num[1]);
        putchar(carNums[i].num[2]);
        putchar(carNums[i].num[3]);
        putchar(' ');
        putchar(carNums[i].num[4]);
        putchar(carNums[i].num[5]);
        putchar('\t');
        dataVec[carNums[i].idx].PrintString();
        putchar('\n');
    }
}

#pragma endregion ReadWrite

int main() {
    TVector<TItem> carNums(100000);
    TVector<TVector<char>> dataVec(100000);
    int size;
    size_t idxGlobal = 0;
    TItem n;
    n.idx = 0;
    while (true) {
        size = ReadCarNum(n.num, '\t');
        if (size == -1) {
            break;
        }
        if (size < CAR_NUMBER_SIZE) {
            continue;
        }
        carNums.PushBack(std::move(n));
        dataVec.PushBack(std::move(ReadData()));
        ++idxGlobal;
        n.idx = idxGlobal;
    }
    auto begin = std::chrono::steady_clock::now();
    RadixSort(carNums);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";
    // Print(carNums, dataVec);
} 