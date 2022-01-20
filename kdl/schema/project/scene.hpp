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

#include <string>
#include <vector>
#include <unordered_map>
#include <kdl/lexer/lexeme.hpp>

namespace kdl::lib
{
    struct layer;

    struct scene
    {
    public:
        explicit scene(const std::string& name);

        [[nodiscard]] inline auto name() const -> std::string { return m_name; }

        auto set_attribute(const std::string& attribute, const lexeme& value) -> void;
        auto set_attribute(const std::string& attribute, const std::vector<lexeme>& value) -> void;

        auto has_attribute(const std::string& attribute) -> bool;
        auto get_attribute(const std::string& attribute) -> std::vector<lexeme>&;

        auto add_layer(const std::shared_ptr<layer>& layer) -> void;
        [[nodiscard]] auto layers() const -> std::vector<std::shared_ptr<layer>>;

    private:
        std::string m_name;
        std::unordered_map<std::string, std::vector<lexeme>> m_attributes;
        std::vector<std::shared_ptr<layer>> m_layers;
    };
}