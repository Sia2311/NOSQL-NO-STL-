#include "Collection.h"
#include "Compare.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

string Collection::generateId() {
    ++idCounter;
    return "doc_" + to_string(idCounter);
}

//+ документ в коллекцию
string Collection::insert(const Document& doc){
    string id = generateId();
    documents.put(id, doc);
    return id;
}

//поиск документов с нужным полем и значением
Array<Document> Collection::findByField(const string& key, const string& value) const {
    
    Array<Document> result;
    result = createArray<Document>();

    // перебираем все документы
    auto all = documents.items();

    for (int i = 0; i < all.size; i++) {

        const Document& doc = all.data[i].value;

        // ищем поле
        auto field = doc.get(key);
        if (!field.has_value()) continue;

        string fieldVal = field.value();
        string cond = value;

        // убираем кавычки из значений 
        if (!cond.empty() && cond.front() == '"' && cond.back() == '"') {
            cond = cond.substr(1, cond.size() - 2);
        }

        // сравнение 
        if (ComparsionOperator(fieldVal, cond)) {

            if (result.size >= result.capacity)
                resizeArray(result);

            pushBack(result, doc);
        }
    }
    return result;
}


//удаление документов с нужным полем и значением
void Collection::removeByField(const string& key, const string& value) {

    Array<string> toDelete;
    toDelete = createArray<string>();

    auto all = documents.items();

    for (int i = 0; i < all.size; i++) {

        const string& id = all.data[i].key;
        const Document& doc = all.data[i].value;

        auto field = doc.get(key);
        if (!field.has_value()) continue;

        string fieldVal = field.value();
        string cond = value;

        // убираем кавычки из значений (если есть)
        if (!cond.empty() && cond.front() == '"' && cond.back() == '"') {
            cond = cond.substr(1, cond.size() - 2);
        }

        // используем общий оператор сравнения 
        if (ComparsionOperator(fieldVal, cond)) {

            if (toDelete.size >= toDelete.capacity)
                resizeArray(toDelete);

            pushBack(toDelete, id);
        }
    }

    //  удаляем все подходящие документы
    for (int i = 0; i < toDelete.size; i++) {
        documents.remove(toDelete.data[i]);
    }

    // сохраняем изменения в файл
    SaveToFile("my_database.json");
    cout << "Documents deleted successfully." << endl;
}


//вывод коллекции документов
void Collection::print() const {
    cout << "Collection (" << documents.size() << " documents):" << endl; //колво

    auto all = documents.items();

    for (int i = 0; i < all.size; i++) {
        cout << all.data[i].key << ": ";
        all.data[i].value.print();
    }
}


// "doc_1": { "name" : "Alice", "age" : "25" }
void Collection::SaveToFile(const string& filename) const {
    ofstream file(filename);
    if(!file.is_open()){
        cerr << "Ошибка откратия файла для записи" << endl;
        return;
    }
    file << "{" << endl; 
    auto all = documents.items(); 
    //бежим по документам
    for(int i = 0; i < all.size; ++i){
        const string& id = all.data[i].key; //извлекаем ид
        const Document& doc = all.data[i].value; //сам документ

        file << " \"" << id << "\": ";

        auto fields = doc.items(); //все поля документов в виде пар
        file << "{ ";

        //бежим по полям документa и пишем имя и значение
        for(int j = 0; j < fields.size; ++j){
            file << "\"" << fields.data[j].key << "\" : " << "\"" << fields.data[j].value << "\"";
            //если есть ещё поля
            if(j + 1 < fields.size){
                file << ", ";
            }  
        }
        file << " }";
        // если есть ещё документ
        if(i + 1 < all.size) {
            file << ",";
        }
        file << endl;
    }
    file << "}" << endl;
    file.close();

    cout << "Коллекция сохранена в файл " << filename << endl;
}

void Collection::LoadFromFIle(const string& filename){
    ifstream file(filename);
    if(!file.is_open()){
        cerr << "Файл не найден" << endl;
        return;
    }
    documents = HashMap <string, Document>();
    idCounter = 0;

    string line;
    //читаем файл построчно
    while(getline(file, line)) {
        if(line.find("doc_") != string::npos){ 
            int idStart = line.find("\"") +1; // начало ид
            int idEnd = line.find("\"", idStart); // конец ид
            if(idStart == string::npos || idEnd == string::npos){
                continue;
            }
            string id = line.substr(idStart, idEnd - idStart);

            Document doc;

            int pos = line.find("{", idEnd); //позиция для поиска кей и валуе
            if(pos == string::npos){
                continue;
            }
            pos++;

            while(true){
                //ищем первую кавычку ключа
                int keyQuote1 = line.find("\"", pos);
                if(keyQuote1 == string::npos){
                    break;
                }
                //вторую кавычку - конец ключа
                int keyQuote2 = line.find("\"", keyQuote1 + 1);
                if(keyQuote2 == string::npos){
                    break;
                }
                //вырезаем текст из кавычек
                string key = line.substr(keyQuote1 + 1, keyQuote2 - keyQuote1 - 1);
                //ищем ":"
                int colon = line.find(":", keyQuote2);
                if(colon == string::npos){
                    break;
                }
                //первая ковычка после ":"
                int valueQuote1 = line.find("\"", colon + 1);
                if(valueQuote1 == string::npos){
                    break;
                }
                //вторая кавычка значения
                int valueQuote2 = line.find("\"", valueQuote1 + 1);
                if(valueQuote2 == string::npos){
                    break;
                }
                
                string value = line.substr(valueQuote1 + 1, valueQuote2 - valueQuote1 - 1);
                doc.set(key,value);
                pos = valueQuote2 + 1; // продолжаем поиск

                //проверка: не дошли ли мы до конца объектая
                int bracePos = line.find("}", pos);
                int nextQuote = line.find("\"", pos);

                if(bracePos != string::npos && (nextQuote == string::npos || bracePos < nextQuote)){
                    break;
                }
            }
            documents.put(id, doc);
            ++idCounter;
        }
    }
    file.close();
    cout << "Коллекция загружена из файла: " << filename << endl;
}

Array<Document> Collection::MainOperator(const string& input){

    Array<Document> result;

    cout << "DEBUG(INPUT): " << input << endl;

    //удаляем пробелы
    string clearInput;
    for(char ch : input){
        if(ch != ' ' && ch != '\t' && ch != '\n'){
            clearInput += ch;
        }
    }
    cout << "DEBUG(CLEARINPUT): " << clearInput << endl;
    //определим тип
    bool isAnd = clearInput.find("\"$and\"") != string::npos;
    bool isOr = clearInput.find("\"$or\"") != string::npos;
    cout << "DEBUG(ISAND/ISOR): " << isAnd << " " << isOr << endl;

    if (isAnd || isOr) {
        int bracketStart = clearInput.find('[');
        int bracketEnd = clearInput.find(']');
        if (bracketStart != string::npos && bracketEnd != string::npos && bracketEnd > bracketStart) {
            string innerPart = clearInput.substr(bracketStart + 1, bracketEnd - bracketStart - 1);
            // пишем внутреннию часть как условию
            clearInput = "{" + innerPart + "}";
            cout << "DEBUG(Flattened AND/OR): " << clearInput << endl;
        }
    }

    //в случаи если нет ни and ни or, можно считать and неявным
    if(!isAnd && !isOr) {
        isAnd = true;
    }
    //вытаскиваем условим из скобок
    int start = clearInput.find("{"); // УЖАСНОЕ МЕСТО (ЛУЧШЕ НЕ ТРОГАТЬ)
    cout << "DEBUG(START): " << start << endl;
    if(start == string::npos){
        return result;
    }
    string inner = clearInput.substr(start);
    cout << "DEBUG(INNER): " << inner << endl;
    //разбиваем услвоия
    Array<string> conditions; // сюда ложатся условия
    int position = 0;
    while(true){
        int open = clearInput.find("{", position);
        int close = clearInput.find ("}", open);
        if(open == string::npos || close == string::npos){
            break;
        }
        //записываем условие
        pushBack(conditions, clearInput.substr(open, close - open + 1));
        //сдвигаем за '}'
        position = close + 1;
    }
    //ищем документы для каждого условия
    Array< Array<Document> > allResults;

    //идём по условиям
    for(int i = 0; i < conditions.size; ++i){
        string cond = conditions.data[i];
        //ищем первую ковычку и закрывающую
        int keyStart = cond.find("\"") + 1;
        int keyEnd = cond.find("\"", keyStart);
        if(keyStart == string::npos || keyEnd == string::npos){
            continue;
        }
        //вырезаем ключ и ищем двоеточие
        string key = cond.substr(keyStart, keyEnd - keyStart);
        cout << "DEBUG: " << key << endl;
        int colon = cond.find(":", keyEnd);

        if(colon == string::npos){
            continue;
        }
        string value = cond.substr(colon + 1); //всё после ":" - условие
        cout << "DEBUG: " << value << endl;
        // убираем лишние символы только если значение не объект {...}
        if (value.find('{') == string::npos) {
            while(!value.empty() && (value.back() == '}' || value.back() == ',' || value.back() == ' ')) {
                value.pop_back();
            }
        }
        // убираем пробелы и табы в начале
        while(!value.empty() && (value.front() == ' ' || value.front() == '\t')) {
            value.erase(0, 1);
        }
        cout << "DEBUG" << " VALUE: " << value << endl;

        Array<Document> subRes = findByField(key, value);
        pushBack(allResults, subRes);
    }

    // объединяем результат
    if(isAnd){
        //and
        if(allResults.size > 0){
            result = allResults.data[0]; //первый набор результатов

            for(int i = 1; i < allResults.size; ++i){ // остальные результаты
                Array<Document> temp;
                for(int a = 0; a < result.size; ++a){ //идёи по результат прошлого условия
                    for(int b = 0; b < allResults.data[i].size; ++b){ // по результата нового условия
                        if(result.data[a].items() == allResults.data[i].data[b].items()){
                            pushBack(temp, result.data[a]);
                        }
                    }
                }
                result = temp;
            }
        }
    }
    //$or
    else if(isOr){
        for (int i = 0; i < allResults.size; ++i) { // каждое условие
            for (int j = 0; j < allResults.data[i].size; ++j) { // каждый документ
                bool exists = false;
                for (int k = 0; k < result.size; ++k) { // не добавлен ли уже документ
                    if (result.data[k].items() == allResults.data[i].data[j].items()) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    pushBack(result, allResults.data[i].data[j]);
                }
            }
        }
    }
    return result;
}
