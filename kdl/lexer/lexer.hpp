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

#if !defined(KDL_LEXER_LEXER_HPP)
#define KDL_LEXER_LEXER_HPP

#include <vector>
#include <string>
#include <memory>
#include <kdl/file/source_file.hpp>
#include <kdl/file/file_reference.hpp>
#include <kdl/lexer/lexeme.hpp>

namespace kdl
{

    class lexer
    {
    private:
        std::vector<lexeme> m_lexemes;
        std::vector<std::size_t> m_cursor_stack;
        std::shared_ptr<source_file> m_source;
        std::string m_consume_slice;
        std::size_t m_cursor { 0 };
        std::size_t m_line { 1 };
        std::size_t m_line_offset { 0 };
        std::size_t m_marker { 0 };
        std::size_t m_expr_paren_balance { 0 };
        bool m_in_expr { false };

    public:
        explicit lexer(const std::shared_ptr<source_file>& source);

        auto reset() -> void;
        auto scan(bool omit_comments = true) -> std::vector<lexeme>;

        [[nodiscard]] auto eof() const -> bool;
        [[nodiscard]] auto source_position() const -> std::size_t;
        [[nodiscard]] auto line() const -> std::size_t;
        [[nodiscard]] auto offset() const -> std::size_t;
        [[nodiscard]] auto lexemes() const -> std::vector<lexeme>;

    private:
        [[nodiscard]] auto generate_file_reference() const -> file_reference;

        auto save_cursor() -> void;
        auto restore_cursor() -> void;
        auto detached_cursor(const std::function<auto(lexer&)->bool>& ctx) -> void;

        auto advance(std::int32_t offset = 1) -> void;
        [[nodiscard]] auto has_available(std::int32_t offset = 0, std::size_t count = 1) const -> bool;
        [[nodiscard]] auto peek(std::size_t count = 1, std::int32_t offset = 0) const -> std::string;
        auto read(std::size_t count = 1, std::int32_t offset = 0) -> std::string;

        auto test(const std::function<auto(const std::string&)->bool>& fn, std::int32_t offset = 0, std::size_t count = 1) const -> bool;
        auto test(const std::function<auto(std::size_t, const std::string&)->bool>& fn, std::int32_t offset = 0, std::size_t count = 1) const -> bool;
        auto consume(const std::function<auto(const std::string&)->bool>& fn, std::size_t count = 1) -> bool;
        auto consume(const std::function<auto(std::size_t, const std::string&)->bool>& fn, std::size_t count = 1) -> bool;

        auto mark_lexeme_start() -> void;
        [[nodiscard]] auto construct_lexeme(lexeme_type type) const -> lexeme;
        [[nodiscard]] auto construct_lexeme(lexeme_type type, const std::string& value) const -> lexeme;
        auto inject_lexeme(lexeme lx) -> void;
    };

}

#endif //KDL_LEXER_LEXER_HPP
