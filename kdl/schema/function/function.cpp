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

#include <kdl/schema/function/function.hpp>
#include <kdl/schema/function/function_argument.hpp>
#include <kdl/schema/binary_type/binary_type.hpp>

// MARK: - Construction

kdl::lib::function::function(const std::string& name, const std::shared_ptr<struct binary_type>& type)
    : m_name(name), m_construction_type(type)
{

}

// MARK: - Accessors

auto kdl::lib::function::name() const -> std::string
{
    return  m_name;
}

auto kdl::lib::function::construction_type() const -> std::weak_ptr<struct binary_type>
{
    return m_construction_type;
}

auto kdl::lib::function::arguments() const -> std::vector<struct function_argument>
{
    return m_arguments;
}

// MARK: - Function Arguments

auto kdl::lib::function::add_argument(const struct function_argument& arg) -> void
{
    m_arguments.emplace_back(arg);
}

auto kdl::lib::function::argument_type_at(std::size_t i) const -> std::weak_ptr<struct binary_type>
{
    return m_arguments.at(i).type();
}

// MARK: - Function Body

auto kdl::lib::function::set_body(const std::vector<lexeme> &body) -> void
{
    m_body = body;
}

auto kdl::lib::function::body() const -> std::vector<lexeme>
{
    return m_body;
}
