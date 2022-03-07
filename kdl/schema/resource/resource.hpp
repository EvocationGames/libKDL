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

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <kdl/lexer/lexeme.hpp>

namespace kdl::lib
{
    struct resource_type;

    struct resource
    {
    public:
        resource(const std::shared_ptr<resource_type>& type, int64_t id, const std::string& name = "");

        [[nodiscard]] inline auto id() const -> int64_t { return m_id; }
        [[nodiscard]] inline auto name() const -> std::string { return m_name; }
        [[nodiscard]] inline auto type() const -> std::weak_ptr<resource_type> { return m_type; }

        [[nodiscard]] auto value(const std::string& field_name) const -> lexeme;
        auto set_value(const lexeme& lx, const std::string& field_name) -> void;

    private:
        int64_t m_id;
        std::string m_name;
        std::weak_ptr<resource_type> m_type;
        std::unordered_map<std::string, lexeme> m_values;
    };
}