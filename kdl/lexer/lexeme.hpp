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

#if !defined(KDL_LEXER_LEXEME_HPP)
#define KDL_LEXER_LEXEME_HPP

#include <string>
#include <memory>
#include <initializer_list>
#include <kdl/file/file_reference.hpp>
#include <kdl/lexer/lexeme_type.hpp>

namespace kdl::lib
{
    /* Denotes a Lexical Token contained within a source file.
     * It contains information about the token type, the textual content,
     * location in file and which file, as well as functions for converting
     * the value.
     */
    struct lexeme
    {
    private:
        lexeme_type m_type { lexeme_type::unknown };
        std::string m_value;
        file_reference m_ref;

    public:
        lexeme();
        explicit lexeme(lexeme_type type, const std::string& value = "");
        lexeme(lexeme_type type, file_reference ref, const std::string& value = "");

        [[nodiscard]] auto string_value() const -> std::string;
        [[nodiscard]] auto is_temporary() const -> bool;
        [[nodiscard]] auto file_reference() const -> file_reference;
        [[nodiscard]] auto type() const -> lexeme_type;

        [[nodiscard]] auto is(const std::string& value) const -> bool;
        [[nodiscard]] auto is(lexeme_type type) const -> bool;
        [[nodiscard]] auto is(lexeme_type type, const std::string& value) const -> bool;
        [[nodiscard]] auto is_one_of(const std::initializer_list<lexeme_type>& type) const -> bool;

        [[nodiscard]] auto base() const -> uint8_t;

        [[nodiscard]] auto int8_value() const -> int8_t;
        [[nodiscard]] auto uint8_value() const -> uint8_t;
        [[nodiscard]] auto int16_value() const -> int16_t;
        [[nodiscard]] auto uint16_value() const -> uint16_t;
        [[nodiscard]] auto int32_value() const -> int32_t;
        [[nodiscard]] auto uint32_value() const -> uint32_t;
        [[nodiscard]] auto int64_value() const -> int64_t;
        [[nodiscard]] auto uint64_value() const -> uint64_t;
        [[nodiscard]] auto double_value() const -> double;
        [[nodiscard]] auto long_double_value() const -> long double;

        [[nodiscard]] auto resource_id() const -> int64_t;
        [[nodiscard]] auto resource_type() const -> std::string;

        [[nodiscard]] auto describe() const -> std::string;

        template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
        [[nodiscard]] auto value() const -> T
        {
            if (is_one_of({ lexeme_type::star, lexeme_type::slash })) {
                return 2;
            }
            else if (is_one_of({ lexeme_type::plus, lexeme_type::minus })) {
                return 3;
            }
            else if (is_one_of({ lexeme_type::shift_left, lexeme_type::shift_right })) {
                return 4;
            }
            else if (is_one_of({ lexeme_type::amp })) {
                return 5;
            }
            else if (is_one_of({ lexeme_type::carat })) {
                return 6;
            }
            else if (is_one_of({ lexeme_type::bar })) {
                return 7;
            }
            else if (m_value.size() >= 2 && m_value.at(0) == '-' && m_type != lexeme_type::hex) {
                return static_cast<T>(std::stoll(m_value, nullptr, 10));
            }
            else if (m_value.size() >= 2 && m_value.at(0) == '-' && m_type == lexeme_type::hex) {
                return static_cast<T>(std::stoll(m_value, nullptr, 16));
            }
            else if (m_type == lexeme_type::hex) {
                return static_cast<T>(std::stoull(m_value, nullptr, 16));
            }
            else {
                return static_cast<T>(std::stoull(m_value, nullptr, 10));
            }
        }

        [[nodiscard]] auto left_associative() const -> bool
        {
            return is_one_of({
                lexeme_type::carat
            });
        }
    };
}

#endif
