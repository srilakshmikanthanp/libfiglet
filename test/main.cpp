// Copyright (c) 2022 Sri Lakshmi Kanthan P
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <iostream>
#include "../include/srilakshmikanthanp/Figlet.hpp"

using namespace srilakshmikanthanp;

int main()
{
    Figlet figlet(FigletFont::make("../fonts/Standard.flf"), Smushed::make());
    std::cout << figlet("Hello, C++");
    return 0;
}