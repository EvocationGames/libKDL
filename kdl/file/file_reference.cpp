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
#include "file/file_reference.hpp"

// MARK: - Constructor

kdl::file_reference::file_reference(std::shared_ptr<source_file> file, std::size_t pos, std::size_t line, std::size_t offset)
    : m_file(std::move(file)), m_absolute_position(pos), m_line_offset(offset), m_line(line)
{

};

// MARK: - Accessors

auto kdl::file_reference::valid() const -> bool
{
    return (m_file != nullptr);
}

auto kdl::file_reference::file() const -> source_file&
{
    return *m_file;
}

auto kdl::file_reference::absolute_position() const -> std::size_t
{
    return m_absolute_position;
}

auto kdl::file_reference::line_offset() const -> std::size_t
{
    return m_line_offset;
}

auto kdl::file_reference::line() const -> std::size_t
{
    return m_line;
}