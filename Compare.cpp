#include <iostream>
#include "Compare.h"
#include <string>
using namespace std;

//операторы сравнения
bool ComparsionOperator (const string& fieldValue, const string& condition){
    if(condition.empty()) {
        return false;
    }
    cout << "[DEBUG] fieldValue=" << fieldValue << " condition=" << condition << endl;

    // если используется несколько операторов
    if (
        (condition.find("$gt") != string::npos || condition.find("$gte") != string::npos) &&
        (condition.find("$lt") != string::npos || condition.find("$lte") != string::npos)
    ) {
        double minValue = 0;
        double maxValue = 0;
        double  fieldNumber = 0;
        //флаги равенства границ
        bool includeMin = false;
        bool includeMax = false;

        try{
            //нижняя гнраница
            if(condition.find("$gte") != string::npos){
                includeMin = true;
                int position = condition.find("$gte");
                int q1 = condition.find("\"", position + 5); // первая ковычка после gte
                int q2 = condition.find("\"", q1 + 1);
                //берём нижнию границу
                minValue = stod(condition.substr(q1 + 1, q2 - q1 - 1));
            }
            else if(condition.find("$gt") != string::npos){
                int position = condition.find("$gt");
                int q1 = condition.find("\"", position + 4);
                int q2 = condition.find("\"", q1 + 1);
                minValue = stod(condition.substr(q1 + 1, q2 - q1 - 1));
            }
            //верхняя граница
            if(condition.find("$lte") != string::npos){
                includeMax = true;
                int position = condition.find("$lte");
                int q1 = condition.find("\"", position + 5);
                int q2 = condition.find("\"", q1 + 1);
                maxValue = stod(condition.substr(q1 + 1, q2 - q1 - 1));
            }
            else if (condition.find("$lt") != string::npos){
                int position = condition.find("$lt");
                int q1 = condition.find ("\"", position + 4);
                int q2 = condition.find ("\"", q1 + 1);
                maxValue = stod(condition.substr(q1 + 1, q2 - q1 - 1));
            }
            //значения поля
            fieldNumber = stod(fieldValue);
            //диапазон при вкл и выкл нижней границей ( и верхней)
            bool lowerOk = includeMin ? (fieldNumber >= minValue) : (fieldNumber > minValue);
            bool upperOK = includeMax ? (fieldNumber <= maxValue) : (fieldNumber < maxValue);

            return (lowerOk && upperOK);
        } catch (...){
            return false;
        }
    }

    //если строка не начинается с { то будет типо ("London")
    if(condition.front() != '{'){
        // если оба значения можно преобразовать в число — сравниваем как числа
        double fVal = 0, cVal = 0;
        bool isNum = true;
        try {
            fVal = stod(fieldValue);
            cVal = stod(condition);
        } catch(...) {
            isNum = false;
        }
        if (isNum) {
            return fVal == cVal; // равенство как чисел
        }
        // иначе сравниваем как строки
        return fieldValue == condition;
    }

    //вытаскиеваем оператора из скобок
    int operatorStart = condition.find("\"") + 1;
    int operatorEnd = condition.find("\"", operatorStart);
    string operat = condition.substr(operatorStart, operatorEnd - operatorStart);

    //извлекаем значение после двоеточия
    int valueStart = condition.find_first_not_of(" \t:{\"", operatorEnd + 1); //пропускаем пробелы и символы
    int valueEnd = condition.find_first_of("\"}]", valueStart);  
    string value = condition.substr(valueStart, valueEnd - valueStart);

    //пробуем интерпретировать значения как числа (для $gt, $lt, $gte, $lte)
    double fieldNum = 0;
    double conditionNum = 0;
    bool diget = true;
    try{
        fieldNum = stod(fieldValue); // значение поля
        conditionNum = stod(value); // значение условия
    } catch(...){
        diget = false;
    }

    // $eq сравнивает
    if(operat == "$eq"){
        if(diget) return fieldNum == conditionNum; // добавлено: числовое сравнение
        return fieldValue == value;
    }

    // $gt - больше чем
    else if (operat == "$gt" && diget){
        return fieldNum > conditionNum;
    }

    // $lt меньше чем
    else if (operat == "$lt" && diget){
        return fieldNum < conditionNum;    
    }

    //$gte больше или равно
    else if (operat == "$gte" && diget){
        return fieldNum >= conditionNum;
    }

    //$lte меньше или равно
    else if(operat == "$lte" && diget){
        return fieldNum <= conditionNum;
    }

   //$like - строковый поиск с поддержкой wildcards ( % - любая строка, _ - один символ)
    else if (operat == "$like") {
    string val = value;
    string str = fieldValue;

    // если чисто процент
    if (val == "%") return true;

    // если прцоент в начале и в конце
    if (!val.empty() && val.front() == '%' && val.back() == '%') {
        string pattern = val.substr(1, val.size() - 2);
        return str.find(pattern) != string::npos;
    }
    if(!value.empty() && value.front() == '%'){
        string pattern = value.substr(1); 
        if(fieldValue.size() < pattern.size()) {
            return false;
        }
        int offset = fieldValue.size() - pattern.size();

        //  проверка c _
        for(int i = 0; i < pattern.size(); ++i){
            if(pattern[i] == '_') continue;          
            if(fieldValue[offset + i] != pattern[i]){ 
                return false;
            }
        }
        return true;
    }
    //кончает процентом или с _ в начале
    if(!value.empty() && value.back() == '%'){
        string pattern = value.substr(0, value.size() - 1); 
        if(fieldValue.size() < pattern.size()) return false;
        // проверка префикса с _
        for(int i = 0; i < pattern.size(); ++i){
            if(pattern[i] == '_') continue;         
            if(fieldValue[i] != pattern[i]) return false;
        }
        return true;
    }
    

    // проверка '_'
    bool match = true;
    if (val.find('_') != string::npos) {
        if (str.size() < val.size()){ 
            return false;
        }
        for (size_t i = 0; i < val.size() && i < str.size(); ++i) {

            if (val[i] == '_'){ 
                continue; 
            }
            if (val[i] != str[i]) { 
                match = false; break; 
            }
        }
        if (match){ 
            return true;
        }
    }

    // если без спецсимволов ищем подстроку
    return str.find(val) != string::npos;
}

    // $in - принадлежит множество значений
    // Идея пройти по всем значениям между кавычками и сравнить с fieldValue
    else if(operat == "$in"){
        int position = condition.find("["); 
        while(position != string::npos){
            int q1 = condition.find("\"", position); //первая кaвычка
            if(q1 == string::npos){
                break;
            }
            int q2 = condition.find("\"" , q1 + 1); //вторая кaвычка
            if(q2 == string::npos) {
                break;
            }
            string valIn = condition.substr(q1 + 1, q2 - q1 - 1); 
            if(valIn == fieldValue){
                return true;
            }
            position = q2 + 1;
        }
        return false;
    }

    return false;
}