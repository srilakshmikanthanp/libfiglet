<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
***
***
***
*** To avoid retyping too much info. Do a search and replace for the following:
*** srilakshmikanthanp, Figlet, twitter_handle, email, project_title, project_description
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/srilakshmikanthanp/Figlet">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">project_title</h3>

  <p align="center">
    project_description
    <br />
    <a href="https://github.com/srilakshmikanthanp/Figlet"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/srilakshmikanthanp/Figlet">View Demo</a>
    ·
    <a href="https://github.com/srilakshmikanthanp/Figlet/issues">Report Bug</a>
    ·
    <a href="https://github.com/srilakshmikanthanp/Figlet/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This is Figlet Library for C++ to generate Large text like this,

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

### Installation

This is header only library so just take `Figlet.hpp` and use.

<!-- USAGE EXAMPLES -->
## Usage

Usage of This Library is vera easy a Hello world would be,

~~~cpp
#include <iostream>
#include "../include/srilakshmikanthanp/Figlet.hpp"

using namespace srilakshmikanthanp::Figlet::Char;

int main()
{
    Figlet figlet(FigletFont::make("Fonts/Standard.flf"), Smushed::make());

    std::cout << figlet("Hello, C++");

    return 0;
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

Project Link: [https://github.com/srilakshmikanthanp/Figlet](https://github.com/srilakshmikanthanp/Figlet)
