// Copyright (c) 2022 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Header Guard
#ifndef SRILAKSHMIKANTHANP_LIBFIGLET_LIBFIGLET_HPP
#define SRILAKSHMIKANTHANP_LIBFIGLET_LIBFIGLET_HPP

#include "./abstract/abstract.hpp"
#include "./constants/constants.hpp"
#include "./driver/driver.hpp"
#include "./fonts/fonts.hpp"
#include "./styles/styles.hpp"
#include "./types/types.hpp"
#include "./utility/functions.hpp"

namespace srilakshmikanthanp
{
  namespace libfiglet
  {
    // Styles for the Figlet using std::string
    using full_width  =   basic_full_width_style<std::string>;
    using kerning     =   basic_kerning_style<std::string>;
    using smushed     =   basic_smushed_style<std::string>;

    // flf Font Parser using std::string
    using flf_font    =   basic_flf_font<std::string>;

    // Figlet Driver using std::string
    using figlet      =   basic_figlet<std::string>;

    // Styles for the Figlet using std::wstring
    using wfull_width =   basic_full_width_style<std::wstring>;
    using wkerning    =   basic_kerning_style<std::wstring>;
    using wsmushed    =   basic_smushed_style<std::wstring>;

    // flf Font Parser using std::wstring
    using wflf_font   =   basic_flf_font<std::wstring>;

    // Figlet Driver using std::wstring
    using wfiglet     =   basic_figlet<std::wstring>;
  }
}

#endif // SRILAKSHMIKANTHANP_LIBFIGLET_LIBFIGLET_HPP
