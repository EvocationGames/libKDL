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

#include <kdl/parser/consumer/consumer.hpp>
#include <kdl/report/reporting.hpp>

// MARK: - Construction

kdl::lib::lexeme_consumer::lexeme_consumer(std::vector<lexeme> lexemes)
    : m_lexemes(std::move(lexemes)), m_cursor(0)
{

}

// MARK: - Lexeme Management

auto kdl::lib::lexeme_consumer::finished(std::size_t count, std::int32_t offset) const -> bool
{
    auto cursor = m_cursor + offset;
    auto end = cursor + count;
    auto size = m_lexemes.size();
    return (cursor > size) || (end > size);
}

auto kdl::lib::lexeme_consumer::has_available(std::size_t count, std::int32_t offset) const -> bool
{
    return (m_cursor + offset + count) < m_lexemes.size();
}

auto kdl::lib::lexeme_consumer::at(std::size_t i) const -> lexeme
{
    return m_lexemes.at(i);
}

auto kdl::lib::lexeme_consumer::save_position() -> void
{
    m_position_stack.emplace_back(m_cursor);
}

auto kdl::lib::lexeme_consumer::restore_position() -> void
{
    if (m_position_stack.empty()) {
        throw std::runtime_error("Attempted to restore to an invalid position whilst parsing.");
    }
    m_cursor = m_position_stack.back();
    m_position_stack.pop_back();
}

auto kdl::lib::lexeme_consumer::advance(std::int32_t offset) -> void
{
    for (auto n = 0; !m_pushed_lexemes.empty() && n < offset; ++n) {
        m_pushed_lexemes.erase(m_pushed_lexemes.begin());
        offset--;
    }
    m_cursor += offset;
}

auto kdl::lib::lexeme_consumer::insert(std::vector<lexeme> lx, std::int32_t offset) -> void
{
    if (finished(1, offset)) {
        m_lexemes.insert(m_lexemes.end(), lx.begin(), lx.end());
    }
    else {
        auto ptr = m_lexemes.begin() + static_cast<decltype(offset)>(m_cursor) + offset;
        m_lexemes.insert(ptr, lx.begin(), lx.end());
    }
}

auto kdl::lib::lexeme_consumer::push_lexemes(std::initializer_list<lexeme> lexemes) -> void
{
    m_pushed_lexemes = std::vector<lexeme>(lexemes);
}

auto kdl::lib::lexeme_consumer::drop_lexemes() -> void
{
    m_pushed_lexemes.clear();
}

auto kdl::lib::lexeme_consumer::peek(std::int32_t offset) const -> lexeme
{
    if (!m_pushed_lexemes.empty() && (offset >= 0 || offset < m_pushed_lexemes.size())) {
        return m_pushed_lexemes.at(offset);
    }
    if (finished(1, offset)) {
        report::error(m_lexemes.at(m_lexemes.size() - 1), "Attempted to access lexeme beyond end of stream.");
    }
    return m_lexemes.at(m_cursor + offset);
}

auto kdl::lib::lexeme_consumer::read(std::int32_t offset) -> lexeme
{
    auto lx = peek(offset);
    if (m_pushed_lexemes.empty() || (offset >= m_pushed_lexemes.size())) {
        advance(offset + 1);
    }
    else {
        for (auto n = 0; n <= offset; ++n) {
            m_pushed_lexemes.erase(m_pushed_lexemes.begin());
        }
    }
    return lx;
}

auto kdl::lib::lexeme_consumer::consume(const expect::function& expectation) -> std::vector<lexeme>
{
    std::vector<lexeme> v;
    while (!finished() && expectation(peek())) {
        v.emplace_back(read());
    }
    return v;
}

// MARK: - Expectations

auto kdl::lib::lexeme_consumer::expect(const expect::function& expectation) -> bool
{
    return (m_previous_expect_result = expectation(peek()));
}

auto kdl::lib::lexeme_consumer::expect_any(std::initializer_list<expect::function> expectations) -> bool
{
    return expect_any(std::vector(expectations));
}

auto kdl::lib::lexeme_consumer::expect_any(std::vector<expect::function> expectations) -> bool
{
    auto lx = peek();
    return (m_previous_expect_result = std::any_of(expectations.begin(), expectations.end(), [expectations, lx] (const expect::function& expectation) {
        return expectation(lx);
    }));
}

auto kdl::lib::lexeme_consumer::expect_all(std::initializer_list<expect::function> expectations) -> bool
{
    return expect_all(std::vector(expectations));
}

auto kdl::lib::lexeme_consumer::expect_all(std::vector<expect::function> expectations) -> bool
{
    m_previous_expect_result = true;
    for (auto n = 0; n < expectations.size(); ++n) {
        if (!expectations.at(n)(peek(n))) {
            m_previous_expect_result = false;
            break;
        }
    }
    return m_previous_expect_result;
}

auto kdl::lib::lexeme_consumer::validate_expect() const -> bool
{
    return m_previous_expect_result;
}

auto kdl::lib::lexeme_consumer::assert_lexemes(std::initializer_list<expect::function> expectations) -> void
{
    if (!expect_all(std::vector(expectations))) {
        report::error(peek(), "Invalid sequence of lexemes encountered.");
    }
    advance(static_cast<std::int32_t>(expectations.size()));
}