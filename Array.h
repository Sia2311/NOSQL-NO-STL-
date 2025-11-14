#pragma once
#include <iostream>
using namespace std;

template<typename T>
struct Array {
    T* data;       // указатель на данные
    int size;      // текущее количество элементов
    int capacity;  // вместимость массива

    Array() {
        data = nullptr;
        size = 0;
        capacity = 0;
    }

    ~Array() {
        if( data != nullptr){
            delete[] data;
            //от висячих указателейя
            data = nullptr;
        }
        size = 0;
        capacity = 0;
    }

    //копирующий конструктор
    Array (const Array& other){
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];

        for(int i = 0; i < size; i++){
            data[i] = other.data[i];
        }
    }

    //оператор присваивания
    Array& operator = (const Array& other){
        //самоприсвание кринж
        if(this == &other){
            return *this;
        }
        //удаляем старый буфер
        if(data != nullptr){
            delete[] data;
        }
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];

        for(int i = 0; i < size; i++){
            data[i] = other.data[i];
        }
        return *this;
    }

    //move конструктор
    //суть в том чтобы не копировать а забирать указатель
    Array(Array&& other){
        data = other.data;
        size = other.size;
        capacity = other.capacity;
        
        other.data = nullptr;   
        other.size = 0;
        other.capacity = 0;
    }

    //move operator
    Array& operator=(Array&& other){   
        if(this == &other){
            return *this;
        }
        //удаляем старый буфер
        if(data != nullptr){
            delete[] data;
        }
        //воруем указатель
        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;

        return *this;
    }
};


// создание массива
template<typename T>
Array<T> createArray() {
    Array<T> arr;
    int StartCapacity = 10;
    arr.size = 0;
    if (StartCapacity < 1) StartCapacity = 1;

    arr.data = new T[StartCapacity];
    if (arr.data == nullptr) {
        arr.capacity = 0;
        cout << "Ошибка выделения памяти" << endl;
    } else {
        arr.capacity = StartCapacity;
    }
    return arr;
}

// удаление массива
template<typename T>
void DelArray(Array<T>& arr) {
    if (arr.data != nullptr) {
        delete[] arr.data;
        arr.data = nullptr;
    }
}

// увеличение вместимости
template<typename T>
bool resizeArray(Array<T>& arr) {
    int NewCapacity = arr.capacity * 2;
    if (NewCapacity < 1) NewCapacity = 1;

    T* newData = new T[NewCapacity];
    if (newData == nullptr) return false;

    if (arr.data == nullptr || arr.size == 0) {
        arr.data = newData;
        arr.capacity = NewCapacity;
        return true;
    }
    
    // копируем элементы
    for (int i = 0; i < arr.size; i++) {
        newData[i] = arr.data[i];
    }

    delete[] arr.data;
    arr.data = newData;
    arr.capacity = NewCapacity;

    return true;
}

// добавить элемент в конец
template<typename T>
bool pushBack(Array<T>& arr, const T& value) {
    if (arr.size >= arr.capacity) {
        if (!resizeArray(arr)) return false;
    }
    arr.data[arr.size++] = value;
    return true;
}

// длина массива
template<typename T>
int getLength(const Array<T>& arr) {
    return arr.size;
}

// получить элемент по индексу
template<typename T>
T getValue(const Array<T>& arr, int index) {
    if (index < 0 || index >= arr.size) {
        cout << "Такого индекса не существует!" << endl;
        return T(); // пустой объект типа T
    }
    return arr.data[index];
}


// заменить значение по индексу
template<typename T>
bool replaceValue(Array<T>& arr, int index, const T& newValue) {
    if (index < 0 || index >= arr.size) {
        cout << "Ошибка: индекс " << index << " выходит за границы массива" << endl;
        return false;
    }
    arr.data[index] = newValue;
    return true;
}

// вставить элемент по индексу
template<typename T>
bool insertValue(Array<T>& arr, int index, const T& newValue) {
    if (index < 0 || index > arr.size) {
        cout << "Ошибка: индекс " << index << " вне диапазона [0, " << arr.size << "]" << endl;
        return false;
    }

    if (arr.size >= arr.capacity) {
        if (!resizeArray(arr)) return false;
    }

    for (int i = arr.size; i > index; i--) {
        arr.data[i] = arr.data[i - 1];
    }

    arr.data[index] = newValue;
    arr.size++;
    return true;
}

// удалить элемент по индексу
template<typename T>
bool removeValue(Array<T>& arr, int index) {
    if (index < 0 || index >= arr.size) {
        cout << "Ошибка: индекс " << index << " выходит за границы массива" << endl;
        return false;
    }

    for (int i = index; i < arr.size - 1; i++) {
        arr.data[i] = arr.data[i + 1];
    }

    arr.size--;
    return true;
}

// вывод массива
template<typename T>
void printArray(Array<T>& arr) {
    if (arr.size == 0) {
        cout << "Массив пустой";
        return;
    }

    cout << "[";
    for (int i = 0; i < arr.size; i++) {
        cout << arr.data[i];
        if (i < arr.size - 1) cout << " ";
    }
    cout << "]";
}

template<typename T>
T& getRef(Array<T>& arr, int index) {
    return arr.data[index];
}

// оператор сравнения 
template<typename T>
bool operator==(const Array<T>& a, const Array<T>& b) {
    if (a.size != b.size) return false;
    for (int i = 0; i < a.size; ++i) {
        if (!(a.data[i] == b.data[i])) return false;
    }
    return true;
}

template<typename T>
Array<T> copyArray(const Array<T>& src) {
    Array<T> dst = createArray<T>();
    // гарантируем вместимость
    while (dst.capacity < src.size) {
        resizeArray(dst);
    }
    // копируем элементы
    for (int i = 0; i < src.size; ++i) {
        pushBack(dst, src.data[i]);
    }
    return dst;
}


