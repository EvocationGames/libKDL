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

#if !defined(KDL_SCHEMA_BINARY_TEMPLATE_HPP)
#define KDL_SCHEMA_BINARY_TEMPLATE_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <kdl/schema/binary_type/binary_type.hpp>

namespace kdl::lib
{
    struct binary_template_field;

    struct binary_template
    {
    private:
        std::string m_name;
        std::vector<std::shared_ptr<binary_template_field>> m_fields;

    public:
        explicit binary_template(const std::string& name);

        auto add_field(const std::shared_ptr<binary_type>& type, const std::unordered_map<std::string, lexeme>& type_args, const std::string& name) -> void;

        [[nodiscard]] auto name() const -> std::string;

        [[nodiscard]] auto field_count() const -> std::size_t;
        [[nodiscard]] auto field_at(std::size_t i) const -> std::shared_ptr<binary_template_field>;
        [[nodiscard]] auto field_named(const std::string& name) const -> std::weak_ptr<binary_template_field>;
    };

}

#endif //KDL_SCHEMA_BINARY_TEMPLATE_BINARY_TEMPLATE_HPP
