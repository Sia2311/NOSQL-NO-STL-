#include <iostream>
#include "Optoinal.h"
#include "Document.h"

using namespace std;

// добавить поле
void Document::set(const string& key, const string& value) {
    fields.put(key, value);
}

// получить значение
Optional<string> Document::get(const string& key) const {
    return fields.get(key);
}

// наличие ключа
bool Document::contains(const string& key) const {
    auto value = fields.get(key);
    return value.HAS_value();
}

// вывод документа
void Document::print() const {
    cout << "{ ";

    auto all = fields.items();
    //цикл по всем парам
    for (int i = 0; i < all.size; ++i) {

        cout << "\"" << all.data[i].key
             << "\": \"" << all.data[i].value << "\"";

        if (i + 1 < all.size)
            cout << ", ";
    }

    cout << " }" << endl;
}

// возврат всех пар
Array<HashMap<string, string>::Pair> Document::items() const {
    return fields.items();
}
