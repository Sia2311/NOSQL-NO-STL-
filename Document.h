#pragma once
#include <string>
#include "Optoinal.h"
#include <iostream>
#include "HashMap.h"
#include "Array.h"

using namespace std;

class Document {
private:
    // хранит все поля документа
    HashMap<string, string> fields;

public:

    void set(const string& key, const string& value);

    Optional<string> get(const string& key) const;

    bool contains(const string& key) const;

    void print() const;

    Array<HashMap<string,string>::Pair> items() const;  
};
