// Copyright (c) 2021 Tom Hancocks
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#if !defined(KDL_SCHEMA_RESOURCE_FIELD_VALUE_HPP)
#define KDL_SCHEMA_RESOURCE_FIELD_VALUE_HPP

#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <kdl/lexer/lexeme.hpp>

namespace kdl::lib
{
    struct binary_template_field;
    struct resource_field_symbol;

    struct resource_field_value
    {
    private:
        std::string m_binary_template_field_name;
        std::weak_ptr<struct binary_template_field> m_binary_template_field;
        std::vector<std::shared_ptr<struct resource_field_symbol>> m_symbols;
        std::optional<lexeme> m_default_value;

    public:
        explicit resource_field_value(const std::shared_ptr<struct binary_template_field>& field);

        auto set_default_value(const lexeme& lx) -> void;

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto default_value() const -> std::optional<lexeme>;

        auto add_symbol(const std::shared_ptr<struct resource_field_symbol>& symbol) -> void;
        [[nodiscard]] auto symbol_named(const std::string& name) const -> std::weak_ptr<struct resource_field_symbol>;
        [[nodiscard]] auto symbols() const -> const std::vector<std::shared_ptr<struct resource_field_symbol>>&;

        [[nodiscard]] auto binary_template_field() const -> std::shared_ptr<struct binary_template_field>;
        [[nodiscard]] auto expected_value_lexeme_type() const -> lexeme_type;
    };
}

#endif //KDL_SCHEMA_RESOURCE_TYPE_RESOURCE_FIELD_VALUE_HPP
