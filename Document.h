#pragma once
#include <string>
#include <optional>
#include <iostream>
#include "HashMap.h"
#include "Array.h"

class Document {
private:
    // хранит все поля документа
    HashMap<std::string, std::string> fields;

public:

    void set(const std::string& key, const std::string& value);

    std::optional<std::string> get(const std::string& key) const;

    bool contains(const std::string& key) const;

    void print() const;

    Array<HashMap<std::string,std::string>::Pair> items() const;  
};
