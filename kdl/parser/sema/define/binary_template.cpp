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

#include <kdl/schema/module.hpp>
#include <kdl/parser/sema/define/binary_template.hpp>
#include <kdl/schema/binary_template/binary_template.hpp>
#include <kdl/report/reporting.hpp>

auto kdl::lib::sema::define::binary_template::parse(lexeme_consumer& consumer,
                                                    const std::shared_ptr<struct binary_template>& tmpl,
                                                    const std::shared_ptr<module>& module) -> void
{
    while (consumer.expect( expect(lexeme_type::rbrace).f() )) {
        std::vector<std::string> namespace_path;

        while (consumer.expect_all({
            expect(lexeme_type::identifier).t(),
            expect(lexeme_type::scope).t()
        })) {
            namespace_path.emplace_back(consumer.read().string_value());
            consumer.advance(1);
        }

        if (!consumer.expect( expect(lexeme_type::identifier).t() )) {
            report::error(consumer.peek(), "Expected identifier for binary field type.");
        }
        auto binary_type_name = consumer.read();
        auto binary_type = module->binary_type_named(binary_type_name.string_value(), namespace_path);
        if (binary_type.expired()) {
            report::error(binary_type_name, "Unknown binary type specified: '" + binary_type_name.string_value() + "'");
        }

        std::unordered_map<std::string, lexeme> args;
        if (consumer.expect( expect(lexeme_type::langle).t() )) {
            consumer.advance();

            // Make sure we have the correct number of attachments provided.
            const auto& attachments = binary_type.lock()->attachments();
            for (const auto& it : attachments) {
                if (!consumer.expect_any({
                    expect(lexeme_type::integer).t(),
                    expect(lexeme_type::percentage).t(),
                    expect(lexeme_type::resource_ref).t()
                })) {
                    report::error(consumer.peek(), "Expected numeric value for binary field type argument.");
                }

                auto arg = consumer.read();
                args.insert(std::pair(it.string_value(), arg));
            }

            consumer.assert_lexemes({ expect(lexeme_type::rangle).t() });
        }

        if (!consumer.expect_all({ expect(lexeme_type::identifier).t() })) {
            report::error(consumer.peek(), "Expected identifier for field name.");
        }

        auto field_name = consumer.read();
        tmpl->add_field(binary_type.lock(), args, field_name.string_value());

        consumer.assert_lexemes({ expect(lexeme_type::semicolon).t() });
    }
}
