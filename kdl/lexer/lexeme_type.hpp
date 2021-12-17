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

#if !defined(LEXEME_TYPE_HPP)
#define LEXEME_TYPE_HPP

namespace kdl::lib
{
    /* Denotes the purpose and intent of a lexical token extracted from a source file.
     */
    enum class lexeme_type : int
    {
        unknown = 0,
        integer, percentage, resource_ref, namespace_ref, decimal, hex, lexpr, rexpr, var,
        string, character,
        keyword, identifier, directive,
        lbrace, rbrace, lparen, rparen, langle, rangle, lbracket, rbracket,
        exclaim, dollar, carat, amp, star, plus, minus,
        equals, slash, bar, question, dot, comma, tilde,
        colon, semicolon, hash, backslash, comment
    };

    static auto describe_lexeme_type(lexeme_type type) -> std::string
    {
        switch (type) {
        case lexeme_type::unknown: return "unknown";
        case lexeme_type::integer: return "integer";
        case lexeme_type::percentage: return "percentage";
        case lexeme_type::resource_ref: return "resource-reference";
        case lexeme_type::namespace_ref: return "namespace-reference";
        case lexeme_type::decimal: return "decimal";
        case lexeme_type::hex: return "hex";
        case lexeme_type::lexpr: return "start-expression";
        case lexeme_type::rexpr: return "end-expression";
        case lexeme_type::var: return "variable";
        case lexeme_type::string: return "string";
        case lexeme_type::character: return "character";
        case lexeme_type::keyword: return "keyword";
        case lexeme_type::identifier: return "identifier";
        case lexeme_type::directive: return "directive";
        case lexeme_type::lbrace: return "lbrace";
        case lexeme_type::rbrace: return "rbrace";
        case lexeme_type::lparen: return "lparen";
        case lexeme_type::rparen: return "rparen";
        case lexeme_type::langle: return "langle";
        case lexeme_type::rangle: return "rangle";
        case lexeme_type::lbracket: return "lbracket";
        case lexeme_type::rbracket: return "rbracket";
        case lexeme_type::exclaim: return "exclaim";
        case lexeme_type::dollar: return "dollar";
        case lexeme_type::carat: return "carat";
        case lexeme_type::amp: return "amp";
        case lexeme_type::star: return "star";
        case lexeme_type::plus: return "plus";
        case lexeme_type::minus: return "minus";
        case lexeme_type::equals: return "equals";
        case lexeme_type::slash: return "slash";
        case lexeme_type::bar: return "bar";
        case lexeme_type::question: return "question";
        case lexeme_type::dot: return "dot";
        case lexeme_type::comma: return "comma";
        case lexeme_type::tilde: return "tilde";
        case lexeme_type::colon: return "colon";
        case lexeme_type::semicolon: return "semicolon";
        case lexeme_type::hash: return "hash";
        case lexeme_type::backslash: return "backslash";
        case lexeme_type::comment: return "comment";
        }
    };
}

#endif //LEXEME_TYPE_HPP
