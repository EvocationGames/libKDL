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

#if !defined(KDL_SCHEMA_BINARY_TYPE_HPP)
#define KDL_SCHEMA_BINARY_TYPE_HPP

#include <string>
#include <memory>
#include <kdl/schema/binary_type/isa.hpp>
#include <kdl/schema/binary_type/char_encoding.hpp>

namespace kdl::lib
{
    struct binary_type
    {
    private:
        std::string m_name;
        binary_type_isa m_isa { binary_type_isa::integer };
        bool m_signed { false };
        bool m_null_terminated { false };
        std::size_t m_size { 1 };
        std::size_t m_count_width { 0 };
        binary_type_char_encoding m_encoding { binary_type_char_encoding::macroman };

    public:
        explicit binary_type(const std::string& name);

        auto set_isa(binary_type_isa isa) -> void;
        auto set_signed(bool f) -> void;
        auto set_null_terminated(bool f) -> void;
        auto set_size(std::size_t size) -> void;
        auto set_count_width(std::size_t count) -> void;
        auto set_char_encoding(binary_type_char_encoding enc) -> void;

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto isa() const -> binary_type_isa;
        [[nodiscard]] auto is_signed() const -> bool;
        [[nodiscard]] auto is_null_terminated() const -> bool;
        [[nodiscard]] auto size() const -> std::size_t;
        [[nodiscard]] auto count_width() const -> std::size_t;
        [[nodiscard]] auto char_encoding() const -> binary_type_char_encoding;

    };
}

#endif //KDL_SCHEMA_BINARY_TYPES_BINARY_TYPE_HPP
