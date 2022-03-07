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

#include <vector>
#include <sstream>
#include <string>
#include <kdl/lexer/lexeme.hpp>

// MARK: - Constructor

kdl::lib::lexeme::lexeme()
    : m_type(lexeme_type::unknown), m_value("")
{
}

kdl::lib::lexeme::lexeme(lexeme_type type, const std::string& value)
    : m_type(type), m_value(value)
{
}

kdl::lib::lexeme::lexeme(lexeme_type type, kdl::lib::file_reference ref, const std::string& value)
    : m_type(type), m_ref(std::move(ref)), m_value(value)
{

}

// MARK: - Accessors

auto kdl::lib::lexeme::string_value() const -> std::string
{
    return m_value;
}

auto kdl::lib::lexeme::is_temporary() const -> bool
{
    return !m_ref.valid();
}

auto kdl::lib::lexeme::file_reference() const -> kdl::lib::file_reference
{
    return m_ref;
}

auto kdl::lib::lexeme::type() const -> lexeme_type
{
    return m_type;
}


// MARK: - Identity Functions

auto kdl::lib::lexeme::is(const std::string& value) const -> bool
{
    return m_value == value;
}

auto kdl::lib::lexeme::is(lexeme_type type) const -> bool
{
    if (type == lexeme_type::any_integer) {
        return (
            m_type == lexeme_type::integer ||
            m_type == lexeme_type::percentage ||
            m_type == lexeme_type::hex ||
            m_type == lexeme_type::resource_ref
        );
    }
    else if (type == lexeme_type::any_string) {
        return (
            m_type == lexeme_type::string
        );
    }
    else {
        return (m_type == type);
    }
}

auto kdl::lib::lexeme::is(lexeme_type type, const std::string& value) const -> bool
{
    return is(type) && is(value);
}

auto kdl::lib::lexeme::is_one_of(const std::initializer_list<lexeme_type> &type) const -> bool
{
    return std::any_of(type.begin(), type.end(), [&] (const auto& it) {
        return this->is(it);
    });
}

// MARK: - Conversion Functions

auto kdl::lib::lexeme::base() const -> uint8_t
{
    switch (m_type) {
        case lexeme_type::color:
        case lexeme_type::hex:
            return 16;
        default:
            return 10;
    }
}

auto kdl::lib::lexeme::int8_value() const -> int8_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<int8_t>(std::stol(m_value, nullptr, base()));
}

auto kdl::lib::lexeme::uint8_value() const -> uint8_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<uint8_t>(std::stoul(m_value, nullptr, base()));
}

auto kdl::lib::lexeme::int16_value() const -> int16_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<int16_t>(std::stol(m_value, nullptr, base()));
}

auto kdl::lib::lexeme::uint16_value() const -> uint16_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<uint16_t>(std::stoul(m_value, nullptr, base()));
}

auto kdl::lib::lexeme::int32_value() const -> int32_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<int32_t>(std::stol(m_value, nullptr, base()));
}

auto kdl::lib::lexeme::uint32_value() const -> uint32_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<uint32_t>(std::stoul(m_value, nullptr, base()));
}

auto kdl::lib::lexeme::int64_value() const -> int64_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<int64_t>(std::stoll(m_value, nullptr, base()));
}

auto kdl::lib::lexeme::uint64_value() const -> uint64_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<uint64_t>(std::stoull(m_value, nullptr, base()));
}

auto kdl::lib::lexeme::double_value() const -> double
{
    // TODO: Handle overflow and underflow errors
    return static_cast<double>(std::stod(m_value));
}

auto kdl::lib::lexeme::long_double_value() const -> long double
{
    // TODO: Handle overflow and underflow errors
    return static_cast<long double>(std::stold(m_value));
}

// MARK: - Resource Reference Specific

static inline auto resource_reference_split(const std::string& ref) -> std::vector<std::string>
{
    std::stringstream stream(ref);
    std::vector<std::string> components;
    std::string segment;
    while (std::getline(stream, segment,'.')) {
        components.emplace_back(segment);
    }
    return components;
}

auto kdl::lib::lexeme::resource_id() const -> int64_t
{
    if (is(lexeme_type::resource_ref)) {
        auto components = resource_reference_split(m_value);
        return static_cast<int64_t>(std::stoll(components.back()));
    }
    return 0;
}

auto kdl::lib::lexeme::resource_type() const -> std::string
{
    if (is(lexeme_type::resource_ref)) {
        auto components = resource_reference_split(m_value);
        if (components.size() >= 2) {
            return components[components.size() - 2];
        }
    }
    return "";
}

// MARK: - Description

auto kdl::lib::lexeme::describe() const -> std::string
{
    return m_ref.describe() + " - " + describe_lexeme_type(m_type) + "<" + m_value + ">";
}