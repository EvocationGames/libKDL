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

#if !defined(KDL_SCHEMA_FUNCTION_FUNCTION_HPP)
#define KDL_SCHEMA_FUNCTION_FUNCTION_HPP

#include <string>
#include <vector>
#include <memory>
#include <kdl/lexer/lexeme.hpp>

namespace kdl::lib
{
    struct function_argument;
    struct binary_type;

    struct function
    {
    private:
        std::string m_name;
        std::weak_ptr<struct binary_type> m_construction_type;
        std::vector<struct function_argument> m_arguments;
        std::vector<lexeme> m_body;

    public:
        explicit function(const std::string& name, const std::shared_ptr<struct binary_type>& type = nullptr);

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto construction_type() const -> std::weak_ptr<struct binary_type>;
        [[nodiscard]] auto arguments() const -> std::vector<struct function_argument>;
        [[nodiscard]] auto argument_type_at(std::size_t i) const -> std::weak_ptr<struct binary_type>;
        [[nodiscard]] auto body() const -> std::vector<lexeme>;

        auto add_argument(const struct function_argument& arg) -> void;
        auto set_body(const std::vector<lexeme>& body) -> void;

    };
}

#endif //KDL_SCHEMA_FUNCTION_FUNCTION_HPP
