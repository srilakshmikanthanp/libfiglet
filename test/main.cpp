#include "../include/srilakshmikanthanp/libfiglet.hpp"
#include <filesystem>
#include <iostream>

using namespace srilakshmikanthanp::libfiglet;

int main(void)
{
  const auto base_dir_path = "D:\\source\\srilakshmikanthanp\\libfiglet\\assets\\fonts";

  for (const auto & entry : std::filesystem::directory_iterator(base_dir_path))
  {
    try
    {
      const auto flf_font = flf_font::make_shared(entry.path().string());
      const figlet figlet(flf_font, full_width::make_shared());
      std::cout << entry.path().filename() << "\n"  << figlet("Hello, C++");
    }
    catch (const std::exception & e)
    {
      std::cout << e.what() << entry.path().string() << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}
