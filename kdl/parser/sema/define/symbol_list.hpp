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

#if !defined(KDL_PARSER_SEMA_DEFINE_SYMBOL_LIST_HPP)
#define KDL_PARSER_SEMA_DEFINE_SYMBOL_LIST_HPP

#include <memory>
#include <kdl/parser/consumer/consumer.hpp>
#include <kdl/lexer/lexeme_type.hpp>

namespace kdl::lib
{
    class resource_field_value;
}

namespace kdl::lib::sema::define::symbol_list
{
    auto parse(lexeme_consumer& consumer, const std::shared_ptr<struct resource_field_value>& value, const lexeme_type& type) -> void;
}

#endif //KDL_PARSER_SEMA_DEFINE_SYMBOL_LIST_HPP
