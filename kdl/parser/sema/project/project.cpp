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

#include <kdl/parser/sema/project/project.hpp>
#include <kdl/schema/module.hpp>
#include <kdl/parser/sema/directive/author.hpp>
#include <kdl/parser/sema/directive/version.hpp>
#include <kdl/parser/sema/directive/copyright.hpp>
#include <kdl/parser/sema/directive/out.hpp>

auto kdl::lib::sema::project::parse(lexeme_consumer& consumer) -> std::shared_ptr<module>
{
    consumer.assert_lexemes({ expect(lexeme_type::directive, "project").t() });

    if (!consumer.expect( expect(lexeme_type::string).t() )) {
        throw std::runtime_error("Expected project name to be a string.");
    }
    std::string project_name { consumer.read().string_value() };
    auto project = std::make_shared<module>(project_name, module_type::project);

    consumer.assert_lexemes({ expect(lexeme_type::lbrace).t() });
    while (consumer.expect({ expect(lexeme_type::rbrace).f() })) {

        if (consumer.expect({ expect(lexeme_type::directive, "author").t() })) {
            sema::directive::author::parse(consumer, project);
        }
        else if (consumer.expect({ expect(lexeme_type::directive, "version").t() })) {
            sema::directive::version::parse(consumer, project);
        }
        else if (consumer.expect({ expect(lexeme_type::directive, "copyright").t() })) {
            sema::directive::copyright::parse(consumer, project);
        }
        else if (consumer.expect({ expect(lexeme_type::directive, "out").t() })) {
            sema::directive::copyright::parse(consumer, project);
        }
        else {
            throw std::runtime_error("Unexpected token encountered in project.");
        }

        consumer.assert_lexemes({ expect(lexeme_type::semicolon).t() });
    }
    consumer.assert_lexemes({ expect(lexeme_type::rbrace).t() });

    return project;
}