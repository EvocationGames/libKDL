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

#include <utility>
#include <kdl/parser/parser.hpp>
#include <kdl/lexer/lexer.hpp>
#include <kdl/parser/sema/directive/out.hpp>
#include <kdl/parser/sema/module/module.hpp>
#include <kdl/schema/namespace.hpp>

namespace kdl::lib::spec::keyword
{
    constexpr const char *project { "project" };
    constexpr const char *module { "module" };
    constexpr const char *out { "out" };
};

// MARK: - Top Level Parser

auto kdl::lib::parser::parse(const std::shared_ptr<source_file> &source) -> void
{
    parse(lexer(source).scan());
}

auto kdl::lib::parser::parse(std::vector<lexeme> lexemes) -> void
{
    m_global_namespace = std::make_shared<name_space>();
    m_consumer = lexeme_consumer(std::move(lexemes));

    while (!m_consumer.finished()) {

        if (m_consumer.expect_any({
            expect(lexeme_type::directive, spec::keyword::project).t(),
            expect(lexeme_type::directive, spec::keyword::module).t()
        })) {
            auto module = sema::module::parse(m_consumer, m_global_namespace);
            m_modules.emplace_back(module);
        }
        else if (m_consumer.expect( expect(lexeme_type::directive, spec::keyword::out).t() )) {
            sema::directive::out::parse(m_consumer);
        }
        else {
            throw std::runtime_error("Unexpected lexeme encountered.");
        }

        m_consumer.assert_lexemes({ expect(lexeme_type::semicolon).t() });
    }
}


