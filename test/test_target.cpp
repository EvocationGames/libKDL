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
#include <kdl/lexer/lexer.hpp>
#include <kdl/parser/parser.hpp>

auto main(const int argc, const char **argv) -> int
{
    if (argc > 1) {
        std::string command { argv[1] };
        if (command == "lexemes" && argc > 2) {
            auto input = std::make_shared<kdl::lib::source_file>("", std::string(argv[2]));
            for (const auto& lx : kdl::lib::lexer(input).scan(false)) {
                std::cout << lx.describe() << std::endl;
            }
        }
        else if (command == "schema" && argc > 2) {
            auto input = std::make_shared<kdl::lib::source_file>("", std::string(argv[2]));
            kdl::lib::parser().parse(input);
        }
        else {
            std::cerr << "unrecognised command: " << command << std::endl;
        }
    }
    return 0;
}