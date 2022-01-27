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

#if !defined(KDL_SCHEMA_RESOURCE_TYPE_HPP)
#define KDL_SCHEMA_RESOURCE_TYPE_HPP

#include <string>
#include <memory>
#include <vector>

namespace kdl::lib
{
    struct binary_template;
    struct resource_field;

    struct resource_type
    {
    private:
        std::string m_name;
        std::string m_code;
        std::weak_ptr<binary_template> m_template;
        std::vector<std::shared_ptr<resource_field>> m_fields;

    public:
        resource_type(const std::string& name, const std::string& code);

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto code() const -> std::string;

        auto set_binary_template(const std::weak_ptr<struct binary_template>& tmpl) -> void;
        [[nodiscard]] auto binary_template() const -> std::weak_ptr<struct binary_template>;

        auto add_field(const std::shared_ptr<struct resource_field>& field) -> void;
        [[nodiscard]] auto fields() const -> const std::vector<std::shared_ptr<resource_field>>&;
    };
}

#endif
