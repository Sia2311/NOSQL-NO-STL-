#include "Collection.h"
#include "Document.h"
#include "Command.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: ./no_sql_dbms <database> <command> [json]\n";
        return 1;
    }

    string dbName = argv[1];
    string command = argv[2];
    string json = (argc >= 4) ? argv[3] : "";

    Collection col;
    string filename = dbName + ".json";
    col.LoadFromFIle(filename);

    if (command == "insert") {
        Document doc = parseJson(json);
        col.insert(doc);
        col.SaveToFile(filename);
        cout << "Document inserted successfully.\n";
    }
    else if (command == "find") {
        Array<Document> found = col.MainOperator(json);
        cout << "Found " << found.size << " document(s):\n";
        for (int i = 0; i < found.size; i++) {
            found.data[i].print();
        }
    }
    else if (command == "delete") {
        Document cond = parseJson(json);
        auto items = cond.items();
        if (items.size > 0) {
            col.removeByField(items.data[0].key, items.data[0].value);
            col.SaveToFile(filename);
            cout << "Documents deleted successfully.\n";
        }
    }
    else if (command == "print") {
        col.print();
    }
    else {
        cerr << "Unknown command.\n";
    }

    return 0;
}
