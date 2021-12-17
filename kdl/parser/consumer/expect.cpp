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

#include "kdl/parser/consumer/expect.hpp"

// MARK: - Construction

kdl::lib::expect::expect(lexeme_type type)
    : m_type(type)
{

}

kdl::lib::expect::expect(std::string value)
    : m_value(std::move(value)), m_type(lexeme_type::unknown)
{

}

kdl::lib::expect::expect(lexeme_type type, std::string value)
    : m_value(std::move(value)), m_type(type)
{

}

// MARK: - Functions

auto kdl::lib::expect::to_be(bool match) const -> function
{
    return [&, match] (const lexeme& lx) -> bool {
        auto outcome = true;

        if (!this->m_value.empty() && !lx.is(this->m_value)) {
            outcome = false;
        }

        if (this->m_type != lexeme_type::unknown && !lx.is(this->m_type)) {
            outcome = false;
        }

        return (outcome == match);
    };
}

auto kdl::lib::expect::to_match() const -> function
{
    return to_be(true);
}

auto kdl::lib::expect::to_not_match() const -> function
{
    return to_be(false);
}

auto kdl::lib::expect::t() const -> function
{
    return to_be(true);
}

auto kdl::lib::expect::f() const -> function
{
    return to_be(false);
}