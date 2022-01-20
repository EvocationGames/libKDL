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

#include <kdl/parser/sema/function/function_call.hpp>


auto kdl::lib::sema::function::call::look_ahead(kdl::lib::lexeme_consumer &consumer) -> bool
{
    auto path_length = 0;

    // Peek forwards and attempt to build an identifier path that is followed by a '('
    // This is required for a function call.
    consumer.save_position();
    while (consumer.expect({ expect(lexeme_type::identifier).t() })) {
        consumer.advance();
        if (consumer.expect({ expect(lexeme_type::scope).t() })) {
            path_length++;
            consumer.advance();
            continue;
        }
        else if (consumer.expect({ expect(lexeme_type::lparen).t() })) {
            path_length++;
            break;
        }
        else {
            path_length = 0;
            break;
        }
    }
    consumer.restore_position();

    return path_length > 0;
}

auto kdl::lib::sema::function::call::parse(kdl::lib::lexeme_consumer &consumer,
                                           const std::shared_ptr<kdl::lib::module> &module) -> kdl::lib::lexeme
{
    return kdl::lib::lexeme(kdl::lib::lexeme_type::plus);
}
