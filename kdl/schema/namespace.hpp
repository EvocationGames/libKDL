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

#if !defined(KDL_SCHEMA_NAMESPACE_HPP)
#define KDL_SCHEMA_NAMESPACE_HPP

#include <memory>
#include <vector>
#include <string>

namespace kdl::lib
{
    struct binary_type;
    struct binary_template;

    class name_space: public std::enable_shared_from_this<name_space>
    {
    private:
        // Global namespace has no name or parent.
        std::string m_name;
        std::weak_ptr<name_space> m_parent;
        std::vector<std::shared_ptr<name_space>> m_children;
        std::vector<std::weak_ptr<binary_type>> m_binary_types;
        std::vector<std::weak_ptr<binary_template>> m_binary_templates;

    public:
        name_space() = default;
        name_space(const std::string& name, const std::weak_ptr<name_space>& parent);

        auto create(const std::string& name) -> std::weak_ptr<name_space>;

        auto parent(bool root = false) -> std::shared_ptr<name_space>;
        auto global() -> std::shared_ptr<name_space>;

        auto child_named(const std::string& name) -> std::shared_ptr<name_space>;
        auto resolve_path(const std::vector<std::string>& path) -> std::shared_ptr<name_space>;

        auto register_binary_type(const std::weak_ptr<binary_type>& type) -> void;
        [[nodiscard]] auto binary_type_named(const std::string& name, const std::vector<std::string>& path) -> std::weak_ptr<binary_type>;
    };

}

#endif //KDL_SCHEMA_NAMESPACE_HPP
