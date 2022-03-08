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

#include <kdl/builtin/posix_types.hpp>
#include <kdl/lexer/lexer.hpp>

// MARK: - Posix Types

static bool s_posix_imported = false;

static constexpr const char *posix_kdl = {R"(
@module POSIX {
    define(*type UInt8) {
		isa = integer;
		size = 8;
	};

	define(*type Int8) {
		isa = integer;
		size = 8;
		is_signed;
	};

	define(*type UInt16) {
		isa = integer;
		size = 16;
	};

	define(*type Int16) {
		isa = integer;
		size = 16;
		is_signed;
	};

	define(*type UInt32) {
		isa = integer;
		size = 32;
	};

	define(*type Int32) {
		isa = integer;
		size = 32;
		is_signed;
	};

	define(*type UInt64) {
		isa = integer;
		size = 64;
	};

	define(*type Int64) {
		isa = integer;
		size = 64;
		is_signed;
	};
};
)"};

// MARK: - Importer

auto kdl::lib::builtin::posix::import(lexeme_consumer &consumer) -> void
{
    if (!s_posix_imported) {
        auto file = std::make_shared<source_file>(posix_kdl);
        lexer sub_lexer { file };
        consumer.insert(sub_lexer.scan(), 1);
    }
    s_posix_imported = true;
}