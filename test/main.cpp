// Copyright (c) 2022 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "../src/srilakshmikanthanp/libfiglet.hpp"
#include <filesystem>
#include <iostream>


using namespace srilakshmikanthanp::libfiglet;


void flf_font_test(void)
{
  const auto base_dir_path = "D:\\source\\srilakshmikanthanp\\libfiglet\\assets\\fonts";

  for (const auto & entry : std::filesystem::directory_iterator(base_dir_path))
  {
    flf_font::make_shared(entry.path().string());
  }
}


void style_test(void)
{
  // Font and figlet objects
  const auto font_path  = "D:\\source\\srilakshmikanthanp\\libfiglet\\assets\\fonts\\Standard.flf";
  const auto flf_font   = flf_font::make_shared(font_path);
  const auto full_width = figlet(flf_font, full_width::make_shared());
  const auto kerning    = figlet(flf_font, kerning::make_shared());
  const auto smushed    = figlet(flf_font, smushed::make_shared());

  // Print
  std::cout << full_width("Hello, C++") <<  std::endl;
  std::cout << kerning("Hello, C++")    <<  std::endl;
  std::cout << smushed("Hello, C++")    <<  std::endl;
}

int main(void)
{
  try
  {
    std::cout << "flf_font_test Start..." << std::endl;
    flf_font_test();
    std::cout << "flf_font_test Done" << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << "font Test Failed : " << e.what() << '\n';
  }

  try
  {
    std::cout << "style_test Start..." << std::endl;
    style_test();
    std::cout << "style_test Done" << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << "Style Test Failed : " << e.what() << '\n';
  }
}
