#include "json/Parser.h"
#include "json/Serializer.h"
#include <iostream>

int main() {
    std::string json = R"({
        "server": {
            "host": "localhost",
            "port": 8080,
            "ssl": true
        },
        "users": [
            {"name": "Alice", "age": 30},
            {"name": "Bob", "age": 25}
        ],
        "features": ["chat", "video", "audio"],
        "note": "line1\nline2\t\"quoted\""
    })";

    std::cout << "=== Original input ===\n" << json << "\n\n";

    json::Value v = json::Parser::Parse(json);

    std::cout << "=== Compact output ===\n";
    std::cout << json::Serializer::ToString(v, true) << "\n\n";

    std::cout << "=== Pretty output ===\n";
    std::cout << json::Serializer::ToString(v, false) << "\n\n";

    // Round-trip: parse → serialize → parse → сравнить
    std::string compact = json::Serializer::ToString(v, true);
    json::Value v2 = json::Parser::Parse(compact);
    std::cout << "Round-trip equal: " << (v == v2 ? "YES" : "NO") << "\n";

    return 0;
}