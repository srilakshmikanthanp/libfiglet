<!-- PROJECT LOGO -->
<h3 align="center">libfiglet</h3>

<p align="center">
  Figlet For C++
  <br />
  <a href="https://srilakshmikanthanp.github.io/libfiglet/docs/html"><strong>Explore the docs »</strong></a>
  <br />
  <a href="https://github.com/srilakshmikanthanp/libfiglet/issues">Report Bug</a>
  ·
  <a href="https://github.com/srilakshmikanthanp/libfiglet/issues">Request Feature</a>
</p>

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project

This is Figlet Library for C++ to generate Large text out of ordinary text like this,

~~~Figlet
 _   _      _ _            ____            
| | | | ___| | | ___      / ___| _     _   
| |_| |/ _ \ | |/ _ \    | |   _| |_ _| |_ 
|  _  |  __/ | | (_) |   | |__|_   _|_   _|
|_| |_|\___|_|_|\___( )   \____||_|   |_|  
                    |/                    
~~~

<!-- GETTING STARTED -->
## Getting Started

### Project Scope

This Project currently only works With ASCII Characters and it currently does not support Unicode Characters (at least flf font parser).

### Installation

Starting with version 1.3.0 the library is no longer header-only But you generate the header file using quom and then you can use it in your project.

1. Clone the repo from the GitHub

~~~sh
git clone https://github.com/srilakshmikanthanp/libfiglet.git
~~~

2. Install the quom with pip

~~~sh
pip install --user quom
~~~

3. Generate Include Files

~~~sh
quom .\src\srilakshmikanthanp\libfiglet.hpp .\dist\libfiglet.hpp
~~~

4. Use the header file in your project

<!-- USAGE EXAMPLES -->
## Usage

Usage of This Library is very easy a Hello C++ would be,

~~~cpp
#include "libfiglet.hpp"
#include <iostream>

using namespace srilakshmikanthanp::libfiglet;

int main()
{
  const figlet figlet(flf_font::make_shared("./path/to/font"), smushed::make_shared());
  std::cout << figlet("Hello, C++");
}

~~~

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<!-- CONTACT -->
## Contact

Project Link: [https://github.com/srilakshmikanthanp/libfiglet](https://github.com/srilakshmikanthanp/libfiglet)
