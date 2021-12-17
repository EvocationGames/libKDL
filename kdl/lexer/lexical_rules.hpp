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

#if !defined(LEXICAL_RULES_HPP)
#define LEXICAL_RULES_HPP

#include <iostream>
namespace kdl::lib::lexical_rule
{

    template<char c>
    struct match
    {
        static auto yes(const std::string& chk) -> bool
        {
            return chk == std::string(1, c);
        }

        static auto no(const std::string& chk) -> bool
        {
            return !yes(chk);
        }
    };

    template<char tc, char...ttc>
    struct sequence
    {
        static auto matches(const std::string& chk) -> bool
        {
            std::vector<char> v = {tc, ttc...};
            if (v.size() != chk.size()) {
                return false;
            }
            for (auto i = 0; i < v.size(); ++i) {
                if (chk[i] != v[i]) {
                    return false;
                }
            }
            return true;
        }

        static auto mismatch(const std::string& chk) -> bool
        {
            return !matches(chk);
        }
    };

    template<char lc, char uc>
    struct range
    {
        static auto contains(const std::string& chk) -> bool
        {
            for (const auto c : chk) {
                if (c < lc || c > uc) {
                    return false;
                }
            }
            return true;
        }

        static auto missing(const std::string& chk) -> bool
        {
            return !contains(chk);
        }
    };

    template<char tc, char...ttc>
    struct set
    {
        static auto contains(const std::string& chk) -> bool
        {
            std::vector<char> v = {tc, ttc...};
            return !std::any_of(chk.begin(), chk.end(), [&] (const char c) {
                return std::find(v.begin(), v.end(), c) == v.end();
            });
        }

        static auto missing(const std::string& chk) -> bool
        {
            return !contains(chk);
        }
    };

    struct identifier
    {
        struct set
        {
            static auto contains(const std::string& chk) -> bool
            {
                return range<'A', 'Z'>::contains(chk)
                    || range<'a', 'z'>::contains(chk)
                    || range<'0', '9'>::contains(chk)
                    || match<'_'>::yes(chk);
            }

            static auto missing(const std::string& chk) -> bool
            {
                return !contains(chk);
            }
        };

        struct limited_set
        {
            static auto contains(const std::string& chk) -> bool
            {
                return range<'A', 'Z'>::contains(chk)
                    || range<'a', 'z'>::contains(chk)
                    || match<'_'>::yes(chk);
            }

            static auto missing(const std::string& chk) -> bool
            {
                return !contains(chk);
            }
        };

        static auto matches(std::size_t pos, const std::string& chk) -> bool
        {
            for (auto it = chk.begin(); it != chk.end(); ++it) {
                auto fn = (it == chk.begin() && pos == 0) ? limited_set::contains : set::contains;
                if (!fn(std::string(1, *it))) {
                    return false;
                }
            }
            return true;
        }

        static auto mismatch(std::size_t pos, const std::string& chk) -> bool
        {
            return !matches(pos, chk);
        }
    };

    struct numeric
    {
        typedef lexical_rule::range<'0', '9'> decimal;

        struct hexadecimal
        {
            static auto contains(const std::string& chk) -> bool
            {
                return std::all_of(chk.begin(), chk.end(), [&] (const char c) {
                    return (c >= 'A' && c <= 'F')
                    || (c >= 'a' && c <= 'f')
                    || (c >= '0' && c <= '9');
                });
            }

            static auto missing(const std::string& chk) -> bool
            {
                return !contains(chk);
            }

            static auto matches(const std::string& chk) -> bool
            {
                if (chk[0] == '0' && (chk[1] == 'x' || chk[1] == 'X')) {
                    return contains(chk.substr(2));
                }
                else {
                    return contains(chk);
                }
            }

            static auto mismatch(const std::string& chk) -> bool
            {
                return !matches(chk);
            }
        };

        static auto matches(const std::string& chk) -> bool
        {
            return decimal::contains(chk)
                || hexadecimal::matches(chk)
                || set<'-'>::contains(chk);
        }

        static auto mismatch(const std::string& chk) -> bool
        {
            return !matches(chk);
        }
    };

    typedef set<' ', '\t'> whitespace;
    typedef match<'\n'> unix_nl;
    typedef match<'\r'> cr;

}

#endif
