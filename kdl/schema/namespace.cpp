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

#include <kdl/schema/namespace.hpp>
#include <kdl/schema/binary_type/binary_type.hpp>

namespace kdl::lib::spec::keywords
{
    constexpr const char * self { "self" };
}

// MARK: - Construction

kdl::lib::name_space::name_space(const std::string& name, const std::weak_ptr<name_space>& parent)
    : m_name(name), m_parent(parent)
{

}

// MARK: - Child Management

auto kdl::lib::name_space::create(const std::string &name) -> std::weak_ptr<name_space>
{
    auto child = std::make_shared<name_space>(name, shared_from_this());
    m_children.emplace_back(child);
    return child;
}

// MARK: - Namespace Lookup

auto kdl::lib::name_space::parent(bool root) -> std::shared_ptr<name_space>
{
    if (auto p = m_parent.lock()) {
        return root ? p->parent(root) : p;
    }
    else {
        return shared_from_this();
    }
}

auto kdl::lib::name_space::global() -> std::shared_ptr<name_space>
{
    return parent(true);
}

auto kdl::lib::name_space::child_named(const std::string& name) -> std::shared_ptr<name_space>
{
    if (name == spec::keywords::self) {
        return shared_from_this();
    }

    for (const auto& child : m_children) {
        if (child->m_name == name) {
            return child;
        }
    }

    return nullptr;
}

auto kdl::lib::name_space::resolve_path(const std::vector<std::string>& path) -> std::shared_ptr<name_space>
{
    if (!path.empty()) {
        auto child = child_named(path.at(0));
        if (child) {
            auto new_path = path;
            new_path.erase(new_path.begin());
            return child->resolve_path(new_path);
        }
        else {
            return global()->resolve_path(path);
        }
    }
    else {
       return shared_from_this();
    }
}

// MARK: - Binary Type Management

auto kdl::lib::name_space::register_binary_type(const std::weak_ptr<binary_type>& type) -> void
{
    m_binary_types.emplace_back(type);
}

auto kdl::lib::name_space::binary_type_named(const std::string& name, const std::vector<std::string>& path) -> std::weak_ptr<binary_type>
{
    auto ns = resolve_path(path);
    if (ns == nullptr) {
        throw std::runtime_error("Unable to find any namespace.");
    }

    if (ns.get() != this) {
        return ns->binary_type_named(name, {});
    }
    else {
        for (const auto& weak : m_binary_types) {
            if (const auto& type = weak.lock(); type->name() == name) {
                return weak;
            }
        }
        return {};
    }
}