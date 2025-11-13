#include <iostream>
#include "Document.h"

using namespace std;

//добавить поле
void Document::set(const string& key, const string& value){
    fields.put(key, value);
}

//получить значение
optional<string> Document::get(const string& key) const{
    return fields.get(key);
}

//наличие ключа
bool Document::contains(const string& key) const{
    auto value = fields.get(key);
    return value.has_value();
}

//вывод документа
void Document::print() const {
    cout << "{ ";

    // NEW: теперь items() возвращает Array<Pair>
    auto all = fields.items(); 

    // NEW: цикл по Array
    for (int i = 0; i < all.size; ++i) {

        // NEW: доступ к key и value через all.data[i].key/value
        cout << "\"" << all.data[i].key 
             << "\": \"" << all.data[i].value << "\"";

        if (i + 1 < all.size)
            cout << ", ";
    }
    cout << " }" << endl;
}

//возврат всех пар
Array<HashMap<string,string>::Pair> Document::items() const {
    // NEW: просто передаём результат HashMap::items()
    return fields.items();
}
