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

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <kdl/lexer/lexeme.hpp>
#include <kdl/schema/binary_type/isa.hpp>
#include <kdl/schema/binary_type/char_encoding.hpp>

namespace kdl::lib
{
    struct function;

    struct binary_type
    {
    public:
        enum class size_type { width, count, null_terminated, fixed };

        explicit binary_type(const std::string& name);

        auto set_isa(binary_type_isa isa) -> void;
        auto set_signed(bool f) -> void;
        auto set_size(const lexeme& size, enum size_type type = size_type::width) -> void;
        auto set_char_encoding(binary_type_char_encoding enc) -> void;
        auto add_function(const std::shared_ptr<struct function>& fn) -> void;
        auto set_attachments(const std::vector<lexeme>& attachments) -> void;

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto isa() const -> binary_type_isa;
        [[nodiscard]] auto is_signed() const -> bool;
        [[nodiscard]] auto size_type() const -> enum size_type;
        [[nodiscard]] auto size(const std::unordered_map<std::string, lexeme>& vars = {}) const -> std::size_t;
        [[nodiscard]] auto char_encoding() const -> binary_type_char_encoding;
        [[nodiscard]] auto function_named(const std::string& name) const -> std::weak_ptr<struct function>;
        [[nodiscard]] auto attachments() const -> std::vector<lexeme>;

        [[nodiscard]] auto is_null_terminated() const -> bool;
        [[nodiscard]] auto is_counted() const -> bool;
        [[nodiscard]] auto is_fixed_size() const -> bool;

    private:
        std::string m_name;
        binary_type_isa m_isa { binary_type_isa::integer };
        bool m_signed { false };
        enum size_type m_size_type { size_type::width };
        lexeme m_size { lexeme_type::integer, "1" };
        binary_type_char_encoding m_encoding { binary_type_char_encoding::macroman };
        std::vector<std::weak_ptr<struct function>> m_functions;
        std::vector<lexeme> m_attachments;
    };
}
