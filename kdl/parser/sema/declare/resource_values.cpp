// Copyright (c) 2022 Tom Hancocks
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

#include <kdl/parser/sema/declare/resource_values.hpp>
#include <kdl/schema/resource_type/resource_type.hpp>
#include <kdl/schema/resource_type/resource_field.hpp>
#include <kdl/schema/resource_type/resource_field_value.hpp>
#include <kdl/schema/resource_type/resource_field_symbol.hpp>
#include <kdl/schema/resource/resource.hpp>
#include <kdl/report/reporting.hpp>

auto kdl::lib::sema::declare::resource::values::parse(lexeme_consumer &consumer,
                                                      const std::shared_ptr<kdl::lib::resource>& resource,
                                                      const std::shared_ptr<kdl::lib::resource_field>& field) -> void
{
    for (const auto& expected_value : field->values()) {
        // First of all we need to determine if their is even a value? If there isn't then we _must_
        // have default values available for the expected value.
        if (consumer.expect( expect(lexeme_type::semicolon).t() )) {
            // We _require_ a default value here...
            if (!expected_value->default_value().has_value()) {
                report::error(consumer.peek(), "Unexpected end of field. Missing value for '" + expected_value->name() + "'");
            }

            // Apply the default value to the field, and then proceed to the next expected value.
            resource->set_value(expected_value->default_value().value(), expected_value->name());
            continue;
        }

        // We are looking at a potential value. The first task is to check if this is an identifier representing
        // either an associated symbol, a function or an expression.
        // TODO: ^^^

        switch (expected_value->expected_value_lexeme_type()) {
            case kdl::lib::lexeme_type::integer: {
                if (!consumer.expect_any({
                    expect(lexeme_type::integer).t(), expect(lexeme_type::hex).t(),
                    expect(lexeme_type::percentage).t(), expect(lexeme_type::resource_ref).t()
                })) {
                    report::error(consumer.peek(), "Expected an integer value.");
                }
                break;
            }
            case kdl::lib::lexeme_type::color: {
                if (!consumer.expect_any({
                    expect(lexeme_type::color).t(), expect(lexeme_type::hex).t()
                })) {
                    report::error(consumer.peek(), "Expected a color representation, via an integer value.");
                }
                break;
            }
            case kdl::lib::lexeme_type::string: {
                if (!consumer.expect_any({
                    expect(lexeme_type::string).t()
                })) {
                    report::error(consumer.peek(), "Expected a string value.");
                }
                break;
            }
        }

        // The value has been validated, so assign it.
        resource->set_value(consumer.read(), expected_value->name());
    }
}