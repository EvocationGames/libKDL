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

#include <kdl/parser/sema/directive/import.hpp>
#include <kdl/lexer/lexer.hpp>
#include <kdl/report/reporting.hpp>
#include <kdl/builtin/resedit_types.hpp>
#include <kdl/builtin/posix_types.hpp>
#include <kdl/builtin/kestrel_foundation.hpp>

auto kdl::lib::sema::directive::import::parse(lexeme_consumer &consumer) -> void
{
    consumer.assert_lexemes({ expect(lexeme_type::directive, "import").t() });

    if (consumer.expect( expect(lexeme_type::string).t() )) {
        auto relative_path = consumer.read();
        auto absolute_path = relative_path.file_reference().file().relative_path(relative_path.string_value());
        auto file = std::make_shared<source_file>("", absolute_path);

        lexer sub_lexer { file };
        consumer.insert(sub_lexer.scan(), 1);
    }
    else if (consumer.expect( expect(lexeme_type::identifier, "ResEdit").t() )) {
        consumer.advance();
        builtin::resedit::import(consumer);
    }
    else if (consumer.expect( expect(lexeme_type::identifier, "POSIX").t() )) {
        consumer.advance();
        builtin::posix::import(consumer);
    }
    else if (consumer.expect( expect(lexeme_type::identifier, "KestrelFoundation").t() )) {
        consumer.advance();
        builtin::kestrel::import(consumer);
    }
    else {
        report::error(consumer.peek(), "Unknown import type.");
    }
}

