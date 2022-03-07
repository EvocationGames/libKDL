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

#include <kdl/parser/sema/define/resource_type.hpp>
#include <kdl/schema/resource_type/resource_type.hpp>
#include <kdl/parser/sema/define/binary_template.hpp>
#include <kdl/parser/sema/define/resource_field.hpp>
#include <kdl/schema/binary_template/binary_template.hpp>
#include <kdl/schema/resource_type/resource_field.hpp>
#include <kdl/schema/resource_type/resource_field_value.hpp>
#include <kdl/schema/module.hpp>
#include <kdl/report/reporting.hpp>

auto kdl::lib::sema::define::resource_type::parse(kdl::lib::lexeme_consumer &consumer,
                                                  const std::shared_ptr<struct resource_type> &type,
                                                  const std::shared_ptr<module> &module) -> void
{
    while (consumer.expect( expect(lexeme_type::rbrace).f() )) {

        if (consumer.expect_all({
            expect(lexeme_type::identifier, "template").t(),
            expect(lexeme_type::lbrace).t()
        })) {
            // In-place template definition
            consumer.advance(2);

            auto generated_tmpl_name = type->name() + "_template";
            auto tmpl = std::make_shared<struct binary_template>(generated_tmpl_name);
            sema::define::binary_template::parse(consumer, tmpl, module);
            module->add_binary_template_definition(tmpl);
            type->set_binary_template(tmpl);

            consumer.assert_lexemes({ expect(lexeme_type::rbrace).t() });
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, "template").t(),
            expect(lexeme_type::equals).t()
        })) {
            // Existing named template definition
            consumer.advance(2);

            std::vector<std::string> namespace_path;
            while (consumer.expect_all({
                expect(lexeme_type::identifier).t(),
                expect(lexeme_type::scope).t()
            })) {
                namespace_path.emplace_back(consumer.read().string_value());
                consumer.advance(1);
            }

            if (consumer.expect( expect(lexeme_type::identifier).f() )) {
                report::warn(consumer.peek(), "Template name should be an identifier.");
            }
            auto name = consumer.read();
            auto tmpl = module->binary_template_named(name.string_value(), namespace_path);
            if (tmpl.expired()) {
                report::error(name, "Unknown binary template specified.");
            }

            type->set_binary_template(tmpl);
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, "assert").t(),
            expect(lexeme_type::lparen).t()
        })) {
            // Setup an assertion for resources of this type
        }
        else if (consumer.expect(
            expect(lexeme_type::directive, "use_code_editor").t()
        )) {
            // NOTE: This is a specific flag for shipyard, to state that the resource should be opened in a
            // code editor.
            type->set_uses_code_editor(true);
            consumer.advance();
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, "field").t(),
            expect(lexeme_type::identifier).t()
        })) {
            // Setup a field definition
            consumer.advance(1);
            auto name = consumer.read();
            auto field = std::make_shared<struct resource_field>(name.string_value());

            if (consumer.expect( expect(lexeme_type::lbrace).t() )) {
                consumer.assert_lexemes({ expect(lexeme_type::lbrace).t() });
                sema::define::resource_field::parse(consumer, type, field);
                consumer.assert_lexemes({ expect(lexeme_type::rbrace).t() });
            }
            else {
                // The field is implicit, as in the name of the field matches the name of the value
                // in the binary template.
                // TODO: Maybe include this in the field parser.
                if (auto tmpl = type->binary_template().lock()) {
                    if (auto bin_field = tmpl->field_named(name.string_value()).lock()) {
                        field->add_value(std::make_shared<struct resource_field_value>(bin_field));
                    }
                    else {
                        report::error(name, "Anonymous resource template for '" + type->name() + "' does not have a field named '" + name.string_value() + "'");
                    }
                }
                else {
                    report::error(name, "Resource type '" + type->name() + "' does not have a template");
                }
            }

            type->add_field(field);
        }

        consumer.assert_lexemes({ expect(lexeme_type::semicolon).t() });
    }
}
