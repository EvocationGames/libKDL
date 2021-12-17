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

#if !defined(KDL_FILE_FILE_REFERENCE_HPP)
#define KDL_FILE_FILE_REFERENCE_HPP

#include <memory>
#include <kdl/file/source_file.hpp>

namespace kdl::lib
{
    class file_reference
    {
    private:
        std::shared_ptr<source_file> m_file { nullptr };
        std::size_t m_absolute_position { 0 };
        std::size_t m_line_offset { 0 };
        std::size_t m_line { 0 };
        std::size_t m_size { 0 };

    public:
        file_reference() = default;
        file_reference(std::shared_ptr<source_file> file, std::size_t pos, std::size_t line, std::size_t offset, std::size_t size);

        [[nodiscard]] auto valid() const -> bool;

        [[nodiscard]] auto file() const -> source_file&;
        [[nodiscard]] auto absolute_position() const -> std::size_t;
        [[nodiscard]] auto line_offset() const -> std::size_t;
        [[nodiscard]] auto line() const -> std::size_t;

        [[nodiscard]] auto describe() const -> std::string;
    };
}

#endif
