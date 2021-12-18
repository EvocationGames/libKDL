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

#if !defined(KDL_SCHEMA_MODULE_HPP)
#define KDL_SCHEMA_MODULE_HPP

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <kdl/schema/module_type.hpp>
#include <kdl/schema/binary_type/binary_type.hpp>
#include <kdl/schema/binary_template/binary_template.hpp>

namespace kdl::lib
{
    class name_space;

    class module: public std::enable_shared_from_this<module>
    {
    private:
        module_type m_type { module_type::general };
        std::weak_ptr<name_space> m_namespace;
        std::weak_ptr<module> m_parent;
        std::vector<std::shared_ptr<module>> m_submodules;
        std::string m_module_name;
        std::string m_version;
        std::vector<std::string> m_authors;
        std::vector<std::string> m_copyright;
        std::vector<std::shared_ptr<binary_type>> m_binary_type_definitions;
        std::vector<std::shared_ptr<binary_template>> m_template_definitions;
        std::vector<int> m_resource_type_definitions;
        std::vector<int> m_resource_declarations;

    public:
        explicit module(const std::string& name, module_type = module_type::general, const std::weak_ptr<module>& parent = {});

        auto set_version(const std::string& version) -> void;
        auto set_namespace(const std::weak_ptr<name_space>& ns) -> void;
        auto add_author(const std::string& author) -> void;
        auto add_copyright(const std::string& copy) -> void;
        auto add_submodule(const std::shared_ptr<module>& submodule) -> void;

        [[nodiscard]] auto submodule_named(const std::string&) const -> std::weak_ptr<module>;
        [[nodiscard]] auto copyright(bool complete = true) const -> std::vector<std::string>;
        [[nodiscard]] auto authors(bool complete = true) const -> std::vector<std::string>;
        [[nodiscard]] auto get_namespace() -> std::weak_ptr<name_space>;

        [[nodiscard]] auto type() const -> module_type;

        auto add_binary_type_definition(const std::shared_ptr<binary_type>& type) -> void;
        [[nodiscard]] auto binary_type_named(const std::string& name, const std::vector<std::string>& path = {}) -> std::weak_ptr<binary_type>;

        auto add_binary_template_definition(const std::shared_ptr<binary_template>& tmpl) -> void;
    };

}

#endif //KDL_SCHEMA_MODULE_HPP
