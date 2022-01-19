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
#include <kdl/schema/project/scene.hpp>
#include <kdl/schema/module.hpp>
#include <kdl/report/reporting.hpp>

namespace kdl::lib::spec::keywords
{
    constexpr const char *scene { "scene" };
    constexpr const char *layer { "layer" };
}

auto kdl::lib::sema::project::scene::parse(lexeme_consumer &consumer, const std::shared_ptr<kdl::lib::module> &module) -> std::shared_ptr<class scene>
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
            expect(lexeme_type::identifier).t()
        })) {

            auto attribute = consumer.read();
            consumer.advance();
            auto function_name = consumer.read();
            if (auto fn = module->function_named(function_name.string_value(), "", {}).lock()){

            }
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier).t(), expect(lexeme_type::equals).t()
        })) {
            // Determine if this is a function
        }
        else {
            report::error(consumer.peek(), "Unexpected token in scene.");
        }
    }
}