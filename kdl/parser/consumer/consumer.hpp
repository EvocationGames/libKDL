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

#if !defined(KDL_PARSER_CONSUMER_HPP)
#define KDL_PARSER_CONSUMER_HPP

#include <vector>
#include <initializer_list>
#include <kdl/lexer/lexeme.hpp>
#include <kdl/parser/consumer/expect.hpp>

namespace kdl::lib
{

    class lexeme_consumer
    {
    private:
        std::vector<std::size_t> m_position_stack;
        std::vector<lexeme> m_lexemes;
        std::vector<lexeme> m_pushed_lexemes;
        std::size_t m_cursor { 0 };
        bool m_previous_expect_result { false };

    public:
        explicit lexeme_consumer(std::vector<lexeme> lexemes);

        [[nodiscard]] auto finished(std::size_t count = 1, std::int32_t offset = 0) const -> bool;
        [[nodiscard]] auto has_available(std::size_t count = 1, std::int32_t offset = 0) const -> bool;
        [[nodiscard]] auto at(std::size_t i) const -> lexeme;

        auto save_position() -> void;
        auto restore_position() -> void;

        auto advance(std::int32_t offset = 1) -> void;

        auto insert(std::vector<lexeme> lx, std::int32_t offset = 0) -> void;
        auto push_lexemes(std::initializer_list<lexeme> lexemes) -> void;
        auto drop_lexemes() -> void;

        [[nodiscard]] auto peek(std::int32_t offset = 0) const -> lexeme;
        auto read(std::int32_t offset = 0) -> lexeme;
        auto consume(const expect::function& expectation) -> std::vector<lexeme>;

        auto expect(const expect::function& expectation) -> bool;
        auto expect_any(std::initializer_list<expect::function> expectations) -> bool;
        auto expect_any(std::vector<expect::function> expectations) -> bool;
        auto expect_all(std::initializer_list<expect::function> expectations) -> bool;
        auto expect_all(std::vector<expect::function> expectations) -> bool;
        [[nodiscard]] auto validate_expect() const -> bool;

        auto assert_lexemes(std::initializer_list<expect::function> expectations) -> void;

    };

}

#endif //KDL_PARSER_CONSUMER_HPP
