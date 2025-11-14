#include "Collection.h"
#include "Document.h"
#include "Parser.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    // проверка аргументов
    if (argc < 3) {
        cerr << "usage: ./no_sql_dbms <database> <command> [json]\n";
        return 1;
    }

    // читаем аргументы
    string dbName = argv[1];
    string command = argv[2];
    string json = (argc >= 4) ? argv[3] : "";

    // загружаем коллекцию
    Collection col;
    string filename = dbName + ".json";
    col.setFilename(filename);
    col.LoadFromFIle();

    // команда insert
    if (command == "insert") {
        Document doc = parseJson(json);
        col.insert(doc);
        col.SaveToFile();
        cout << "document inserted\n";
    }

    // команда find
    else if (command == "find") {
        Array<Document> found = col.MainOperator(json);
        cout << "found " << found.size << " document(s):\n";

        for (int i = 0; i < found.size; i++) {
            found.data[i].print();
        }
    }

    // команда delete
    else if (command == "delete") {
        Document cond = parseJson(json);
        auto items = cond.items();

        if (items.size > 0) {
            string key = items.data[0].key;
            string val = items.data[0].value;

            col.removeByField(key, val);
            col.SaveToFile();
            cout << "documents deleted\n";
        }
    }

    // команда print
    else if (command == "print") {
        col.print();
    }

    // неизвестная команда
    else {
        cerr << "unknown command\n";
    }

    return 0;
}
