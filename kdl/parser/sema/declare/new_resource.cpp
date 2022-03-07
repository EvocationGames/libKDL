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

#include <memory>
#include <kdl/parser/sema/declare/new_resource.hpp>
#include <kdl/parser/sema/declare/resource_values.hpp>
#include <kdl/schema/module.hpp>
#include <kdl/schema/resource/resource.hpp>
#include <kdl/schema/resource_type/resource_type.hpp>
#include <kdl/report/reporting.hpp>
#include <kdl/schema/resource_type/resource_field.hpp>
#include <iostream>

namespace kdl::lib::spec::keywords
{
    constexpr const char *new_ { "new" };
}

auto kdl::lib::sema::declare::new_resource::parse(kdl::lib::lexeme_consumer &consumer,
                                                  const std::shared_ptr<kdl::lib::module> &module,
                                                  const std::shared_ptr<kdl::lib::resource_type> &type) -> void
{
    consumer.assert_lexemes({
        expect(lexeme_type::identifier, spec::keywords::new_).t(),
        expect(lexeme_type::lparen).t()
    });

    int64_t id = INT64_MIN;
    std::string name = "";

    while (consumer.expect( expect(lexeme_type::rparen).f() )) {
        if (consumer.expect( expect(lexeme_type::resource_ref).t() )) {
            id = consumer.read().int64_value();
        }
        else if (consumer.expect( expect(lexeme_type::string).t() )) {
            name = consumer.read().string_value();
        }
        else {
            report::error(consumer.peek(), "Expected either a resource id, or a name.");
        }

        if (consumer.expect( expect(lexeme_type::comma).t() )) {
            consumer.advance();
            continue;
        }
        else if (consumer.expect( expect(lexeme_type::rparen).t() )) {
            consumer.advance();
            break;
        }
        else {
            report::error(consumer.peek(), "Expected either comma or rparen.");
        }
    }

    auto resource = std::make_shared<kdl::lib::resource>(type, id, name);

    // Check to see if the resource is being created from the contents of a file. If it
    // is, then completely disregard the parsing of this resource.
    // Only single field resources can be created this way.

    if (consumer.expect_all({
        expect(lexeme_type::equals).t(), expect(lexeme_type::identifier, "file").t(),
        expect(lexeme_type::string).t()
    })) {
        if (type->fields().size() == 1) {
            consumer.advance(2);
            auto path = consumer.read();

            // To resolve the path, we need to get the file that the path is contained with in, as it is
            // relative to it.
            auto absolute_path = path.file_reference().file().relative_path(path.string_value());
            auto file = source_file("", absolute_path);
            auto file_contents = lexeme(lexeme_type::string, file.source());
            auto field = type->fields().front();

            resource->set_value(file_contents, field->name());
            module->add_resource(resource);

            return;
        }
        else {
            report::error(consumer.peek(), "Declaring resources from a file reference can only be done on single field resources.");
        }
    }

    consumer.assert_lexemes({
        expect(lexeme_type::lbrace).t()
    });

    while (consumer.expect( expect(lexeme_type::rbrace).f() )) {
        if (!consumer.expect_all({
            expect(lexeme_type::identifier).t(),
            expect(lexeme_type::equals).t()
        })) {
            report::error(consumer.peek(), "Expected identifier for field name.");
        }

        auto field_name = consumer.read();
        consumer.advance();

        auto field = type->field_named(field_name.string_value());
        if (!field) {
            report::error(field_name, "Unrecognised field in resource type '" + type->name() + "'");
        }

        sema::declare::resource::values::parse(consumer, resource, field);

        consumer.assert_lexemes({
            expect(lexeme_type::semicolon).t()
        });
    }

    consumer.assert_lexemes({
        expect(lexeme_type::rbrace).t()
    });

    module->add_resource(resource);
}
