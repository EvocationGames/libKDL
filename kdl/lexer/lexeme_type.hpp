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

namespace kdl
{
    /* Denotes the purpose and intent of a lexical token extracted from a source file.
     */
    enum class lexeme_type : int
    {
        unknown = 0,
        integer, percentage, res_id, decimal,
        string, character,
        keyword, identifier, directive,
        lbrace, rbrace, lparen, rparen, langle, rangle,
        exclaim, dollar, carat, amp, star, plus, minus,
        equals, slash, bar, question, dot, comma, tilde, grave,
        colon, semicolon, hash
    };
}

#endif //LEXEME_TYPE_HPP
