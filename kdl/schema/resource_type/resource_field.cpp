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

#include <kdl/schema/resource_type/resource_field.hpp>
#include <kdl/schema/resource_type/resource_field_value.hpp>

// MARK: - Constructor

kdl::lib::resource_field::resource_field(const std::string& name)
    : m_name(name)
{

}

kdl::lib::resource_field::resource_field(const std::shared_ptr<struct resource_field_value>& field)
    : m_name(field->name()), m_values({ field })
{

}

// MARK: - Accessor

auto kdl::lib::resource_field::name() const -> std::string
{
    return m_name;
}

// MARK: - Field Value Management

auto kdl::lib::resource_field::add_value(const std::shared_ptr<struct resource_field_value> &value) -> void
{
    m_values.emplace_back(value);
}

auto kdl::lib::resource_field::values() const -> const std::vector<std::shared_ptr<struct resource_field_value>> &
{
    return m_values;
}
