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

#include <kdl/exe/function_execution.hpp>
#include <kdl/schema/function/function_argument.hpp>
#include <kdl/schema/function/function.hpp>
#include <kdl/schema/binary_type/binary_type.hpp>
#include <kdl/report/reporting.hpp>

// MARK: - Construction

kdl::lib::exe::function_execution::function_execution(const std::shared_ptr<struct function> &fn,
                                                      const std::vector<lexeme>& args,
                                                      const lexeme& call_site)
    : m_consumer(fn->body()), m_args(args), m_function(fn), m_vars()
{
    if (fn->arguments().size() != args.size()) {
        report::error(call_site, "Incorrect number of arguments passed to function.");
    }

    for (auto i = 0; i < args.size(); ++i) {
        auto expected_arg = fn->arguments().at(i);
        auto arg = args.at(i);

        exe::variable var { expected_arg.name(), expected_arg.type().lock(), arg };
        m_vars.emplace_back(var);
    }
}

auto kdl::lib::exe::function_execution::execute() -> kdl::lib::lexeme
{
    auto construction_type = m_function->construction_type().lock();
    lexeme result { lexeme_type::integer, m_consumer.at(0).file_reference(), "0" };

    std::vector<lexeme> output_queue;
    std::vector<lexeme> operator_stack;

    while (!m_consumer.finished()) {
        auto token = m_consumer.read();
        if (token.is(lexeme_type::identifier)) {
            token = resolve_variable_named(token);
        }

        // Shunting yard algorithm...
        if (token.is_one_of({ lexeme_type::integer, lexeme_type::percentage })) {
            output_queue.emplace_back(token);
        }
        else if (token.is_one_of({
            lexeme_type::plus, lexeme_type::minus, lexeme_type::star, lexeme_type::slash,
            lexeme_type::shift_left, lexeme_type::shift_right, lexeme_type::bar, lexeme_type::amp, lexeme_type::carat
        })) {
            while (!operator_stack.empty()) {
                const auto& o2 = operator_stack.back();
                if ((!o2.is(lexeme_type::lparen)) && (o2.value<std::int64_t>() >= token.value<std::int64_t>()) && token.left_associative()) {
                    output_queue.emplace_back(operator_stack.back());
                    operator_stack.pop_back();
                }
                else {
                    break;
                }
            }
            operator_stack.emplace_back(token);
        }
        else if (token.is(lexeme_type::lparen)) {
            operator_stack.emplace_back(token);
        }
        else if (token.is(lexeme_type::rparen)) {
            while (!operator_stack.empty() && !operator_stack.back().is(lexeme_type::lparen)) {
                output_queue.emplace_back(operator_stack.back());
                operator_stack.pop_back();
            }

            if (!operator_stack.back().is(lexeme_type::lparen)) {
                report::error(operator_stack.back(), "Expected an opening parentheses '('.");
            }

            operator_stack.pop_back();
        }
    }

    while (!operator_stack.empty()) {
        output_queue.emplace_back(operator_stack.back());
        operator_stack.pop_back();
    }

    // Ensure that we're actually at the end of the expression.
    if (!m_consumer.finished()) {
        report::error(m_consumer.peek(), "Unexpected end of expression reached.");
    }

    // Evaluate the expression.
    std::vector<__int128> working_stack;
    for (const auto& o : output_queue) {
        if (o.is_one_of({ lexeme_type::integer, lexeme_type::hex, lexeme_type::percentage })) {
            working_stack.emplace_back(o.value<__int128>());
        }
        else if (o.is(lexeme_type::plus)) {
            const auto lhs = working_stack.back(); working_stack.pop_back();
            const auto rhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs + rhs);
        }
        else if (o.is(lexeme_type::minus)) {
            const auto lhs = working_stack.back(); working_stack.pop_back();
            const auto rhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs - rhs);
        }
        else if (o.is(lexeme_type::star)) {
            const auto lhs = working_stack.back(); working_stack.pop_back();
            const auto rhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs * rhs);
        }
        else if (o.is(lexeme_type::slash)) {
            const auto lhs = working_stack.back(); working_stack.pop_back();
            const auto rhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs / rhs);
        }
        else if (o.is(lexeme_type::carat)) {
            const auto lhs = working_stack.back(); working_stack.pop_back();
            const auto rhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs ^ rhs);
        }
        else if (o.is(lexeme_type::shift_right)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs >> rhs);
        }
        else if (o.is(lexeme_type::shift_left)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs << rhs);
        }
        else if (o.is(lexeme_type::bar)) {
            const auto lhs = working_stack.back(); working_stack.pop_back();
            const auto rhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs | rhs);
        }
        else if (o.is(lexeme_type::amp)) {
            const auto lhs = working_stack.back(); working_stack.pop_back();
            const auto rhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs & rhs);
        }
        else {
            report::error(o, "Unknown operator encountered.");
        }
    }

    if (working_stack.empty() || working_stack.size() > 1) {
        report::error(m_function->body().front(), "There was an error evaluating the expression. [" + std::to_string(working_stack.size()) + "]");
    }

    __int128 r = working_stack.front();
    if (r < 0) {
        result = lexeme(lexeme_type::integer, result.file_reference(), std::to_string(static_cast<int64_t>(r)));
    }
    else {
        result = lexeme(lexeme_type::integer, result.file_reference(), std::to_string(static_cast<uint64_t>(r)));
    }

    return result;
}

// MARK: - Variables

auto kdl::lib::exe::function_execution::resolve_variable_named(const kdl::lib::lexeme &var) const -> kdl::lib::lexeme
{
    for (auto it = m_vars.rbegin(); it != m_vars.rend(); ++it) {
        if (it->name() == var.string_value()) {
            return it->value();
        }
    }
    report::error(var, "Unrecognised variable name.");
}
