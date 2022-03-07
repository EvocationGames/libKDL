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

#include <kdl/schema/resource_type/resource_type.hpp>
#include <kdl/schema/resource_type/resource_field.hpp>
#include <kdl/schema/binary_template/binary_template.hpp>

// MARK: - Construction

kdl::lib::resource_type::resource_type(const std::string &name, const std::string &code)
    : m_name(name), m_code(code)
{

}

// MARK: - Accessors

auto kdl::lib::resource_type::name() const -> std::string
{
    return m_name;
}

auto kdl::lib::resource_type::code() const -> std::string
{
    return m_code;
}

// MARK: - Templates

auto kdl::lib::resource_type::set_binary_template(const std::weak_ptr<struct binary_template>& tmpl) -> void
{
    m_template = tmpl;
}

auto kdl::lib::resource_type::binary_template() const -> std::weak_ptr<struct binary_template>
{
    return m_template;
}

// MARK: - Field Management

auto kdl::lib::resource_type::add_field(const std::shared_ptr<struct resource_field> &field) -> void
{
    m_fields.emplace_back(field);
}

auto kdl::lib::resource_type::fields() const -> const std::vector<std::shared_ptr<resource_field>>&
{
    return m_fields;
}

auto kdl::lib::resource_type::field_named(const std::string &name) const -> std::shared_ptr<resource_field>
{
    for (const auto& field : m_fields) {
        if (field->name() == name) {
            return field;
        }
    }
    return nullptr;
}
