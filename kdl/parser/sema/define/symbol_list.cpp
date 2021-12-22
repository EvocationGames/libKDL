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

#include <iostream>
#include <kdl/parser/sema/define/symbol_list.hpp>
#include <kdl/schema/resource_type/resource_field_value.hpp>
#include <kdl/schema/resource_type/resource_field_symbol.hpp>
#include <kdl/report/reporting.hpp>

static bool comma_warning = false;

auto kdl::lib::sema::define::symbol_list::parse(kdl::lib::lexeme_consumer &consumer,
                                                const std::shared_ptr<struct resource_field_value> &value,
                                                const kdl::lib::lexeme_type &type) -> void
{
    while (consumer.expect( expect(lexeme_type::rbracket).f() )) {
        // TODO: Restructure this for better performance. The type check should be done externally and then execute
        // loops according to the type.
        if (!consumer.expect( expect(lexeme_type::identifier).t() )) {
            report::warn(consumer.peek(), "Symbol name should be an identifier, and can have unintended consequences if not.");
        }
        auto symbol_name = consumer.read();

        consumer.assert_lexemes({ expect(lexeme_type::equals).t() });

        switch (type) {
        case lexeme_type::integer: {
            if (!consumer.expect_any({
                expect(lexeme_type::integer).t(),
                expect(lexeme_type::resource_ref).t(),
                expect(lexeme_type::percentage).t()
            })) {
                report::warn(consumer.peek(), "Symbol value should be an integer type.");
            }
            auto symbol_value = consumer.read();
            value->add_symbol(std::make_shared<struct resource_field_symbol>(symbol_name.string_value(), symbol_value));
            break;
        }
        case lexeme_type::string: {
            if (!consumer.expect_any({
                expect(lexeme_type::string).t(),
                expect(lexeme_type::identifier).t()
            })) {
                report::warn(consumer.peek(), "Symbol value should be a string type.");
            }
            auto symbol_value = consumer.read();
            value->add_symbol(std::make_shared<struct resource_field_symbol>(symbol_name.string_value(), symbol_value));
            break;
        }
        default:
            report::error(consumer.peek(), "Unknown symbol type requested: " + describe_lexeme_type(type));
        }

        if (consumer.expect_all({ expect(lexeme_type::comma).f(), expect(lexeme_type::rbracket).f() }) && !comma_warning) {
            report::warn(consumer.peek(-1), "Symbols in value list should be seperated by a comma.");
            comma_warning = true;
        }
        else if (consumer.expect( expect(lexeme_type::comma).t() )) {
            consumer.advance();
        }
    }
}
