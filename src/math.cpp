#include "math.h"
#include "exprtk.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>

double evalExpression(const std::string& expr) {
    using symbol_table_t = exprtk::symbol_table<double>;
    using expression_t   = exprtk::expression<double>;
    using parser_t       = exprtk::parser<double>;

    // Нормализация: убираем пробелы по краям
    std::string s = expr;
    auto not_space = [](unsigned char c){ return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());

    symbol_table_t symbol_table;
    // Можно добавить пользовательские переменные/константы:
    // double pi = 3.14159265358979323846;
    // symbol_table.add_constant("pi", pi);

    expression_t expression;
    expression.register_symbol_table(symbol_table);

    parser_t parser;
    if (!parser.compile(s, expression)) {
        // Сформируем понятное сообщение
        throw std::runtime_error("Ошибка парсинга математического выражения");
    }

    return expression.value();
}

bool isMathExpression(const std::string& expr) {
    // Эвристика: присутствует хотя бы одна цифра и один из операторов/скобок
    bool has_digit = false;
    bool has_op = false;
    for (unsigned char c : expr) {
        if (std::isdigit(c)) has_digit = true;
        if (c=='+' || c=='-' || c=='*' || c=='/' || c=='^' || c=='(' || c==')')
            has_op = true;
        if (has_digit && has_op) return true;
    }
    return false;
}