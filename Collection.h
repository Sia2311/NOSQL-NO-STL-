#pragma once
#include <string>
#include "HashMap.h"
#include "Document.h"
#include "Array.h"

class Collection {
private:
    HashMap<string, Document> documents;  // хранение документов
    int idCounter = 0; 

    string generateId();
    string filename;

public:

    string insert(const Document& doc);

    Array<Document> findByField(const string&key, const string& value) const;

    void removeByField(const string& key, const string& value);

    void print() const;

    void SaveToFile() const; 

    void LoadFromFIle();

    Array<Document> MainOperator(const string& input);

    void setFilename(const string& name);

};
