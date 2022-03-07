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

#if !defined(KDL_FILE_SOURCE_FILE_HPP)
#define KDL_FILE_SOURCE_FILE_HPP

#include <string>
#include <memory>

namespace kdl::lib
{

    class source_file: public std::enable_shared_from_this<source_file>
    {
    private:
        static constexpr const char * memory { "{*MEMORY*}" };

        std::string m_file_path;
        std::string m_source;
        std::size_t m_source_size;

    public:
        explicit source_file(std::string source, std::string path = source_file::memory);

        [[nodiscard]] auto source() const -> std::string;
        [[nodiscard]] auto path() const -> std::string;

        [[nodiscard]] auto size() const -> std::size_t;

        [[nodiscard]] auto relative_path(const std::string& rel) const -> std::string;
    };

}

#endif //KDL_FILE_SOURCE_FILE_HPP
