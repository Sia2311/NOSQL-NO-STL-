# NoSQL DBMS (C++) — Custom Database Engine

Учебная реализация NoSQL базы данных на языке **C++**, выполненная без STL-контейнеров.
Все основные структуры данных и логика работы разработаны вручную:

- динамический массив `Array<T>`
- односвязный список `ForwardList<T>`
- хеш-таблица `HashMap<K, V>` с цепочками
- объект документа `Document`
- коллекция документов `Collection`
- собственный `Optional<T>`
- собственный JSON-парсер
- механизм фильтрации документов и логических операций ($and / $or)

База данных хранится в формате JSON в отдельном файле:

```
<database>.json
```

## 🚀 Запуск

```
./no_sql_dbms <database_name> <command> [json_query]
```

Пример:
```
./no_sql_dbms mydb find '{"age":{"$gt":"20"}}'
```

---

# ✨ Возможности

### ✔ Добавление документа  
### ✔ Поиск по одному и нескольким полям  
### ✔ Поддержка операторов:  
- `$eq` (неявно)  
- `$gt`  
- `$lt`  
- `$gte`  
- `$lte`  
- `$like`  
- `$in`  
- `$and`  
- `$or`  

### ✔ Удаление документов по условию  
### ✔ Вывод всей коллекции  
### ✔ Автоматическая сериализация в файл  
### ✔ Автоматическая перезагрузка из файла  
### ✔ Реализация без STL (кроме string / iostream)

---

# 📌 Поддерживаемый формат запросов

Все JSON-запросы должны быть заключены в одинарные кавычки `'...'`.

---

# 🧩 Команды

## INSERT

```
./no_sql_dbms testdb insert '{"name":"Alice","age":"25","city":"Paris"}'
./no_sql_dbms testdb insert '{"name":"Bob","age":"30","city":"London"}'
./no_sql_dbms testdb insert '{"name":"Eva","age":"22","city":"London","salary":"2500"}'
```

## PRINT

```
./no_sql_dbms testdb print
```

# FIND — Поиск

## Простое равенство

```
./no_sql_dbms testdb find '{"city":"Paris"}'
./no_sql_dbms testdb find '{"age":"30"}'
```

## Неявный AND

```
./no_sql_dbms testdb find '{"city":"Paris","age":"25"}'
```

## Операторы сравнения

```
./no_sql_dbms testdb find '{"age":{"$gt":"25"}}'
./no_sql_dbms testdb find '{"age":{"$gte":"30"}}'
./no_sql_dbms testdb find '{"age":{"$lt":"25"}}'
./no_sql_dbms testdb find '{"age":{"$lte":"22"}}'
```

## Диапазоны

```
./no_sql_dbms testdb find '{"age":{"$gt":"20","$lt":"30"}}'
./no_sql_dbms testdb find '{"age":{"$gte":"20","$lte":"30"}}'
```

## LIKE

```
./no_sql_dbms testdb find '{"city":{"$like":"%ar%"}}'
./no_sql_dbms testdb find '{"city":{"$like":"L%"}}'
./no_sql_dbms testdb find '{"city":{"$like":"%s"}}'
```

## IN

```
./no_sql_dbms testdb find '{"city":{"$in":["Paris","London"]}}'
./no_sql_dbms testdb find '{"age":{"$in":["19","22","28"]}}'
```

## AND

```
./no_sql_dbms testdb find '{"$and":[{"city":"Paris"},{"age":{"$gt":"20"}}]}'
```

## OR

```
./no_sql_dbms testdb find '{"$or":[{"city":"Paris"},{"city":"London"}]}'
```

# Сложные запросы

```
./no_sql_dbms testdb find '{"$and":[{"$or":[{"city":"Paris"},{"city":"London"}]},{"age":{"$gt":"20"}},{"salary":{"$lt":"3500"}}]}'
```

```
./no_sql_dbms testdb find '{"$or":[{"$and":[{"city":"Paris"},{"age":{"$lt":"30"}}]},{"$and":[{"city":"Berlin"},{"salary":{"$gt":"3000"}}]}]}'
```

# DELETE

```
./no_sql_dbms testdb delete '{"city":"Paris"}'
./no_sql_dbms testdb delete '{"age":"30"}'
```

# Структура файлов

```
/src
  ├── Array.h
  ├── SinglyList.h
  ├── HashMap.h
  ├── Optional.h
  ├── Document.h / .cpp
  ├── Collection.h / .cpp
  ├── Parser.h / .cpp
  ├── Compare.h / .cpp
  └── main.cpp
```

# Архитектура

## HashMap<K,V>
- собственная хеш-таблица
- разрешение коллизий цепочками
- автоматическое rehash
- хранит документы по ID

## Document
- хранит key:value
- вывод, доступ к полям

## Collection
- find / insert / delete / print
- сериализация в JSON

## ComparsionOperator
Поддерживает:
- $eq, $gt, $lt, $gte, $lte  
- $like  
- $in  
- ranges  
- $and / $or

# Автор
**Никита Н.Р.**
