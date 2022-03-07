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

#include <kdl/schema/project/scene.hpp>

// MARK: - Construction

kdl::lib::scene::scene(const std::string &name)
    : m_name(name)
{
}

// MARK: - Attribute Management

auto kdl::lib::scene::set_attribute(const std::string &attribute, const lexeme &value) -> void
{
    set_attribute(attribute, std::vector<lexeme>({value}));
}

auto kdl::lib::scene::set_attribute(const std::string &attribute, const std::vector<lexeme> &value) -> void
{
    m_attributes.insert(std::pair(attribute, value));
}

auto kdl::lib::scene::get_attribute(const std::string &attribute) -> std::vector<lexeme> &
{
    return m_attributes.at(attribute);
}

auto kdl::lib::scene::has_attribute(const std::string &attribute) -> bool
{
    return (m_attributes.find(attribute) != m_attributes.end());
}

// MARK: - Scene Events

auto kdl::lib::scene::set_event_scripts(const std::string& event, const std::vector<lexeme>& scripts) -> void
{
    m_events.insert(std::pair(event, scripts));
}

auto kdl::lib::scene::event_scripts(const std::string& event) -> std::vector<lexeme>
{
    auto it = m_events.find(event);
    if (it == m_events.end()) {
        return {};
    }
    return it->second;
}
