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

#include <string>
#include <kdl/parser/sema/define/binary_type.hpp>
#include <kdl/schema/binary_type/binary_type.hpp>
#include <kdl/report/reporting.hpp>

namespace kdl::lib::spec::keyword
{
    constexpr const char *isa { "isa" };
    constexpr const char *integer { "integer" };
    constexpr const char *string { "string" };
    constexpr const char *color { "color" };
    constexpr const char *size { "size" };
    constexpr const char *null_terminated { "null_terminated" };
    constexpr const char *counted { "counted" };
    constexpr const char *chr { "char" };
    constexpr const char *is_signed { "is_signed" };
}

namespace kdl::lib::sema::define::binary_type::encoding
{
    constexpr const char *ascii { "ascii" };
    constexpr const char *macroman { "macroman" };
    constexpr const char *utf8 { "utf8" };
}

auto kdl::lib::sema::define::binary_type::parse(lexeme_consumer &consumer, const std::shared_ptr<struct binary_type>& type) -> void
{
    auto name = consumer.peek(-1);
    while (consumer.expect( expect(lexeme_type::rbrace).f() )) {

        if (consumer.expect_all({
            expect(lexeme_type::identifier, spec::keyword::isa).t(),
            expect(lexeme_type::equals).t(),
            expect(lexeme_type::identifier).t()
        })) {
            consumer.advance(2);
            auto isa_type = consumer.read();
            if (isa_type.string_value() == spec::keyword::integer) {
                type->set_isa(binary_type_isa::integer);
            }
            else if (isa_type.string_value() == spec::keyword::string) {
                type->set_isa(binary_type_isa::string);
            }
            else if (isa_type.string_value() == spec::keyword::color) {
                type->set_isa(binary_type_isa::color);
            }
            else {
                report::error(isa_type, "Unrecognised binary type isa '" + isa_type.string_value() + "'");
            }
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, spec::keyword::size).t(),
            expect(lexeme_type::equals).t(),
            expect(lexeme_type::integer).t()
        })) {
            consumer.advance(2);
            type->set_size(consumer.read().uint32_value());
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, spec::keyword::size).t(),
            expect(lexeme_type::equals).t(),
            expect(lexeme_type::identifier, spec::keyword::null_terminated).t()
        })) {
            consumer.advance(3);
            type->set_size(0);
            type->set_null_terminated(true);
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, spec::keyword::size).t(),
            expect(lexeme_type::equals).t(),
            expect(lexeme_type::identifier, spec::keyword::counted).t(),
            expect(lexeme_type::integer).t()
        })) {
            consumer.advance(3);
            type->set_count_width(consumer.read().uint32_value());
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, spec::keyword::chr).t(),
            expect(lexeme_type::equals).t(),
            expect(lexeme_type::identifier).t()
        })) {
            consumer.advance(2);
            auto enc_type = consumer.read();
            if (enc_type.string_value() == encoding::ascii) {
                type->set_char_encoding(binary_type_char_encoding::ascii);
            }
            else if (enc_type.string_value() == encoding::macroman) {
                type->set_char_encoding(binary_type_char_encoding::macroman);
            }
            else if (enc_type.string_value() == encoding::utf8) {
                type->set_char_encoding(binary_type_char_encoding::utf8);
            }
            else {
                report::warn(enc_type, "Unrecognised binary type character encoding type. Using macroman.");
                type->set_char_encoding(binary_type_char_encoding::macroman);
            }
        }
        else if (consumer.expect( expect(lexeme_type::identifier, spec::keyword::is_signed).t() )) {
            consumer.advance();
            type->set_signed(true);
        }

        consumer.assert_lexemes({ expect(lexeme_type::semicolon).t() });
    }

    if (type->isa() == binary_type_isa::color && type->size() != 32) {
        report::warn(name, "Color binary type should be unsigned and have a size of 32.");
    }
}