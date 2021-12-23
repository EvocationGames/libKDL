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

#include <kdl/schema/function/function.hpp>
#include <kdl/parser/sema/define/function.hpp>
#include <kdl/schema/function/function_argument.hpp>
#include <kdl/schema/module.hpp>
#include <kdl/schema/binary_type/binary_type.hpp>
#include <kdl/report/reporting.hpp>

auto kdl::lib::sema::define::function::parse(kdl::lib::lexeme_consumer &consumer,
                                             const std::shared_ptr<kdl::lib::module> &module) -> std::shared_ptr<struct function>
{
    std::vector<std::string> ns;

    while (consumer.expect_all({ expect(lexeme_type::identifier).t(), expect(lexeme_type::scope).t() })) {
        ns.emplace_back(consumer.read().string_value());
        consumer.advance();
    }

    if (!consumer.expect_all({
        expect(lexeme_type::identifier).t(),
        expect(lexeme_type::dot).t(),
        expect(lexeme_type::identifier).t()
    })) {
        report::error(consumer.peek(), "Unexpected token sequence for function name.");
    }

    // Get the construction type for the function. This tells KDL what type the function is associated with, and
    // what type it is trying to create.
    auto construction_type_name = consumer.read();
    auto construction_type = module->binary_type_named(construction_type_name.string_value(), ns).lock();
    if (!construction_type) {
        report::error(construction_type_name, "Construction type of function is not recognised.");
    }
    if (construction_type->isa() != binary_type_isa::integer) {
       report::error(construction_type_name, "Functions are only supported on integer types.");
    }
    consumer.advance();

    // Get the function name.
    auto name = consumer.read();
    auto fn = std::make_shared<struct function>(name.string_value(), construction_type);
    construction_type->add_function(fn);

    // Parse out the arguments for the function, if there are any.
    if (consumer.expect( expect(lexeme_type::colon).t() )) {
        consumer.advance();

        while (consumer.expect( expect(lexeme_type::rparen).f() )) {
            ns.clear();
            while (consumer.expect_all({ expect(lexeme_type::identifier).t(), expect(lexeme_type::scope).t() })) {
                ns.emplace_back(consumer.read().string_value());
                consumer.advance();
            }

            if (consumer.expect_all({
                expect(lexeme_type::identifier).t(),
                expect(lexeme_type::identifier).t()
            })) {
                auto argument_type_name = consumer.read();
                auto argument_type = module->binary_type_named(argument_type_name.string_value(), ns).lock();
                if (!argument_type) {
                    report::error(argument_type_name, "Unrecognised type for argument.");
                }

                auto argument_name = consumer.read();
                fn->add_argument({ argument_type, argument_name.string_value() });
            }
            else {
                report::error(consumer.peek(), "Unexpected token sequence in function argument list.");
            }

            if (consumer.expect( expect(lexeme_type::comma).t() )) {
                consumer.advance();
                continue;
            }
            else if (consumer.expect( expect(lexeme_type::rparen).t() )) {
                break;
            }
            else {
                report::error(consumer.peek(), "Unexpected token in function argument list.");
            }
        }
    }

    return fn;
}
