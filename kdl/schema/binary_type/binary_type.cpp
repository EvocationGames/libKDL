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

#include <kdl/schema/binary_type/binary_type.hpp>
#include <kdl/schema/function/function.hpp>
#include <kdl/report/reporting.hpp>

// MARK: - Construction

kdl::lib::binary_type::binary_type(const std::string& name)
    : m_name(name)
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

auto kdl::lib::binary_type::set_size(const lexeme& size, enum size_type type) -> void
{
    m_size = size;
    m_size_type = type;
}

auto kdl::lib::binary_type::set_char_encoding(binary_type_char_encoding enc) -> void
{
    m_encoding = enc;
}

auto kdl::lib::binary_type::add_function(const std::shared_ptr<struct function>& fn) -> void
{
    m_functions.emplace_back(fn);
}

auto kdl::lib::binary_type::set_attachments(const std::vector<lexeme> &attachments) -> void
{
    m_attachments = attachments;
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

auto kdl::lib::binary_type::size_type() const -> enum size_type
{
    return m_size_type;
}


auto kdl::lib::binary_type::size(const std::unordered_map<std::string, lexeme>& vars) const -> std::size_t
{
    if (m_size.is(lexeme_type::var)) {
        auto it = vars.find(m_size.string_value());
        if (it != vars.end()) {
            return it->second.uint64_value();
        }
        else {
            report::error(m_size, "Variable not found.");
        }
    }
    return m_size.uint64_value();
}

auto kdl::lib::binary_type::char_encoding() const -> binary_type_char_encoding
{
    return m_encoding;
}

auto kdl::lib::binary_type::function_named(const std::string& name) const -> std::weak_ptr<struct function>
{
    for (auto& weak : m_functions) {
        auto fn = weak.lock();
        if (fn && fn->name() == name) {
            return fn;
        }
    }
    return {};
}

auto kdl::lib::binary_type::attachments() const -> std::vector<lexeme>
{
    return m_attachments;
}

auto kdl::lib::binary_type::is_null_terminated() const -> bool
{
    return m_size_type == size_type::null_terminated;
}

auto kdl::lib::binary_type::is_counted() const -> bool
{
    return m_size_type == size_type::count;
}

auto kdl::lib::binary_type::is_fixed_size() const -> bool
{
    return m_size_type == size_type::fixed;
}
