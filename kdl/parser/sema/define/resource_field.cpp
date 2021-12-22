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

#include <kdl/parser/sema/define/resource_field.hpp>
#include <kdl/parser/sema/define/symbol_list.hpp>
#include <kdl/schema/resource_type/resource_field_value.hpp>
#include <kdl/schema/resource_type/resource_field.hpp>
#include <kdl/schema/resource_type/resource_type.hpp>
#include <kdl/schema/resource_type/resource_field_symbol.hpp>
#include <kdl/schema/binary_template/binary_template.hpp>
#include <kdl/schema/binary_template/binary_template_field.hpp>
#include <kdl/report/reporting.hpp>
#include <optional>

auto kdl::lib::sema::define::resource_field::parse(kdl::lib::lexeme_consumer &consumer,
                                                   const std::shared_ptr<struct resource_type> &type,
                                                   const std::shared_ptr<struct resource_field> &field) -> void
{
    auto tmpl = type->binary_template().lock();
    if (!tmpl) {
        report::error(consumer.peek(-1), "Resource type '" + type->name() + "' does not have a template");
    }

    while (consumer.expect( expect(lexeme_type::rbrace).f() )) {
        auto binary_field_name = consumer.read();

        auto bin_field = tmpl->field_named(binary_field_name.string_value()).lock();
        if (!bin_field) {
            report::error(binary_field_name, "Resource template '" + tmpl->name() + "' does not have a field named '" + binary_field_name.string_value() + "'");
        }

        // Start construction the field value, and determine what type of lexeme we should expect for default values
        // and symbols.
        auto field_value = std::make_shared<struct resource_field_value>(bin_field);
        field->add_value(field_value);

        // Check for a potential default value and read it. We'll check the value once we have the symbol list, as
        // we may need to resolve it.
        std::optional<lexeme> default_value = {};
        if (consumer.expect( expect(lexeme_type::equals).t() )) {
            consumer.advance();

            if (!consumer.expect_any({
                expect(lexeme_type::string).t(),
                expect(lexeme_type::identifier).t(),
                expect(lexeme_type::integer).t(),
                expect(lexeme_type::percentage).t(),
                expect(lexeme_type::resource_ref).t()
            })) {
                report::error(consumer.peek(), "Illegal token provided as default value.");
            }

            default_value = consumer.read();
        }

        // Check if the field value has any pre-defined constants/values/symbols.
        if (consumer.expect( expect(lexeme_type::lbracket).t() )) {
            consumer.advance();
            sema::define::symbol_list::parse(consumer, field_value, field_value->expected_value_lexeme_type());
            consumer.assert_lexemes({ expect(lexeme_type::rbracket).t() });
        }

        // Now resolve the default value if it exists.
        if (default_value.has_value()) {
            // If the value is an identifier, we're going to try and resolve it in the symbol list.
            if (default_value->type() == lexeme_type::identifier) {
                auto value = field_value->symbol_named(default_value.value().string_value()).lock();
                if (!value) {
                    report::warn(default_value.value(), "Symbol not recognised and will be treated as a string.");
                }
                else {
                    default_value = { value->value() };
                }

            }

            // Now make sure the resolved value actually has the correct type.
            auto value_type = default_value->type();
            auto expected_type = field_value->expected_value_lexeme_type();
            switch (expected_type) {
            case lexeme_type::string:
                if (value_type != lexeme_type::string && value_type != lexeme_type::identifier) {
                    report::error(default_value.value(), "Value was expected to be a string.");
                }
                break;

            case lexeme_type::integer:
                if (value_type != lexeme_type::integer && value_type != lexeme_type::percentage) {
                    report::error(default_value.value(), "Value was expected to be an integer.");
                }
                break;

            default:
                report::error(default_value.value(), "Unrecognised value type.");
            }

            field_value->set_default_value(default_value.value());
        }

        consumer.assert_lexemes({ expect(lexeme_type::semicolon).t() });
    }
}
