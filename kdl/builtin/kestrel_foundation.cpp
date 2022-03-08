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

#include <kdl/builtin/kestrel_foundation.hpp>
#include <kdl/builtin/posix_types.hpp>
#include <kdl/lexer/lexer.hpp>

// MARK: - Kestrel Types

static bool s_kestrel_imported = false;

static constexpr const char *kestrel_kdl = {R"(
@module KestrelBinaryTypes {
    define(*type Color32) {
		isa = color;
		size = 32;
	};

	define(*type PascalString) {
		isa = string;
		size = counted 255;
		char = macroman;
	};

	define(*type CString) {
		isa = string;
		size = null_terminated;
		char = macroman;
	};

	define(*type String<max>) {
		isa = string;
		size = null_terminated $max;
		char = macroman;
	};

	define(*type TypeName) {
		isa = string;
		size = fixed 4;
		char = macroman;
	};
};

@module KestrelResourceTypes {
    define (LuaScript : "LuaS") {
		template {
			CString Script;
		};

		@use_code_editor;

		field Script;
	};

	define (GLSL : "glsl") {
		template {
			CString Script;
		};

		@use_code_editor;

		field Script;
	};

	define (MetalShader : "mlsl") {
		template {
			CString Script;
		};

		@use_code_editor;

		field Script;
	};
};
)"};

// MARK: - Importer

auto kdl::lib::builtin::kestrel::import(lexeme_consumer &consumer) -> void
{
    builtin::posix::import(consumer);

    if (!s_kestrel_imported) {
        auto file = std::make_shared<source_file>(kestrel_kdl);
        lexer sub_lexer { file };
        consumer.insert(sub_lexer.scan(), 1);
    }

    s_kestrel_imported = true;
}