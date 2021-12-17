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

#include <stdexcept>
#include <string>
#include <kdl/parser/sema/define/binary_type.hpp>

auto kdl::lib::sema::define::binary_type::parse(lexeme_consumer &consumer, struct binary_type &type) -> void
{
    while (consumer.expect( expect(lexeme_type::rbrace).f() )) {

        if (consumer.expect_all({
            expect(lexeme_type::identifier, "isa").t(),
            expect(lexeme_type::equals).t(),
            expect(lexeme_type::identifier).t()
        })) {
            consumer.advance(2);
            std::string isa_type { consumer.read().string_value() };
            if (isa_type == "integer") {
                type.set_isa(binary_type_isa::integer);
            }
            else if (isa_type == "string") {
                type.set_isa(binary_type_isa::string);
            }
            else {
                throw std::runtime_error("Unrecognised binary type isa '" + isa_type + "'");
            }
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, "size").t(),
            expect(lexeme_type::equals).t(),
            expect(lexeme_type::integer).t()
        })) {
            consumer.advance(2);
            type.set_size(consumer.read().uint32_value());
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, "size").t(),
            expect(lexeme_type::equals).t(),
            expect(lexeme_type::identifier, "null_terminated").t()
        })) {
            consumer.advance(3);
            type.set_size(0);
            type.set_null_terminated(true);
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, "size").t(),
            expect(lexeme_type::equals).t(),
            expect(lexeme_type::identifier, "counted").t(),
            expect(lexeme_type::integer).t()
        })) {
            consumer.advance(3);
            type.set_count_width(consumer.read().uint32_value());
        }
        else if (consumer.expect_all({
            expect(lexeme_type::identifier, "char").t(),
            expect(lexeme_type::equals).t(),
            expect(lexeme_type::identifier).t()
        })) {
            consumer.advance(2);
            std::string enc_type { consumer.read().string_value() };
            if (enc_type == "ascii") {
                type.set_char_encoding(binary_type_char_encoding::ascii);
            }
            else if (enc_type == "macroman") {
                type.set_char_encoding(binary_type_char_encoding::macroman);
            }
            else if (enc_type == "utf8") {
                type.set_char_encoding(binary_type_char_encoding::utf8);
            }
            else {
                throw std::runtime_error("Unrecognised binary type character encoding type '" + enc_type + "'");
            }
        }
        else if (consumer.expect( expect(lexeme_type::identifier, "is_signed").t() )) {
            consumer.advance();
            type.set_signed(true);
        }

        consumer.assert_lexemes({ expect(lexeme_type::semicolon).t() });
    }
}