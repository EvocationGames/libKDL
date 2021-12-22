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

#include <iostream>
#include <kdl/report/reporting.hpp>

static auto construct_report(const kdl::lib::lexeme& lx, const std::string& message, const std::string& prefix) -> std::string
{
    // Request the entire line that the lexeme is in, from its source file.
    auto line = lx.file_reference().complete_source_line();

    auto out = prefix + ": " + lx.file_reference().describe() + "\x1b[0m\n";
    out += "\t" + message + "\n";
    out += "\t" + line + "\n";
    out += "\t";
    for (auto i = 0; i < lx.file_reference().line_offset(); ++i) {
        out += " ";
    }
    out += "^\n";

    return out;
}

auto kdl::lib::report::note(const lexeme& lx, const std::string& message) -> void
{
    auto report = construct_report(lx, message, "\x1b[1;37mNote");
    std::cout << report << std::endl;
}

auto kdl::lib::report::warn(const lexeme& lx, const std::string& message) -> void
{
    auto report = construct_report(lx, message, "\x1b[1;33mWarning");
    std::cout << report << std::endl;
}

auto kdl::lib::report::error(const lexeme& lx, const std::string& message) -> void
{
    auto report = construct_report(lx, message, "\x1b[31mError");
    std::cerr << report << std::endl;
    exit(1);
}


auto kdl::lib::report::note(const std::string& message) -> void
{
    std::cout << "\x1b[1;37mNote:" << message << "\x1b[0m" << std::endl;
}

auto kdl::lib::report::warn(const std::string& message) -> void
{
    std::cout << "\x1b[1;33mWarning:" << message << "\x1b[0m" << std::endl;
}

auto kdl::lib::report::error(const std::string& message) -> void
{
    std::cerr << "\x1b[31mError:" << message << "\x1b[0m" << std::endl;
    exit(1);
}

