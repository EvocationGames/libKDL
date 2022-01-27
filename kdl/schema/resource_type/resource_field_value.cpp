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

#include <kdl/schema/binary_template/binary_template_field.hpp>
#include <kdl/schema/binary_type/binary_type.hpp>
#include <kdl/schema/resource_type/resource_field_value.hpp>
#include <kdl/schema/resource_type/resource_field_symbol.hpp>

// MARK: - Construction

kdl::lib::resource_field_value::resource_field_value(const std::shared_ptr<struct binary_template_field>& field)
    : m_binary_template_field_name(field->name()), m_binary_template_field(field)
{

}

// MARK: - Accessors

auto kdl::lib::resource_field_value::name() const -> std::string
{
    return m_binary_template_field_name;
}

auto kdl::lib::resource_field_value::default_value() const -> std::optional<lexeme>
{
    return m_default_value;
}

// MARK: - Setters

auto kdl::lib::resource_field_value::set_default_value(const kdl::lib::lexeme &lx) -> void
{
    m_default_value = { lx };
}

// MARK: - Data/Value Types

auto kdl::lib::resource_field_value::binary_template_field() const -> std::shared_ptr<struct binary_template_field>
{
    return m_binary_template_field.lock();
}

auto kdl::lib::resource_field_value::expected_value_lexeme_type() const -> kdl::lib::lexeme_type
{
    if (auto bin_type = m_binary_template_field.lock()) {
        switch (bin_type->type()->isa()) {
            case binary_type_isa::integer:  return lexeme_type::integer;
            case binary_type_isa::string:   return lexeme_type::string;
            case binary_type_isa::color:    return lexeme_type::integer;
        }
    }
    return lexeme_type::integer;
}

// MARK: - Symbol Management

auto kdl::lib::resource_field_value::add_symbol(const std::shared_ptr<struct resource_field_symbol> &symbol) -> void
{
    m_symbols.emplace_back(symbol);
}

auto kdl::lib::resource_field_value::symbol_named(const std::string &name) const -> std::weak_ptr<struct resource_field_symbol>
{
    for (auto& sym : m_symbols) {
        if (sym->name() == name) {
            return sym;
        }
    }
    return {};
}

auto kdl::lib::resource_field_value::symbols() const -> const std::vector<std::shared_ptr<struct resource_field_symbol>> &
{
    return m_symbols;
}
