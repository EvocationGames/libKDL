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

#include <kdl/file/source_file.hpp>
#include <utility>
#include <fstream>

// MARK: - Constructor

kdl::lib::source_file::source_file(std::string source, std::string path)
    : m_file_path(std::move(path)), m_source(std::move(source))
{
    if (m_source.empty() && m_file_path != memory) {
        // Attempt to load the contents of the file from disk.
        std::ifstream file(m_file_path);
        std::string str;
        std::string contents;
        while (std::getline(file, str)) {
            contents += str;
            contents.push_back('\n');
        }
        m_source = std::move(contents);
    }
    m_source_size = m_source.size();
}

// MARK: - Accessors

auto kdl::lib::source_file::source() const -> std::string
{
    return m_source;
}

auto kdl::lib::source_file::path() const -> std::string
{
    return m_file_path;
}

auto kdl::lib::source_file::size() const -> std::size_t
{
    return m_source_size;
}

// MARK: - Path Operations

auto kdl::lib::source_file::relative_path(const std::string &rel) const -> std::string
{
    // Get the parent path...
    auto parent = m_file_path.substr(0, m_file_path.find_last_of('/'));
    if (!rel.empty()) {
        if (rel[0] == '/') {
            return parent + rel;
        }
        else {
            return parent + "/" + rel;
        }
    }
    else {
        return parent;
    }
}
