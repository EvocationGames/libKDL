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

#include <kdl/exe/variable.hpp>
#include <kdl/schema/binary_type/binary_type.hpp>

// MARK: - Construction

kdl::lib::exe::variable::variable(const std::string& name, const std::shared_ptr<struct binary_type>& type, const lexeme& value)
    : m_name(name), m_type(type), m_value(value)
{

}

// MARK: - Accessors

auto kdl::lib::exe::variable::name() const -> std::string
{
    return m_name;
}

auto kdl::lib::exe::variable::type() const -> std::shared_ptr<struct binary_type>
{
    return m_type;
}

auto kdl::lib::exe::variable::value() const -> lexeme
{
    return m_value;
}

// MARK: - Setters

auto kdl::lib::exe::variable::set_value(const lexeme& value) -> void
{
    m_value = value;
}
