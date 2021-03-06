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

#include <utility>
#include <unordered_set>
#include <kdl/schema/module.hpp>
#include <kdl/schema/namespace.hpp>
#include <kdl/schema/binary_type/binary_type.hpp>
#include <kdl/schema/binary_template/binary_template.hpp>
#include <kdl/schema/resource_type/resource_type.hpp>
#include <kdl/schema/function/function.hpp>
#include <kdl/schema/project/scene.hpp>
#include <kdl/schema/resource/resource.hpp>

// MARK: - Constructor

kdl::lib::module::module(const std::string& name, module_type type, const std::weak_ptr<module>& parent)
    : m_module_name(name), m_type(type), m_parent(parent)
{
    if (auto p = m_parent.lock()) {
        p->add_submodule(shared_from_this());
    }
}

// MARK: - Setters

auto kdl::lib::module::set_version(const std::string& version) -> void
{
    m_version = version;
}

auto kdl::lib::module::set_namespace(const std::weak_ptr<name_space>& ns) -> void
{
    m_namespace = ns;
}

auto kdl::lib::module::add_author(const std::string& author) -> void
{
    m_authors.emplace_back(author);
}

auto kdl::lib::module::add_copyright(const std::string& copy) -> void
{
    m_copyright.emplace_back(copy);
}

auto kdl::lib::module::add_submodule(const std::shared_ptr<module>& submodule) -> void
{
    // Make sure the submodule doesn't have a parent already.
    if (submodule->m_parent.lock()) {
        // TODO: Attempt to add a module to more than one location should be illegal?
        return;
    }
    m_submodules.emplace_back(submodule);
}

// MARK: - Lookup

auto kdl::lib::module::submodule_named(const std::string& name) const -> std::weak_ptr<module>
{
    for (auto& submodule : m_submodules) {
        if (submodule->m_module_name == name) {
            return submodule->shared_from_this();
        }
    }
    return {};
}

// MARK: - Accessors

auto kdl::lib::module::name() const -> std::string
{
    return m_module_name;
}

auto kdl::lib::module::copyright(bool complete) const -> std::vector<std::string>
{
    std::vector<std::string> copy = m_copyright;

    if (complete) {
        if (auto parent = m_parent.lock()) {
            auto parent_copyright = parent->copyright(complete);
            copy.insert(copy.begin(), parent_copyright.begin(), parent_copyright.end());
        }
    }

    return copy;
}

auto kdl::lib::module::authors(bool complete) const -> std::vector<std::string>
{
    std::vector<std::string> authors = m_authors;

    if (complete) {
        if (auto parent = m_parent.lock()) {
            auto parent_authors = parent->authors(complete);
            authors.insert(authors.begin(), parent_authors.begin(), parent_authors.end());
        }
    }

    return authors;
}

auto kdl::lib::module::get_namespace() -> std::weak_ptr<name_space>
{
    return m_namespace.expired()
        ? (m_parent.expired() ? std::weak_ptr<name_space>() : m_parent.lock()->get_namespace())
        : m_namespace;
}

auto kdl::lib::module::type() const -> module_type
{
    return m_type;
}

// MARK: - Binary Types

auto kdl::lib::module::add_binary_type_definition(const std::shared_ptr<binary_type>& type) -> void
{
    // TODO: Check for existing binary type.
    m_binary_type_definitions.emplace_back(type);

    if (auto ns = get_namespace().lock()) {
        ns->register_binary_type(type);
    }
}

auto kdl::lib::module::binary_type_named(const std::string& name, const std::vector<std::string>& path) -> std::weak_ptr<binary_type>
{
    if (path.size() == 1 && path.at(0) == "this") {
        for (const auto& type : m_binary_type_definitions) {
            if (type->name() == name) {
                return type;
            }
        }
        return {};
    }

    if (auto ns = get_namespace().lock()) {
        return ns->binary_type_named(name, path);
    }
    else {
        return {};
    }
}

// MARK: - Binary Templates

auto kdl::lib::module::add_binary_template_definition(const std::shared_ptr<binary_template>& tmpl) -> void
{
    // TODO: Check for existing binary template
    m_template_definitions.emplace_back(tmpl);

    if (auto ns = get_namespace().lock()) {
        ns->register_binary_template(tmpl);
    }
}

auto kdl::lib::module::binary_template_named(const std::string& name, const std::vector<std::string>& path) -> std::weak_ptr<binary_template>
{
    if (path.size() == 1 && path.at(0) == "this") {
        for (const auto& tmpl : m_template_definitions) {
            if (tmpl->name() == name) {
                return tmpl;
            }
        }
        return {};
    }

    if (auto ns = get_namespace().lock()) {
        return ns->binary_template_named(name, path);
    }
    else {
        return {};
    }
}

auto kdl::lib::module::binary_templates() const -> const std::vector<std::shared_ptr<binary_template>> &
{
    return m_template_definitions;
}

// MARK: - Resource Type Definitions

auto kdl::lib::module::add_resource_type_definition(const std::shared_ptr<resource_type>& type) -> void
{
    m_resource_type_definitions.emplace_back(type);

    if (auto ns = get_namespace().lock()) {
        ns->register_resource_type(type);
    }
}

auto kdl::lib::module::resource_type_named(const std::string& name, const std::vector<std::string>& path) -> std::weak_ptr<resource_type>
{
    if (path.size() == 1 && path.at(0) == "this") {
        for (const auto& type : m_resource_type_definitions) {
            if (type->name() == name) {
                return type;
            }
        }
        return {};
    }

    if (auto ns = get_namespace().lock()) {
        return ns->resource_type_named(name, path);
    }
    else {
        return {};
    }
}

auto kdl::lib::module::resource_types() -> std::vector<std::shared_ptr<resource_type>>
{
    std::unordered_set<std::string> type_names;
    std::vector<std::shared_ptr<resource_type>> types = m_resource_type_definitions;

    for (const auto& type : types) {
        type_names.insert(type->name());
    }

    if (auto ns = get_namespace().lock()) {
        for (const auto& res : m_resource_declarations) {
            if (auto type = ns->resource_type_named(res.first, {}).lock()) {
                if (type_names.find(type->name()) == type_names.end()) {
                    type_names.insert(type->name());
                    types.emplace_back(type);
                }
            }
        }
    }

    return types;
}

// MARK: - Functions

auto kdl::lib::module::add_function(const std::shared_ptr<function>& fn) -> void
{
    m_functions.emplace_back(fn);

    if (auto ns = get_namespace().lock()) {
        ns->register_function(fn);
    }
}

auto kdl::lib::module::function_named(const std::string& name, const std::string& type, const std::vector<std::string>& path) -> std::weak_ptr<function>
{
    if (path.size() == 1 && path.at(0) == "this") {
        for (const auto& fn : m_functions) {
            auto construction_type = fn->construction_type().lock();
            if (construction_type && fn->name() == name && construction_type->name() == type) {
                return fn;
            }
        }
        return {};
    }

    if (auto ns = get_namespace().lock()) {
        return ns->function_named(name, path);
    }
    else {
        return {};
    }
}

// MARK: - Scenes

auto kdl::lib::module::add_scene(const std::shared_ptr<scene>& scene) -> void
{
    m_scenes.emplace_back(scene);
}

auto kdl::lib::module::scenes() const -> std::vector<std::shared_ptr<scene>>
{
    return m_scenes;
}

// MARK: - Resources

auto kdl::lib::module::add_resource(const std::shared_ptr<resource> &res) -> void
{
    auto type_name = res->type().lock();
    if (!type_name) {
        // TODO: Handle this error case correctly.
        return;
    }

    auto it = m_resource_declarations.find(type_name->name());
    if (it == m_resource_declarations.end()) {
        auto pair = std::pair(type_name->name(), std::vector<std::shared_ptr<resource>>());
        it = m_resource_declarations.insert(pair).first;
    }

    it->second.emplace_back(res);
}

auto kdl::lib::module::resources(const std::string& type) const -> std::vector<std::shared_ptr<resource>>
{
    auto it = m_resource_declarations.find(type);
    if (it == m_resource_declarations.end()) {
        // TODO: Handle the type not being found correctly.
        return {};
    }
    return it->second;
}
