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

#include <stdexcept>
#include <functional>
#include <kdl/parser/sema/define/define.hpp>
#include <kdl/parser/sema/define/binary_type.hpp>
#include <kdl/schema/module.hpp>
#include <kdl/schema/binary_types/binary_type.hpp>

auto kdl::lib::sema::define::parse(lexeme_consumer &consumer, const std::shared_ptr<module> &project) -> void
{
    consumer.assert_lexemes({
        expect(lexeme_type::identifier, "define").t(),
        expect(lexeme_type::lparen).t()
    });

    std::function<auto()->void> continuation = [] {};

    if (consumer.expect( expect(lexeme_type::star).t() )) {
        consumer.advance();

        if (consumer.expect_all({ expect(lexeme_type::identifier, "type").t(), expect(lexeme_type::string).t() })) {
            // Binary Type Definition
            consumer.advance();
            std::string type_name { consumer.read().string_value() };
            struct binary_type type { type_name };

            continuation = [&consumer, &type, project] {
                sema::define::binary_type::parse(consumer, type);
                project->add_binary_type_definition(type);
            };
        }
        else {
            throw std::runtime_error("Unexpected definition type provided.");
        }
    }
    else {
        throw std::runtime_error("Unexpected definition type provided.");
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
