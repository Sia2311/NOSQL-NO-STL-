#pragma once
#include <iostream>
using namespace std;

template<typename T>
struct SNode {
    T data; // значение узла
    SNode* next; // указатель на след узел
};

template<typename T>
struct ForwardList {
    SNode<T>* head; //голова списка
    int size; // счётчик элементов

    ForwardList() {
        head = nullptr;
        size = 0;
    }

    ~ForwardList(){
        SNode<T>* current = head;
        while(current != nullptr){
            SNode<T>* next = current -> next;
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;
    }
    //копирующий конструктор
    ForwardList(const ForwardList& other){
        head = nullptr;
        size = 0;
        if(other.head == nullptr){
            return;
        }
        //указатель на узел другого списка
        SNode<T>* currentOther = other.head;
        head = new SNode<T> { currentOther -> data, nullptr};
        size = 1;
        //последний узел в копии
        SNode<T>* currentThis = head;
        currentOther = currentOther -> next;
        while(currentOther != nullptr){
            //новый узел с копией данных
            currentThis -> next = new SNode<T> {currentOther -> data, nullptr};
            //бежим в двух списках
            currentThis = currentThis -> next;
            currentOther = currentOther -> next;
            size++;
        }
    }
    //копирующие присваивание
    ForwardList& operator = (const ForwardList& other){
        
        if(this == &other){
            return *this;
        }
        //удаляем текущий список
        SNode<T>* current = head;
        while(current != nullptr){
            SNode<T>* next = current -> next;
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;
        //если другой список пуст
        if(other.head == nullptr){
            return *this;
        }
        SNode<T>* currentOther = other.head;

        head = new SNode<T>{ currentOther->data, nullptr };
        size = 1;
    
        SNode<T>* currentThis = head;
        currentOther = currentOther->next;
    
        while (currentOther != nullptr) {
            currentThis->next = new SNode<T>{ currentOther->data, nullptr };
    
            currentThis = currentThis->next;
            currentOther = currentOther->next;
    
            size++;
        }
    
        return *this;
    }
    //move конструктор 
    ForwardList(ForwardList&& other){
        //воруем
        head = other.head;
        size = other.size;
        //убиваем источник
        other.head = nullptr;
        other.size = 0;
    }
    //оператор присваивания
    ForwardList& operator = (ForwardList&& other){
        if(this == &other){
            return *this;
        }
        //удаляем старый список
        SNode<T>* current = head;
        while(current != nullptr){
            SNode<T>* next = current -> next;
            delete current;
            current = next;
        }
        //воруем 
        head = other.head;
        size = other.size;

        other.head = nullptr;
        other.size = 0;

        return *this;
    }
};

// пустой список
template<typename T>
ForwardList<T> CreateFL(){
    ForwardList<T> list;
    list.head = NULL; // голова никуда не смотрит
    list.size = 0;
    return list;
}

// del список
template<typename T>
void DelFL (ForwardList<T>& list){
    SNode<T>* current = list.head; // начало с головы
    while (current != NULL){
        SNode<T>* next = current -> next; // след узел
        delete current;
        current = next;
    }
    list.head = NULL;
    list.size = 0;
}
// vivod
template<typename T>
void PrintFL (const ForwardList<T>& list){
    if(list.head == NULL){
        cout << "Список пуст" << endl;
        return;
    }
    SNode<T>* current = list.head;
    while(current != nullptr){
        cout << current -> data; // выводим текущий узел
        if (current -> next != NULL){
            cout << " ->";
        }
        current = current -> next; // следующий узел
    }
    cout << endl;
}

template<typename T>
int GetSize(const ForwardList<T>& list){
    return list.size;
}

//добавление в начало
template<typename T>
void AddHead(ForwardList<T>& list, T value){
    SNode<T>* newSNode = new SNode<T> {value, list.head}; // новый узел указывет на старую голову
    list.head = newSNode;
    list.size++;
}

//добавление в конец
template<typename T>
void AddEnd(ForwardList<T>& list, T value){
    SNode<T>* newSNode = new SNode<T> {value, NULL}; // новый узел в конце
    // Если список пуст то новый узел теперь голова
    if (list.head == NULL){
        list.head = newSNode;
    }
    else{
        // ищем последний узел
        SNode<T>* current = list.head;
        while (current->next != NULL){
            current = current -> next;
        }
        
        current -> next = newSNode;
    }
    list.size++;
}

//поиск по значению
template<typename T>
SNode<T>* FindValue(const ForwardList<T>& list, T value){
    SNode<T>* current = list.head;
    while (current != NULL){
        //если нашли
        if (current -> data == value){
            return current;
        }
        current = current -> next;
    }
    return NULL;
}

// добавление после указанного элемента
template<typename T>
bool AddAfter(ForwardList<T>& list, T afterValue, T newValue){
    // поиск узла после которого нужна вставить элемент
    SNode<T>* target = FindValue(list, afterValue);
    if (target == NULL){
        cout << "Элемент " << afterValue << " не найден." << endl;
        return false;
    }
//создаем новый узел
// next нового узла будет указывать на то что было после target
SNode<T>* newSNode = new SNode<T> {newValue, target -> next};
target -> next = newSNode; // таргет указывает на новый узел
list.size++;
return true;
}
// Добавление  перед указанным элементом  
template<typename T>
bool AddBefore(ForwardList<T>& list, T beforeValue, T newValue) {
    if (list.head == nullptr) return false;  // пустой список
    
    // Если вставляем перед головой
    if (list.head->data == beforeValue) {
        AddHead(list, newValue);  // делаем новую голову
        return true;
    }
    
    // Ищем узел который стоит перед тем который ищем
    SNode<T>* current = list.head;
    while (current->next != nullptr && current->next->data != beforeValue) {
        current = current->next;
    }
    
    // Проверяем, нашли ли нужный элемент
    if (current->next == nullptr) {
        cout << "Элемент '" << beforeValue << "' не найден!" << endl;
        return false;
    }
    
    // Вставляем новый узел между current и current->некст
    SNode<T>* newSNode = new SNode<T>{newValue, current->next};
    current->next = newSNode;
    list.size++;
    return true;
}

// Удаление первого элемента списка
template<typename T>
void DeleteHead(ForwardList<T>& list) {
    if (list.head == nullptr) return;  // нечего удалять
    
    SNode<T>* temp = list.head;      // запоминаем старую голову
    list.head = list.head->next; // новая голова - следующий элемент
    delete temp;                
    list.size--;                 
}

// Удаление последнего элемента списка
template<typename T>
void DeleteTail(ForwardList<T>& list) {
    if (list.head == nullptr) return;  // список пуст
    
    if (list.head->next == nullptr) {
        // Если в списке только один элемент
        delete list.head;
        list.head = nullptr;
    } else {
        // Ищем предпоследний элемент
        SNode<T>* current = list.head;
        while (current->next->next != nullptr) {
            current = current->next;
        }
        // Удаляем последний элемент
        delete current->next;
        current->next = nullptr;  // предпоследний теперь последний
    }
    list.size--;
}

// Удаление элемента по значению
template<typename T>
bool DeleteByValue(ForwardList<T>& list, T value) {
    if (list.head == nullptr) return false;  
    
    // если голова 
    if (list.head->data == value) {
        DeleteHead(list);
        return true;
    }
    
    //ищем узел который стоит перед тем который удаляем
    SNode<T>* current = list.head;
    while (current->next != nullptr && current->next->data != value) {
        current = current->next;
    }
    
    //проверяем нашли ли элемент для удаления
    if (current->next == nullptr) {
        cout << "Элемент '" << value << "' не найден!" << endl;
        return false;
    }
    
    //удаляем узел и перевязываем 
    SNode<T>* temp = current->next;       
    current->next = current->next->next; // перепрыгиваем через удаляемый
    delete temp;                      
    list.size--;
    return true;
}

template<typename T>
bool DeleteAfter(ForwardList<T>& list, const T& afterValue) {
    if (list.head == nullptr) return false;  

    // ищем после какого удалить
    SNode<T>* target = FindValue(list, afterValue);
    if (target == nullptr) {
        cout << "Элемент '" << afterValue << "' не найден!" << endl;
        return false;
    }
    if (target->next == nullptr) {
        cout << "После '" << afterValue << "' элемента нет!" << endl;
        return false;
    }

    // удаление
    SNode<T>* toDelete = target->next;
    target->next = toDelete->next;
    delete toDelete;
    list.size--;
    return true;
}

//удаление до
template<typename T>
bool DeleteBefore(ForwardList<T>& list, const T& beforeValue) {
    if (list.head == nullptr) return false;              
    if (list.head->data == beforeValue) {
        // перед головой нечего удалять
        cout << "Перед '" << beforeValue << "нуллптр" << endl;
        return false;
    }

    SNode<T>* prevPrev = nullptr;
    SNode<T>* prev = list.head;
    SNode<T>* cur = list.head->next;

    // ищем первый узел с данными beforeValue (cur указывает на него)
    while (cur != nullptr && cur->data != beforeValue) {
        prevPrev = prev;
        prev = cur;
        cur = cur->next;
    }

    if (cur == nullptr) {
        cout << "Элемент '" << beforeValue << "' не найден!" << endl;
        return false;
    }

    // удаляем узел prev
    if (prevPrev == nullptr) {
        // удаляемая нода была головой
        list.head = cur;
    } else {
        prevPrev->next = cur;
    }
    delete prev;
    list.size--;
    return true;
}

template <typename T>
void ClearFL(ForwardList<T>& list) {
    SNode<T>* current = list.head;
    while (current != nullptr) {
        SNode<T>* next = current->next;
        delete current;
        current = next;
    }
    list.head = nullptr;
    list.size = 0;
}
