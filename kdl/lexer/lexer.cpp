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

#include <kdl/lexer/lexer.hpp>
#include <kdl/lexer/lexical_rules.hpp>
#include <iostream>

// MARK: - Construction

kdl::lexer::lexer(const std::shared_ptr<source_file>& source)
    : m_source(source)
{

}

// MARK: - High Level Scanning

auto kdl::lexer::reset() -> void
{
    m_lexemes.clear();
    m_cursor_stack.clear();
    m_cursor = 0;
    m_line = 1;
    m_line_offset = 0;
}

auto kdl::lexer::scan(bool omit_comments) -> std::vector<lexeme>
{
    reset();

    // Open up a new loop and keep iterating until we have no more characters to consume.
    while (has_available()) {
        consume(lexical_rule::whitespace::contains);
        mark_lexeme_start();

        // Check for a newline.
        if (test(lexical_rule::unix_nl::yes)) {
            advance();
            m_line++;
            m_line_offset = 0;
            continue;
        }
        else if (test(lexical_rule::cr::yes)) {
            advance();
            continue;
        }

        // Check for a comment. Comments consume the remainder of the line that we are on.
        if (test(lexical_rule::match<'`'>::yes)) {
            advance();
            consume(lexical_rule::unix_nl::no);
            if (!omit_comments) {
                inject_lexeme(construct_lexeme(lexeme_type::comment));
            }
            continue;
        }

        // Directives
        else if (test(lexical_rule::match<'@'>::yes)) {
            advance();
            consume(lexical_rule::identifier::matches);
            inject_lexeme(construct_lexeme(lexeme_type::directive));
        }

        // Literals
        else if (test(lexical_rule::match<'"'>::yes)) {
            advance();
            consume(lexical_rule::match<'"'>::no);
            inject_lexeme(construct_lexeme(lexeme_type::string));
            advance();
        }
        else if (test(lexical_rule::match<'$'>::yes) && !m_in_expr) {
            advance();
            if (test(lexical_rule::match<'('>::yes)) {
                m_in_expr = true;
                inject_lexeme(construct_lexeme(lexeme_type::lexpr, read()));
            }
            else {
                consume(lexical_rule::identifier::matches);
                inject_lexeme(construct_lexeme(lexeme_type::var));
            }
        }
        else if (test(lexical_rule::match<')'>::yes) && m_in_expr && m_expr_paren_balance == 0) {
            m_in_expr = false;
            inject_lexeme(construct_lexeme(lexeme_type::rexpr, read()));
        }
        else if (test(lexical_rule::match<'#'>::yes)) {
            advance();

            // Types of Resource Reference:
            //  #128
            //  #'namespace'128
            if (test(lexical_rule::match<'\''>::yes)) {
                advance();
                consume(lexical_rule::match<'\''>::no);
                inject_lexeme(construct_lexeme(lexeme_type::namespace_ref));
                advance();
                mark_lexeme_start();
            }

            auto negative = test(lexical_rule::match<'-'>::yes);
            if (negative) {
                advance();
            }

            consume(lexical_rule::numeric::decimal::contains);
            if (negative) {
                m_consume_slice.insert(0, 1, '-');
            }
            inject_lexeme(construct_lexeme(lexeme_type::resource_ref));
        }
        else if (test(lexical_rule::numeric::decimal::contains)) {
            lexeme_type number_type = lexeme_type::decimal;
            if (peek(2) == "0x" || peek(2) == "0X") {
                // Hexadecimal
                advance(2);
                consume(lexical_rule::numeric::hexadecimal::contains);
                number_type = lexeme_type::hex;
            }
            else {
                auto negative = test(lexical_rule::match<'-'>::yes);
                if (negative) {
                    advance();
                }

                consume(lexical_rule::numeric::decimal::contains);
                if (negative) {
                    m_consume_slice.insert(0, 1, '-');
                }

                if (test(lexical_rule::match<'%'>::yes)) {
                    number_type = lexeme_type::percentage;
                    advance();
                }
            }

            inject_lexeme(construct_lexeme(number_type));
        }
        else if (test(lexical_rule::identifier::limited_set::contains)) {
            consume(lexical_rule::identifier::set::contains);
            inject_lexeme(construct_lexeme(lexeme_type::identifier));
        }

        // Operators
        else if (test(lexical_rule::match<';'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::semicolon, read()));
        }
        else if (test(lexical_rule::match<':'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::colon, read()));
        }
        else if (test(lexical_rule::match<'*'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::star, read()));
        }
        else if (test(lexical_rule::match<'+'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::plus, read()));
        }
        else if (test(lexical_rule::match<'-'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::minus, read()));
        }
        else if (test(lexical_rule::match<'='>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::equals, read()));
        }
        else if (test(lexical_rule::match<'!'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::exclaim, read()));
        }
        else if (test(lexical_rule::match<'?'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::question, read()));
        }
        else if (test(lexical_rule::match<'^'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::carat, read()));
        }
        else if (test(lexical_rule::match<'&'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::amp, read()));
        }
        else if (test(lexical_rule::match<'('>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::lparen, read()));
            if (m_in_expr) {
                m_expr_paren_balance++;
            }
        }
        else if (test(lexical_rule::match<')'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::rparen, read()));
            if (m_in_expr) {
                m_expr_paren_balance--;
            }
        }
        else if (test(lexical_rule::match<'['>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::lbracket, read()));
        }
        else if (test(lexical_rule::match<']'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::rbracket, read()));
        }
        else if (test(lexical_rule::match<'<'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::langle, read()));
        }
        else if (test(lexical_rule::match<'>'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::rangle, read()));
        }
        else if (test(lexical_rule::match<'{'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::lbrace, read()));
        }
        else if (test(lexical_rule::match<'}'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::rbrace, read()));
        }
        else if (test(lexical_rule::match<'.'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::dot, read()));
        }
        else if (test(lexical_rule::match<','>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::comma, read()));
        }
        else if (test(lexical_rule::match<'|'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::bar, read()));
        }
        else if (test(lexical_rule::match<'\\'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::backslash, read()));
        }
        else if (test(lexical_rule::match<'~'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::tilde, read()));
        }
        else if (test(lexical_rule::match<'/'>::yes)) {
            inject_lexeme(construct_lexeme(lexeme_type::slash, read()));
        }

        else {
            throw std::logic_error("Unexpected character encountered: '" + peek() + "'");
        }
    }

    return m_lexemes;
}

// MARK: - Basic Accessors
auto kdl::lexer::eof() const -> bool
{
    return !has_available();
}

auto kdl::lexer::source_position() const -> std::size_t
{
    return m_cursor;
}

auto kdl::lexer::line() const -> std::size_t
{
    return m_line;
}

auto kdl::lexer::offset() const -> std::size_t
{
    return m_line_offset;
}

auto kdl::lexer::lexemes() const -> std::vector<lexeme>
{
    return m_lexemes;
}

// MARK: - Cursor Functions

auto kdl::lexer::save_cursor() -> void
{
    m_cursor_stack.emplace_back(m_cursor);
}

auto kdl::lexer::restore_cursor() -> void
{
    if (m_cursor_stack.empty()) {
        throw std::logic_error("Lexer attempted to restore to a position that did not exist.");
    }
    m_cursor = m_cursor_stack.back();
    m_cursor_stack.pop_back();
}

auto kdl::lexer::detached_cursor(const std::function<auto(lexer&)->bool>& ctx) -> void
{
    save_cursor();
    if (ctx(*this)) {
        restore_cursor();
    }
}

auto kdl::lexer::advance(std::int32_t offset) -> void
{
    m_cursor += offset;
    m_line_offset += offset;
}

auto kdl::lexer::has_available(std::int32_t offset, std::size_t count) const -> bool
{
    return (m_cursor + offset + count) <= (m_source->size());
}

auto kdl::lexer::peek(std::size_t count, std::int32_t offset) const -> std::string
{
    if (!has_available(offset, count)) {
        throw std::logic_error("Failed to read string from source.");
    }
    return m_source->source().substr(m_cursor + offset, count);
}

auto kdl::lexer::read(std::size_t count, std::int32_t offset) -> std::string
{
    auto str = peek(count, offset);
    advance(static_cast<std::int32_t>(offset + count));
    return str;
}

// MARK: - Test Function

auto kdl::lexer::test(const std::function<auto(const std::string&)->bool>& fn, std::int32_t offset, std::size_t count) const -> bool
{
    return fn(peek(count, offset));
}

auto kdl::lexer::test(const std::function<auto(std::size_t, const std::string&)->bool>& fn, std::int32_t offset, std::size_t count) const -> bool
{
    return fn(0, peek(count, offset));
}

auto kdl::lexer::consume(const std::function<auto(const std::string&)->bool>& fn, std::size_t count) -> bool
{
    m_consume_slice.clear();
    while (fn(peek(count))) {
        m_consume_slice += read(count);
    }
    return !m_consume_slice.empty();
}

auto kdl::lexer::consume(const std::function<auto(std::size_t, const std::string&)->bool>& fn, std::size_t count) -> bool
{
    std::size_t pos = 0;
    m_consume_slice.clear();
    while (fn(pos, peek(count))) {
        pos += count;
        m_consume_slice += read(count);
    }
    return !m_consume_slice.empty();
}

// MARK: - File References

auto kdl::lexer::generate_file_reference() const -> file_reference
{
    return { m_source, m_cursor, m_line, m_marker, m_line_offset - m_marker };
}

// MARK: - Lexeme Construction

auto kdl::lexer::mark_lexeme_start() -> void
{
    m_marker = m_line_offset;
}

auto kdl::lexer::construct_lexeme(lexeme_type type) const -> lexeme
{
    return { type, generate_file_reference(), m_consume_slice };
}

auto kdl::lexer::construct_lexeme(kdl::lexeme_type type, const std::string& value) const -> kdl::lexeme
{
    return { type, generate_file_reference(), value };
}

auto kdl::lexer::inject_lexeme(lexeme lx) -> void
{
    m_lexemes.emplace_back(std::move(lx));
}
