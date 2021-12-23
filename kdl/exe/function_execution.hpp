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

#if !defined(KDL_EXE_FUNCTION_EXECUTION_HPP)
#define KDL_EXE_FUNCTION_EXECUTION_HPP

#include <memory>
#include <string>
#include <vector>
#include <kdl/lexer/lexeme.hpp>
#include <kdl/parser/consumer/consumer.hpp>
#include <kdl/exe/variable.hpp>


namespace kdl::lib
{
    struct function;
    struct function_argument;
}

namespace kdl::lib::exe
{

    struct function_execution
    {
    private:
        lexeme_consumer m_consumer;
        std::shared_ptr<struct function> m_function;
        std::vector<lexeme> m_args;
        std::vector<variable> m_vars;

        [[nodiscard]] auto resolve_variable_named(const lexeme& var) const -> lexeme;

    public:
        function_execution(const std::shared_ptr<struct function>& fn, const std::vector<lexeme>& args, const lexeme& call_site);

        [[nodiscard]] auto execute() -> lexeme;
    };

}

#endif //KDL_EXE_FUNCTION_EXECUTION_HPP
