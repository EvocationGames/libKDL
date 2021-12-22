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

#include <functional>
#include <kdl/parser/sema/define/define.hpp>
#include <kdl/parser/sema/define/binary_type.hpp>
#include <kdl/parser/sema/define/binary_template.hpp>
#include <kdl/parser/sema/define/resource_type.hpp>
#include <kdl/schema/module.hpp>
#include <kdl/schema/binary_type/binary_type.hpp>
#include <kdl/schema/binary_template/binary_template.hpp>
#include <kdl/schema/resource_type/resource_type.hpp>
#include <kdl/report/reporting.hpp>

namespace kdl::lib::spec::keywords
{
    constexpr const char *define { "define" };
    constexpr const char *type { "type" };
    constexpr const char *tmpl { "template" };
}

auto kdl::lib::sema::define::parse(lexeme_consumer &consumer, const std::shared_ptr<module> &module) -> void
{
    consumer.assert_lexemes({
        expect(lexeme_type::identifier, spec::keywords::define).t(),
        expect(lexeme_type::lparen).t()
    });

    std::function<auto()->void> continuation = [] {};

    if (consumer.expect( expect(lexeme_type::star).t() )) {
        consumer.advance();

        if (consumer.expect_all({
            expect(lexeme_type::identifier, spec::keywords::type).t(),
            expect(lexeme_type::identifier).t()
        })) {
            // Binary Type Definition
            consumer.advance();
            auto name = consumer.read();
            continuation = [&consumer, name, module] {
                auto type = std::make_shared<struct binary_type>(name.string_value());
                sema::define::binary_type::parse(consumer, type);
                module->add_binary_type_definition(type);
            };
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, spec::keywords::tmpl).t(),
            expect(lexeme_type::identifier).t()
        })) {
            // Binary Template Definition
            consumer.advance();
            auto name = consumer.read();
            continuation = [&consumer, name, module] {
                auto tmpl = std::make_shared<struct binary_template>(name.string_value());
                sema::define::binary_template::parse(consumer, tmpl, module);
                module->add_binary_template_definition(tmpl);
            };
        }
        else {
            report::error(consumer.peek(), "Unexpected definition type provided.");
        }
    }
    else if (consumer.expect_all({
        expect(lexeme_type::identifier).t(),
        expect(lexeme_type::colon).t(),
        expect(lexeme_type::string).t()
    })) {
        // Resource Type Definition
        auto name = consumer.read();
        consumer.advance();
        auto code = consumer.read();
        continuation = [&consumer, name, code, module] {
            auto type = std::make_shared<struct resource_type>(name.string_value(), code.string_value());
            sema::define::resource_type::parse(consumer, type, module);
            module->add_resource_type_definition(type);
        };
    }
    else {
        report::error(consumer.peek(), "Unexpected token in definition found.");
    }

    consumer.assert_lexemes({
        expect(lexeme_type::rparen).t(),
        expect(lexeme_type::lbrace).t()
    });

    continuation();

    consumer.assert_lexemes({
        expect(lexeme_type::rbrace).t(),
    });
}
