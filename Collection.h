#pragma once
#include <string>
#include "HashMap.h"
#include "Document.h"
#include "Array.h"

class Collection {
private:
    HashMap<string, Document> documents;  // хранение документов
    int idCounter = 0; // для генерации уникальных _id

    // генерация нового _id
    string generateId();

public:
    // вставка документа, возвращает сгенерированный id
    string insert(const Document& doc);

    // поиск всех документов, где key=value
    Array<Document> findByField(const string&key, const string& value) const;

    // удаление всех документов, где key=value
    void removeByField(const string& key, const string& value);

    // вывод всей коллекции
    void print() const;

    void SaveToFile(const string& filename) const; 

    void LoadFromFIle(const string& filename);

    Array<Document> MainOperator(const string& input);

};
