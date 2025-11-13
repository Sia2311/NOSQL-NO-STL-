#pragma once
#include <iostream>
#include <string>
#include <optional>
#include "Array.h"
#include "SinglyList.h"

using namespace std;

template <typename K, typename V>
class HashMap{
public:

struct Pair{
    K key;
    V value;
    
    bool operator==(const Pair& other) const {
        return key == other.key && value == other.value;
    }
};

private:

    //хранилище массив цепочек
    Array<ForwardList<Pair>> table;
    
    int capacity; // колво ячеек
    int count; //колво элементов

    int hash(const K& key) const {
        int h = 0;
        //если ключ строка
        for(char c : key){
            h = h * 31 + c;
        }
        return abs(h) % capacity;
    }

    //РЕХЕШИРОВАНИЕ
    void rehash() {
        int oldCapacity = capacity;
        Array<ForwardList<Pair>> oldTable = table;

        capacity *= 2; 
        //создаем новую табл
        table = createArray<ForwardList<Pair>>();
        while (table.capacity < capacity) {
            resizeArray(table);
        }
        //пустые цепочки
        for (int i = 0; i < capacity; ++i) {
            ForwardList<Pair> fl = CreateFL<Pair>();
            pushBack(table, fl);
        }

        count = 0;
        //переносим пары из старой табл
        for (int i = 0; i < oldCapacity; ++i) {
            ForwardList<Pair>& chain = oldTable.data[i];
            SNode<Pair>* current = chain.head;
            while (current != nullptr) {
                put(current->data.key, current->data.value);
                current = current->next;
            }
        }
     }

public:

//конструктор
    HashMap(int initCap = 13){
        capacity = initCap;

        table = createArray<ForwardList<Pair>>();
        while (table.capacity < capacity) {
            resizeArray(table);
        }
        //каждая ячейка = пустая цепочка
        for (int i = 0; i < capacity; ++i) {
            ForwardList<Pair> fl = CreateFL<Pair>();
            pushBack(table, fl);
        }
        count = 0;
    }
    //добавление
    void put(const K& key, const V& value){

        int index = hash(key);
        ForwardList<Pair>& chain = table.data[index];
        //существует ли уже такой ключ
        SNode<Pair>* current = chain.head;
        while (current != nullptr) {
            if (current->data.key == key) {
                current->data.value = value;
                return;
            }
            current = current->next;
        }
        //новый ключ - добавляем
        Pair p{key, value};
        AddEnd(chain, p);
        count++;

        if ((float)count / capacity > 0.75f) {
            rehash();
        }
    }

    void remove(const K& key){
        int index = hash(key);
        ForwardList<Pair>& chain = table.data[index];

        if (chain.head == nullptr) {
            return;
        }
        //удаление головы списка
        if (chain.head->data.key == key) {
            DeleteHead(chain);
            count--;
            return;
        }
        //поиск узла перед удаляемым
        SNode<Pair>* prev = chain.head;
        SNode<Pair>* cur = chain.head->next;

        while (cur != nullptr && cur->data.key != key) {
            prev = cur;
            cur = cur->next;
        }
    
        if (cur != nullptr) {
            prev->next = cur->next;
            delete cur;
            chain.size--;
            count--;
        }
    }
    

    //значение по ключу
    optional<V> get(const K& key) const {
        int index = hash(key);
        const ForwardList<Pair>& chain = table.data[index];

        SNode<Pair>* current = chain.head;
        while (current != nullptr) {
            if (current->data.key == key) {
                return current->data.value;
            }
            current = current->next;
        }
        return nullopt; 
    }
    //вернуть массив всех пар
    Array<Pair> items() const {

        Array<Pair> result = createArray<Pair>();

        for (int i = 0; i < capacity; ++i) {
            const ForwardList<Pair>& chain = table.data[i]; //цепочка
            SNode<Pair>* current = chain.head; //идём с головы

            while (current != nullptr) {
                if (result.size >= result.capacity) {
                    resizeArray(result);
                } 
                pushBack(result, current->data);
                current = current->next;
            }
        }

        return result;
    }

    int size() const {
        return count;
    }
};
