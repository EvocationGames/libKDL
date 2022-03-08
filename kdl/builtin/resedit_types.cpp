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

#include <kdl/builtin/resedit_types.hpp>
#include <kdl/lexer/lexer.hpp>

// MARK: - ResEdit Types

static bool s_resedit_imported = false;

static constexpr const char *resedit_kdl = {R"(
@module ResEdit {
    define(*type DBYT) {
        isa = integer;
        size = 1;
        is_signed;
    };

    define(*type DWRD) {
        isa = integer;
        size = 2;
        is_signed;
    };

    define(*type DLNG) {
        isa = integer;
        size = 4;
        is_signed;
    };

    define(*type DQAD) {
        isa = integer;
        size = 8;
        is_signed;
    };

    define(*type HBYT) {
        isa = integer;
        size = 1;
    };

    define(*type HWRD) {
        isa = integer;
        size = 2;
    };

    define(*type HLNG) {
        isa = integer;
        size = 4;
    };

    define(*type HQAD) {
        isa = integer;
        size = 8;
    };

    define(*type PSTR) {
        isa = string;
        size = counted 255;
        char = macroman;
    };

    define(*type CSTR) {
        isa = string;
        size = null_terminated;
        char = macroman;
    };

    define(*type Cxxx<max>) {
        isa = string;
        size = null_terminated $max;
        char = macroman;
    };

    define(*type TNAM) {
        isa = string;
        size = fixed 4;
        char = macroman;
    };
};
)"};


// MARK: - Importer

auto kdl::lib::builtin::resedit::import(lexeme_consumer& consumer) -> void
{
    if (!s_resedit_imported) {
        auto file = std::make_shared<source_file>(resedit_kdl);
        lexer sub_lexer { file };
        consumer.insert(sub_lexer.scan(), 1);
    }
    s_resedit_imported = true;
}
