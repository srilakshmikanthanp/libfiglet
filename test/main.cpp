// Copyright (c) 2022 Sri Lakshmi Kanthan P
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "../include/srilakshmikanthanp/libfiglet.hpp"
#include <iostream>

using namespace srilakshmikanthanp::libfiglet;

int main()
{
    const auto font_file = "D:\\source\\srilakshmikanthanp\\libfiglet\\assets\\fonts\\Standard.flf";
    const figlet figlet(flf_font::make_shared(font_file), smushed::make_shared());
    std::cout << figlet("Hello");
}
