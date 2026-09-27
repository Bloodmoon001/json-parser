#include <gtest/gtest.h>
#include "json/Parser.h"

using json::Parser;
using json::ParseError;
using json::Value;

TEST(Parser, Null) {
    Value v = Parser::Parse("null");
    EXPECT_TRUE(v.isNull());
}

TEST(Parser, True) {
    Value v = Parser::Parse("true");
    EXPECT_TRUE(v.isBool());
    EXPECT_TRUE(v.asBool());
}

TEST(Parser, False) {
    Value v = Parser::Parse("false");
    EXPECT_TRUE(v.isBool());
    EXPECT_FALSE(v.asBool());
}

TEST(Parser, Integer) {
    EXPECT_DOUBLE_EQ(Parser::Parse("42").asNumber(), 42.0);
    EXPECT_DOUBLE_EQ(Parser::Parse("-7").asNumber(), -7.0);
    EXPECT_DOUBLE_EQ(Parser::Parse("0").asNumber(), 0.0);
}

TEST(Parser, Decimal) {
    EXPECT_DOUBLE_EQ(Parser::Parse("3.14").asNumber(), 3.14);
    EXPECT_DOUBLE_EQ(Parser::Parse("-0.5").asNumber(), -0.5);
}

TEST(Parser, Exponent) {
    EXPECT_DOUBLE_EQ(Parser::Parse("1e10").asNumber(), 1e10);
    EXPECT_DOUBLE_EQ(Parser::Parse("2.5E-3").asNumber(), 2.5e-3);
}

TEST(Parser, WhitespaceAroundValue) {
    EXPECT_TRUE(Parser::Parse("   null   ").isNull());
    EXPECT_DOUBLE_EQ(Parser::Parse("\n\t 42 \r\n").asNumber(), 42.0);
}

TEST(Parser, Errors) {
    EXPECT_THROW(Parser::Parse(""), ParseError);
    EXPECT_THROW(Parser::Parse("nul"), ParseError);
    EXPECT_THROW(Parser::Parse("tru"), ParseError);
    EXPECT_THROW(Parser::Parse("nullx"), ParseError);
    EXPECT_THROW(Parser::Parse("01"), ParseError);
    EXPECT_THROW(Parser::Parse("1."), ParseError);
    EXPECT_THROW(Parser::Parse("+5"), ParseError);
    EXPECT_THROW(Parser::Parse("--5"), ParseError);
}

TEST(Parser, ErrorHasPosition) {
    try {
        Parser::Parse("  xx");
        FAIL() << "Expected ParseError";
    }
    catch (const ParseError& e) {
        EXPECT_EQ(e.position(), 2u);
    }
}

TEST(Parser, SimpleString) {
    EXPECT_EQ(Parser::Parse(R"("hello")").asString(), "hello");
    EXPECT_EQ(Parser::Parse(R"("")").asString(), "");
}

TEST(Parser, StringEscapes) {
    EXPECT_EQ(Parser::Parse(R"("a\"b")").asString(), "a\"b");
    EXPECT_EQ(Parser::Parse(R"("a\\b")").asString(), "a\\b");
    EXPECT_EQ(Parser::Parse(R"("a\nb")").asString(), "a\nb");
    EXPECT_EQ(Parser::Parse(R"("a\tb")").asString(), "a\tb");
    EXPECT_EQ(Parser::Parse(R"("a\/b")").asString(), "a/b");
    EXPECT_EQ(Parser::Parse(R"("a\bb")").asString(), "a\bb");
}

TEST(Parser, StringUnicodeEscape) {
    // \u0041 = 'A', \u0042 = 'B'
    EXPECT_EQ(Parser::Parse(R"("\u0041\u0042")").asString(), "AB");
}

TEST(Parser, StringErrors) {
    EXPECT_THROW(Parser::Parse(R"("unterminated)"), ParseError);
    EXPECT_THROW(Parser::Parse(R"("bad \x escape")"), ParseError);
    EXPECT_THROW(Parser::Parse("\"raw\nnewline\""), ParseError);
}

TEST(Parser, EmptyArray) {
    Value v = Parser::Parse("[]");
    EXPECT_TRUE(v.isArray());
    EXPECT_EQ(v.size(), 0u);
}

TEST(Parser, ArrayOfNumbers) {
    Value v = Parser::Parse("[1, 2, 3]");
    ASSERT_TRUE(v.isArray());
    ASSERT_EQ(v.size(), 3u);
    EXPECT_DOUBLE_EQ(v.asArray()[0].asNumber(), 1.0);
    EXPECT_DOUBLE_EQ(v.asArray()[1].asNumber(), 2.0);
    EXPECT_DOUBLE_EQ(v.asArray()[2].asNumber(), 3.0);
}

TEST(Parser, MixedArray) {
    Value v = Parser::Parse(R"(["a", true, null, 3.14])");
    ASSERT_TRUE(v.isArray());
    ASSERT_EQ(v.size(), 4u);
    EXPECT_TRUE(v.asArray()[0].isString());
    EXPECT_TRUE(v.asArray()[1].isBool());
    EXPECT_TRUE(v.asArray()[2].isNull());
    EXPECT_TRUE(v.asArray()[3].isNumber());
}

TEST(Parser, NestedArray) {
    Value v = Parser::Parse("[[1, 2], [3, 4]]");
    ASSERT_TRUE(v.isArray());
    ASSERT_EQ(v.size(), 2u);
    EXPECT_DOUBLE_EQ(v.asArray()[0].asArray()[1].asNumber(), 2.0);
    EXPECT_DOUBLE_EQ(v.asArray()[1].asArray()[0].asNumber(), 3.0);
}

TEST(Parser, ArrayErrors) {
    EXPECT_THROW(Parser::Parse("[1, 2,]"), ParseError);
    EXPECT_THROW(Parser::Parse("[1 2]"), ParseError);
    EXPECT_THROW(Parser::Parse("[1, 2"), ParseError);
}

TEST(Parser, EmptyObject) {
    Value v = Parser::Parse("{}");
    EXPECT_TRUE(v.isObject());
    EXPECT_EQ(v.size(), 0u);
}

TEST(Parser, SimpleObject) {
    Value v = Parser::Parse(R"({"name": "Alice", "age": 30})");
    ASSERT_TRUE(v.isObject());
    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(v.asObject().at("name").asString(), "Alice");
    EXPECT_DOUBLE_EQ(v.asObject().at("age").asNumber(), 30.0);
}

TEST(Parser, NestedObject) {
    Value v = Parser::Parse(R"({"a": {"b": {"c": 42}}})");
    EXPECT_DOUBLE_EQ(
        v.asObject().at("a").asObject().at("b").asObject().at("c").asNumber(),
        42.0);
}

TEST(Parser, ComplexDocument) {
    std::string json = R"({
        "users": [
            {"name": "Alice", "age": 30, "admin": true},
            {"name": "Bob",   "age": 25, "admin": false}
        ],
        "count": 2,
        "active": null
    })";
    Value v = Parser::Parse(json);
    ASSERT_TRUE(v.isObject());
    EXPECT_EQ(v.asObject().at("count").asNumber(), 2.0);
    EXPECT_TRUE(v.asObject().at("active").isNull());
    ASSERT_EQ(v.asObject().at("users").size(), 2u);
    EXPECT_EQ(v.asObject().at("users").asArray()[0].asObject().at("name").asString(), "Alice");
    EXPECT_TRUE(v.asObject().at("users").asArray()[1].asObject().at("admin").asBool() == false);
}

TEST(Parser, ObjectErrors) {
    EXPECT_THROW(Parser::Parse(R"({name: "x"})"), ParseError);  // key not string
    EXPECT_THROW(Parser::Parse(R"({"a" 1})"), ParseError);  // missing :
    EXPECT_THROW(Parser::Parse(R"({"a": 1,})"), ParseError);  // trailing comma
}
#include "json/Serializer.h"

using json::Serializer;

TEST(Serializer, Null) {
    EXPECT_EQ(Serializer::ToString(Value(nullptr)), "null");
}

TEST(Serializer, Bool) {
    EXPECT_EQ(Serializer::ToString(Value(true)), "true");
    EXPECT_EQ(Serializer::ToString(Value(false)), "false");
}

TEST(Serializer, Number) {
    EXPECT_EQ(Serializer::ToString(Value(42)), "42");
    EXPECT_EQ(Serializer::ToString(Value(-7)), "-7");
    EXPECT_EQ(Serializer::ToString(Value(0)), "0");
}

TEST(Serializer, String) {
    EXPECT_EQ(Serializer::ToString(Value("hello")), "\"hello\"");
    EXPECT_EQ(Serializer::ToString(Value("")), "\"\"");
}

TEST(Serializer, StringEscapes) {
    EXPECT_EQ(Serializer::ToString(Value("a\"b")), "\"a\\\"b\"");
    EXPECT_EQ(Serializer::ToString(Value("a\\b")), "\"a\\\\b\"");
    EXPECT_EQ(Serializer::ToString(Value("a\nb")), "\"a\\nb\"");
    EXPECT_EQ(Serializer::ToString(Value("a\tb")), "\"a\\tb\"");
}

TEST(Serializer, Array) {
    Value v(Value::Array{ Value(1), Value(2), Value(3) });
    EXPECT_EQ(Serializer::ToString(v), "[1,2,3]");
}

TEST(Serializer, EmptyArray) {
    Value v(Value::Array{});
    EXPECT_EQ(Serializer::ToString(v), "[]");
}

TEST(Serializer, Object) {
    Value v(Value::Object{
        { "a", Value(1) },
        { "b", Value("x") }
        });
    // std::map сортирует ключи, поэтому "a" идёт первым
    EXPECT_EQ(Serializer::ToString(v), "{\"a\":1,\"b\":\"x\"}");
}

TEST(Serializer, EmptyObject) {
    Value v(Value::Object{});
    EXPECT_EQ(Serializer::ToString(v), "{}");
}

TEST(Serializer, RoundTrip) {
    std::string input = R"({"a":[1,2,3],"b":{"c":true},"d":"x"})";
    Value v1 = Parser::Parse(input);
    std::string output = Serializer::ToString(v1);
    Value v2 = Parser::Parse(output);
    EXPECT_EQ(v1, v2);
}

TEST(Serializer, RoundTripComplex) {
    std::string input = R"({
        "users": [{"name":"Alice","age":30}],
        "flags": [true, false, null],
        "nested": {"a": {"b": {"c": [1,2,3]}}}
    })";
    Value v1 = Parser::Parse(input);
    std::string output = Serializer::ToString(v1);
    Value v2 = Parser::Parse(output);
    EXPECT_EQ(v1, v2);
}