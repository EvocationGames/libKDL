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
#include <kdl/schema/module.hpp>
#include <kdl/parser/parser.hpp>

// MARK: - Constructor

kdl::lib::module::module(std::string name, module_type type, const std::weak_ptr<module>& parent)
    : m_module_name(std::move(name)), m_type(type), m_parent(parent)
{
    if (auto p = m_parent.lock()) {
        p->add_submodule(shared_from_this());
    }
}

// MARK: - Setters

auto kdl::lib::module::set_parser(const std::weak_ptr<parser>& p) -> void
{
    m_parser = p;
}

auto kdl::lib::module::set_version(const std::string& version) -> void
{
    m_version = version;
}

auto kdl::lib::module::set_namespace(const std::string& ns) -> void
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

auto kdl::lib::module::get_namespace(bool complete) const -> std::string
{
    std::string ns = m_namespace;
    if (complete) {
        if (auto parent = m_parent.lock()) {
            auto parent_ns = parent->get_namespace(complete);
            ns.insert(ns.begin(), parent_ns.begin(), parent_ns.end());
        }
    }
    return ns;
}

auto kdl::lib::module::type() const -> module_type
{
    return m_type;
}

// MARK: - Binary Types

auto kdl::lib::module::add_binary_type_definition(binary_type type) -> void
{
    // TODO: Check for existing binary type.
    m_binary_type_definitions.emplace_back(std::move(type));
}