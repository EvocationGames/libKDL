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

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <unordered_map>
#include <kdl/schema/module_type.hpp>

namespace kdl::lib
{
    class name_space;
    struct function;
    struct binary_type;
    struct binary_template;
    struct resource_type;
    struct resource;
    struct scene;

    class module: public std::enable_shared_from_this<module>
    {
    public:
        explicit module(const std::string& name, module_type = module_type::general, const std::weak_ptr<module>& parent = {});

        auto set_version(const std::string& version) -> void;
        auto set_namespace(const std::weak_ptr<name_space>& ns) -> void;
        auto add_author(const std::string& author) -> void;
        auto add_copyright(const std::string& copy) -> void;
        auto add_submodule(const std::shared_ptr<module>& submodule) -> void;

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto submodule_named(const std::string&) const -> std::weak_ptr<module>;
        [[nodiscard]] auto copyright(bool complete = true) const -> std::vector<std::string>;
        [[nodiscard]] auto authors(bool complete = true) const -> std::vector<std::string>;
        [[nodiscard]] auto get_namespace() -> std::weak_ptr<name_space>;

        [[nodiscard]] auto type() const -> module_type;

        auto add_binary_type_definition(const std::shared_ptr<binary_type>& type) -> void;
        [[nodiscard]] auto binary_type_named(const std::string& name, const std::vector<std::string>& path = {}) -> std::weak_ptr<binary_type>;

        auto add_binary_template_definition(const std::shared_ptr<binary_template>& tmpl) -> void;
        [[nodiscard]] auto binary_template_named(const std::string& name, const std::vector<std::string>& path = {}) -> std::weak_ptr<binary_template>;
        [[nodiscard]] auto binary_templates() const -> const std::vector<std::shared_ptr<binary_template>>&;

        auto add_resource_type_definition(const std::shared_ptr<resource_type>& type) -> void;
        [[nodiscard]] auto resource_type_named(const std::string& name, const std::vector<std::string>& path = {}) -> std::weak_ptr<resource_type>;
        [[nodiscard]] auto resource_types() -> std::vector<std::shared_ptr<resource_type>>;

        auto add_function(const std::shared_ptr<function>& fn) -> void;
        [[nodiscard]] auto function_named(const std::string& name, const std::string& type, const std::vector<std::string>& path = {}) -> std::weak_ptr<function>;

        auto add_resource(const std::shared_ptr<resource>& res) -> void;
        [[nodiscard]] auto resources(const std::string& type) const -> std::vector<std::shared_ptr<resource>>;

        auto add_scene(const std::shared_ptr<scene>& scene) -> void;
        [[nodiscard]] auto scenes() const -> std::vector<std::shared_ptr<scene>>;

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
        std::vector<std::shared_ptr<function>> m_functions;
        std::vector<std::shared_ptr<resource_type>> m_resource_type_definitions;
        std::unordered_map<std::string, std::vector<std::shared_ptr<resource>>> m_resource_declarations;
        std::vector<std::shared_ptr<scene>> m_scenes;

    };

}
