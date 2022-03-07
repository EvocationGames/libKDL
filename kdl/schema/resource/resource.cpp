// Copyright (c) 2022 Tom Hancocks
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

#include <kdl/schema/resource/resource.hpp>
#include <kdl/schema/resource_type/resource_type.hpp>

// MARK: - Construction

kdl::lib::resource::resource(const std::shared_ptr<resource_type>& type, int64_t id, const std::string& name)
    : m_type(type), m_id(id), m_name(name)
{

}

// MARK: - Value Look Up

auto kdl::lib::resource::value(const std::string &field_name) const -> kdl::lib::lexeme
{
    auto it = m_values.find(field_name);
    if (it == m_values.end()) {
        return kdl::lib::lexeme(lexeme_type::unknown);
    }
    return it->second;
}

auto kdl::lib::resource::set_value(const kdl::lib::lexeme &lx, const std::string &field_name) -> void
{
    m_values.insert(std::pair(field_name, lx));
}
