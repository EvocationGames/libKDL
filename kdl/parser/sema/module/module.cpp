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

#include <kdl/parser/sema/module/module.hpp>
#include <kdl/schema/module.hpp>
#include <kdl/parser/sema/directive/author.hpp>
#include <kdl/parser/sema/directive/version.hpp>
#include <kdl/parser/sema/directive/copyright.hpp>
#include <kdl/parser/sema/directive/out.hpp>
#include <kdl/parser/sema/define/define.hpp>
#include <kdl/parser/sema/project/scene.hpp>
#include <kdl/schema/namespace.hpp>
#include <kdl/report/reporting.hpp>

namespace kdl::lib::spec::keywords
{
    constexpr const char *project { "project" };
    constexpr const char *module { "module" };
    constexpr const char *author { "author" };
    constexpr const char *version { "version" };
    constexpr const char *copyright { "copyright" };
    constexpr const char *out { "out" };
    constexpr const char *define { "define" };
    constexpr const char *declare { "declare" };
    constexpr const char *component { "component" };
    constexpr const char *name_space { "namespace" };
    constexpr const char *scene { "scene" };
}

auto kdl::lib::sema::module::parse(lexeme_consumer& consumer, const std::weak_ptr<name_space>& ns) -> std::shared_ptr<class module>
{
    if (!consumer.expect_all({
        expect(lexeme_type::directive).t(),
        expect(lexeme_type::string).t()
    })) {
        report::error(consumer.peek(), "Unexpected token sequence encountered.");
    }

    std::shared_ptr<class module> module;
    auto directive = consumer.read();
    if (directive.string_value() == spec::keywords::project) {
        module = std::make_shared<class module>(consumer.read().string_value(), module_type::project);
    }
    else if (directive.string_value() == spec::keywords::module) {
        module = std::make_shared<class module>(consumer.read().string_value(), module_type::module);
    }
    else {
        report::error(directive, "Unexpected directive.");
    }

    // Assign the namespace that we've been provided, until the user sets one up.
    module->set_namespace(ns);

    consumer.assert_lexemes({ expect(lexeme_type::lbrace).t() });
    while (consumer.expect({ expect(lexeme_type::rbrace).f() })) {

        // DIRECTIVES
        if (consumer.expect_all({
            expect(lexeme_type::directive, spec::keywords::name_space).t(),
            expect(lexeme_type::identifier).t()
        })) {
            consumer.advance();
            if (module->get_namespace().expired()) {
                report::error(consumer.peek(), "Module namespace is missing. This is a fatal error.");
            }
            auto parent = module->get_namespace().lock();
            module->set_namespace(parent->create(consumer.read().string_value()));
        }
        else if (consumer.expect({ expect(lexeme_type::directive, spec::keywords::author).t() })) {
            sema::directive::author::parse(consumer, module);
        }
        else if (consumer.expect({ expect(lexeme_type::directive, spec::keywords::version).t() })) {
            sema::directive::version::parse(consumer, module);
        }
        else if (consumer.expect({ expect(lexeme_type::directive, spec::keywords::copyright).t() })) {
            sema::directive::copyright::parse(consumer, module);
        }
        else if (consumer.expect({ expect(lexeme_type::directive, spec::keywords::out).t() })) {
            sema::directive::out::parse(consumer);
        }

        // FUNCTIONS
        else if (consumer.expect({ expect(lexeme_type::identifier, spec::keywords::define).t() })) {
            sema::define::parse(consumer, module);
        }
        else if (consumer.expect({ expect(lexeme_type::identifier, spec::keywords::declare).t() })) {

        }
        else if (consumer.expect({ expect(lexeme_type::identifier, spec::keywords::component).t() })) {

        }
        else if (consumer.expect({ expect(lexeme_type::identifier, spec::keywords::scene).t() })) {
            sema::project::scene::parse(consumer, module);
        }

        else {
            report::error(consumer.peek(), "Unexpected token encountered in module.");
        }

        consumer.assert_lexemes({ expect(lexeme_type::semicolon).t() });
    }
    consumer.assert_lexemes({ expect(lexeme_type::rbrace).t() });

    return module;
}