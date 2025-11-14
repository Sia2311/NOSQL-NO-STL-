#include "Collection.h"
#include "Document.h"
#include "Parser.h"
#include <iostream>
#include <string>

using namespace std;

//парсер
Document parseJson(const string& json){
    Document doc;
    int pos = 0;

  while ((pos = json.find("\"", pos)) != string::npos) {
    //ищем ключ
    int keyStart = pos + 1;
    int keyEnd = json.find("\"", keyStart);
    if (keyEnd == string::npos) {
        break;
    }
    string key = json.substr(keyStart, keyEnd - keyStart);

    // Ищем значение после двоеточия которе отделяет кей от валуе
    int colonPos = json.find(":", keyEnd);
    if (colonPos == string::npos) {
        break;
    }

    // Пропускаем табуляцию
    int valueStart = json.find_first_not_of(" \t", colonPos + 1);
    if (valueStart == string::npos) break;

    //вратли дальше правильно работает код
    // но это попытка понять что за тип значения

    //строка в кавычках
    if(json[valueStart] == '\"'){
        int valueRealStart = valueStart + 1;
        int valueEnd = json.find("\"", valueRealStart);
        if(valueEnd == string::npos) break; // защита от обрыва строки
        string value = json.substr(valueRealStart, valueEnd - valueRealStart);

        doc.set(key, value);
        pos = valueEnd + 1;
    }

    //вложенный тип ( типо : {"age":{"$gt":25}})
    else if(json[valueStart] == '{'){
        //ищем конец
        int braceEnd = json.find("}", valueStart); 
        int nextOpen = json.find("{", valueStart + 1);
        //если есть вложенные скобки, ищем дальше
        if(nextOpen != string::npos && nextOpen < braceEnd){
            braceEnd = json.find("}", braceEnd + 1);
        }
        if(braceEnd == string::npos) break;
        string object = json.substr(valueStart, braceEnd - valueStart + 1); //берём внутренность скобок
        doc.set(key, object);
        pos = braceEnd + 1;
    }

    //число (age:25)
    else if (isdigit(json[valueStart]) || json[valueStart] == '-') {
        int valueEnd = json.find_first_of(",} ", valueStart); // конец числа
        if(valueEnd == string::npos) valueEnd = json.size();
        string value = json.substr(valueStart, valueEnd - valueStart); // вырезаем число
        doc.set(key, value);          
        pos = valueEnd + 1;           
    }

    else{
        pos++;
    }
}
return doc;
}