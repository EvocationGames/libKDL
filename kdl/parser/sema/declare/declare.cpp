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

#include <kdl/parser/sema/declare/declare.hpp>
#include <kdl/schema/module.hpp>
#include <kdl/report/reporting.hpp>
#include <kdl/parser/sema/declare/new_resource.hpp>

namespace kdl::lib::spec::keywords
{
    constexpr const char *declare { "declare" };
    constexpr const char *new_ { "new" };
    constexpr const char *override { "override" };
    constexpr const char *duplicate { "duplicate" };
    constexpr const char *import { "import" };
}

auto kdl::lib::sema::declare::parse(kdl::lib::lexeme_consumer &consumer, const std::shared_ptr<kdl::lib::module> &module) -> void
{
    consumer.assert_lexemes({
        expect(lexeme_type::identifier, spec::keywords::declare).t()
    });

    if (!consumer.expect( expect(lexeme_type::identifier).t() )) {
        report::error(consumer.peek(), "Declaration requires an identifier for the resource type name.");
    }
    auto type_name = consumer.read();

    // Look up the resource type and validate it.
    auto type = module->resource_type_named(type_name.string_value()).lock();
    if (!type) {
        report::error(type_name, "Resource type '" + type_name.string_value() + "' is not recognised.");
    }

    consumer.assert_lexemes({
        expect(lexeme_type::lbrace).t()
    });

    while (consumer.expect( expect(lexeme_type::rbrace).f() )) {
        if (consumer.expect({ expect(lexeme_type::identifier, spec::keywords::new_).t() })) {
            sema::declare::new_resource::parse(consumer, module, type);
        }
        else if (consumer.expect({ expect(lexeme_type::identifier, spec::keywords::override).t() })) {

        }
        else if (consumer.expect({ expect(lexeme_type::identifier, spec::keywords::duplicate).t() })) {

        }
        else if (consumer.expect({ expect(lexeme_type::identifier, spec::keywords::import).t() })) {

        }
        else {
            report::error(consumer.peek(), "Unrecognised token encountered in resource declaration.");
        }

        consumer.assert_lexemes({ expect(lexeme_type::semicolon).t() });
    }

    consumer.assert_lexemes({
        expect(lexeme_type::rbrace).t()
    });
}
