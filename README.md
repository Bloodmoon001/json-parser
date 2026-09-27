# JSON Parser

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-green.svg)](https://cmake.org/)
[![Tests](https://img.shields.io/badge/tests-34%20passed-brightgreen.svg)](#tests)

A dependency-free JSON parser and serializer written in C++17.

JSON-парсер и сериализатор на C++17, написанный с нуля, без сторонних библиотек.

---

## 🇬🇧 English

### About
This project is a **fully functional JSON library** implemented from scratch in modern C++17. It demonstrates recursion, `std::variant`, move semantics, exception handling with source positions, and a clean layered architecture.

### Features
* **Full JSON support** — all 6 value types: `null`, `bool`, `number`, `string`, `array`, `object`.
* **Recursive descent parser** — clean, readable, with precise error positions.
* **Serializer** — both compact and pretty-printed output.
* **Round-trip correctness** — `parse → serialize → parse` produces an identical value.
* **Escape sequences** — including `\uXXXX` Unicode escapes.
* **Unit tests** — 34 tests using Google Test, covering parser, serializer and error cases.
* **Architecture** — the library is header-based and can be included into any C++ project.

### Project Structure
```
json-parser/
├── include/json/     # Public headers (Value, Parser, Serializer)
├── src/              # Implementation
├── tests/            # Google Test unit tests
├── apps/             # Demo executable
└── CMakeLists.txt
```

### Built With
* **C++17** — `std::variant`, `std::optional`, structured bindings
* **CMake** — build management
* **Google Test** — unit testing
* **vcpkg** — dependency management

### Getting Started

#### Prerequisites
* Visual Studio 2022 (with "Desktop development with C++")
* CMake 3.20+
* vcpkg

#### Build & Run
1. Install dependencies via vcpkg:
   ```bash
   vcpkg install gtest:x64-windows
   ```
2. Clone the repository:
   ```bash
   git clone https://github.com/Bloodmoon001/json-parser.git
   cd json-parser
   ```
3. Open the folder in Visual Studio (`File` → `Open` → `Folder`). VS will auto-configure CMake.
4. Press `F5` to run the demo, or select `json_tests.exe` to run the tests.

### Example Usage
```cpp
#include "json/Parser.h"
#include "json/Serializer.h"
#include <iostream>

int main() {
    auto value = json::Parser::Parse(R"({"name":"Alice","age":30})");

    std::cout << value.asObject().at("name").asString() << "\n";  // Alice
    std::cout << value.asObject().at("age").asNumber()  << "\n";  // 30

    std::cout << json::Serializer::ToString(value, false) << "\n";
    // {
    //   "age": 30,
    //   "name": "Alice"
    // }
}
```

### Tests
```bash
./bin/json_tests.exe
```
Expected output: `[  PASSED  ] 34 tests.`

### License
MIT License. See `LICENSE` for details.

---

## 🇷🇺 Русский

### О проекте
Это **полноценная библиотека для работы с JSON**, написанная с нуля на современном C++17. Проект демонстрирует рекурсию, `std::variant`, семантику перемещения, обработку исключений с указанием позиции и чистую слоистую архитектуру.

### Возможности
* **Полная поддержка JSON** — все 6 типов значений: `null`, `bool`, `number`, `string`, `array`, `object`.
* **Парсер рекурсивного спуска** — чистый код с точными сообщениями об ошибках.
* **Сериализатор** — компактный и pretty-printed вывод.
* **Round-trip корректность** — `parse → serialize → parse` даёт идентичное значение.
* **Escape-последовательности** — включая Unicode `\uXXXX`.
* **Unit-тесты** — 34 теста на Google Test: парсер, сериализатор и ошибки.
* **Архитектура** — библиотека подключается в любой C++ проект.

### Структура проекта
```
json-parser/
├── include/json/     # Публичные заголовки (Value, Parser, Serializer)
├── src/              # Реализация
├── tests/            # Unit-тесты Google Test
├── apps/             # Демо-программа
└── CMakeLists.txt
```

### Технологии
* **C++17** — `std::variant`, `std::optional`, structured bindings
* **CMake** — система сборки
* **Google Test** — unit-тестирование
* **vcpkg** — управление зависимостями

### Сборка и запуск

#### Требования
* Visual Studio 2022 (с рабочей нагрузкой «Разработка классических приложений на C++»)
* CMake 3.20+
* vcpkg

#### Сборка
1. Установите зависимости через vcpkg:
   ```bash
   vcpkg install gtest:x64-windows
   ```
2. Склонируйте репозиторий:
   ```bash
   git clone https://github.com/Bloodmoon001/json-parser.git
   cd json-parser
   ```
3. Откройте папку в Visual Studio (`Файл` → `Открыть` → `Папка`). VS сам настроит CMake.
4. Нажмите `F5` для запуска демо или выберите `json_tests.exe` для тестов.

### Пример использования
```cpp
#include "json/Parser.h"
#include "json/Serializer.h"
#include <iostream>

int main() {
    auto value = json::Parser::Parse(R"({"name":"Alice","age":30})");

    std::cout << value.asObject().at("name").asString() << "\n";  // Alice
    std::cout << value.asObject().at("age").asNumber()  << "\n";  // 30

    std::cout << json::Serializer::ToString(value, false) << "\n";
}
```

### Тесты
```bash
./bin/json_tests.exe
```
Ожидаемый вывод: `[  PASSED  ] 34 tests.`

### Лицензия
MIT License. См. файл `LICENSE`.

---