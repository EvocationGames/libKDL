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

#include <kdl/schema/binary_types/binary_type.hpp>

// MARK: - Construction

kdl::lib::binary_type::binary_type(std::string name)
    : m_name(std::move(name))
{

}

// MARK: - Setters

auto kdl::lib::binary_type::set_isa(binary_type_isa isa) -> void
{
    m_isa = isa;
}

auto kdl::lib::binary_type::set_signed(bool f) -> void
{
    m_signed = f;
}

auto kdl::lib::binary_type::set_null_terminated(bool f) -> void
{
    m_null_terminated = f;
}

auto kdl::lib::binary_type::set_size(std::size_t size) -> void
{
    m_size = size;
}

auto kdl::lib::binary_type::set_count_width(std::size_t count) -> void
{
    m_count_width = count;
}

auto kdl::lib::binary_type::set_char_encoding(binary_type_char_encoding enc) -> void
{
    m_encoding = enc;
}

// MARK: - Accessors

auto kdl::lib::binary_type::name() const -> std::string
{
    return m_name;
}

auto kdl::lib::binary_type::isa() const -> binary_type_isa
{
    return m_isa;
}

auto kdl::lib::binary_type::is_signed() const -> bool
{
    return m_signed;
}

auto kdl::lib::binary_type::is_null_terminated() const -> bool
{
    return m_null_terminated;
}

auto kdl::lib::binary_type::size() const -> std::size_t
{
    return m_size;
}

auto kdl::lib::binary_type::count_width() const -> std::size_t
{
    return m_count_width;
}

auto kdl::lib::binary_type::char_encoding() const -> binary_type_char_encoding
{
    return m_encoding;
}
