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

#include <kdl/lexer/lexeme.hpp>

// MARK: - Constructor

kdl::lexeme::lexeme(lexeme_type type, const std::string& value)
    : m_type(type), m_value(value)
{

}

kdl::lexeme::lexeme(kdl::lexeme_type type, kdl::file_reference ref, const std::string& value)
    : m_type(type), m_ref(std::move(ref)), m_value(value)
{

}

// MARK: - Accessors

auto kdl::lexeme::string_value() const -> std::string
{
    return m_value;
}

auto kdl::lexeme::is_temporary() const -> bool
{
    return !m_ref.valid();
}

auto kdl::lexeme::file_reference() const -> kdl::file_reference
{
    return m_ref;
}

auto kdl::lexeme::type() const -> lexeme_type
{
    return m_type;
}


// MARK: - Identity Functions

auto kdl::lexeme::is(const std::string& value) const -> bool
{
    return m_value == value;
}

auto kdl::lexeme::is(lexeme_type type) const -> bool
{
    return m_type == type;
}

auto kdl::lexeme::is(lexeme_type type, const std::string& value) const -> bool
{
    return is(type) && is(value);
}

// MARK: - Conversion Functions

auto kdl::lexeme::int8_value() const -> int8_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<int8_t>(std::stol(m_value));
}

auto kdl::lexeme::uint8_value() const -> uint8_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<uint8_t>(std::stoul(m_value));
}

auto kdl::lexeme::int16_value() const -> int16_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<int16_t>(std::stol(m_value));
}

auto kdl::lexeme::uint16_value() const -> uint16_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<uint16_t>(std::stoul(m_value));
}

auto kdl::lexeme::int32_value() const -> int32_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<int32_t>(std::stol(m_value));
}

auto kdl::lexeme::uint32_value() const -> uint32_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<uint32_t>(std::stoul(m_value));
}

auto kdl::lexeme::int64_value() const -> int64_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<int64_t>(std::stoll(m_value));
}

auto kdl::lexeme::uint64_value() const -> uint64_t
{
    // TODO: Handle overflow and underflow errors
    return static_cast<uint64_t>(std::stoull(m_value));
}

auto kdl::lexeme::double_value() const -> double
{
    // TODO: Handle overflow and underflow errors
    return static_cast<double>(std::stod(m_value));
}

auto kdl::lexeme::long_double_value() const -> long double
{
    // TODO: Handle overflow and underflow errors
    return static_cast<long double>(std::stold(m_value));
}
