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

#include <kdl/parser/sema/project/scene.hpp>
#include <kdl/parser/sema/function/function_call.hpp>
#include <kdl/schema/project/scene.hpp>
#include <kdl/schema/module.hpp>
#include <kdl/report/reporting.hpp>

namespace kdl::lib::spec::keywords
{
    constexpr const char *scene { "scene" };
    constexpr const char *layer { "layer" };
}

auto kdl::lib::sema::project::scene::parse(lexeme_consumer &consumer, const std::shared_ptr<kdl::lib::module> &module) -> void
{
    if (!consumer.expect_all({
        expect(lexeme_type::identifier, spec::keywords::scene).t(),
        expect(lexeme_type::identifier).t(),
        expect(lexeme_type::lbrace).t()
    })) {
        report::error(consumer.peek(), "Expected scene structure");
    }

    consumer.advance();
    auto scene_name = consumer.read();
    consumer.advance();

    auto scene = std::make_shared<class kdl::lib::scene>(scene_name.string_value());

    while (consumer.expect( expect(lexeme_type::rbrace).f() )) {
        if (consumer.expect( expect(lexeme_type::identifier, spec::keywords::layer).t() )) {

        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier).t(), expect(lexeme_type::equals).t(),
            expect(lexeme_type::identifier).f()
        })) {
            auto attribute = consumer.read();
            consumer.advance();

            std::vector<lexeme> v;
            while (consumer.expect_any({
                expect(lexeme_type::identifier).t(), expect(lexeme_type::color).t(),
                expect(lexeme_type::any_integer).t(), expect(lexeme_type::any_string).t(),
                expect(lexeme_type::resource_ref).t(), expect(lexeme_type::percentage).t(),
                expect(lexeme_type::hex).t(), expect(lexeme_type::character).t(),
            })) {
                v.emplace_back(consumer.read());
            }

            scene->set_attribute(attribute.string_value(), v);
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier).t(), expect(lexeme_type::equals).t(),
            expect(lexeme_type::identifier).t()
        })) {
            auto attribute = consumer.read();
            consumer.advance();

            auto result = consumer.peek();
            if (sema::function::call::look_ahead(consumer)) {
                // We're looking at a function
                result = sema::function::call::parse(consumer, module);
            }
            else {
                consumer.advance();
            }

            scene->set_attribute(attribute.string_value(), result);
        }
        else {
            report::error(consumer.peek(), "Unexpected token in scene.");
        }

        consumer.assert_lexemes({ expect(lexeme_type::semicolon).t() });
    }

    consumer.assert_lexemes({ expect(lexeme_type::rbrace).t() });
    module->add_scene(scene);
}