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

#include <kdl/schema/binary_template/binary_template.hpp>
#include <kdl/schema/binary_template/binary_template_field.hpp>

// MARK: - Construction

kdl::lib::binary_template::binary_template(const std::string& name)
    : m_name(name)
{

}

// MARK: - Field Management

auto kdl::lib::binary_template::add_field(const std::shared_ptr<binary_type>& type, const std::string& name) -> void
{
    m_fields.emplace_back(std::make_shared<binary_template_field>(type, name));
}

// MARK: - Field Querying and Accessors

auto kdl::lib::binary_template::field_count() const -> std::size_t
{
    return m_fields.size();
}

auto kdl::lib::binary_template::field_at(std::size_t i) const -> std::shared_ptr<binary_template_field>
{
    return m_fields.at(i);
}

auto kdl::lib::binary_template::field_named(const std::string& name) const -> std::shared_ptr<binary_template_field>
{
    return nullptr;
}

