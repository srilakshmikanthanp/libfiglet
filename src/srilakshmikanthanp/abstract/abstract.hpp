// Copyright (c) 2022 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Header Guards
#ifndef SRILAKSHMIKANTHANP_LIBFIGLET_ABSTRACT_HPP
#define SRILAKSHMIKANTHANP_LIBFIGLET_ABSTRACT_HPP

#include "../types/types.hpp"

#include <algorithm>
#include <ostream>
#include <string>
#include <map>
#include <iomanip>
#include <regex>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>

namespace srilakshmikanthanp
{
  namespace libfiglet
  {
    /**
     * @brief Base Font Type for Figlet
     */
    template <class string_type_t>
    struct basic_base_figlet_font
    {
    public:                                                               // public type definition
      using string_type      =   string_type_t;                           // String Type
      using char_type        =   typename string_type_t::value_type;      // Character Type
      using traits_type      =   typename string_type_t::traits_type;     // Traits Type
      using size_type        =   typename string_type_t::size_type;       // Size Type

      using fig_char_type    =   std::vector<string_type_t>;              // Figlet char
      using fig_str_type     =   std::vector<string_type_t>;              // Figlet String

    private:                                                              // private typedefs
      using sstream_type     =   std::basic_stringstream<char_type>;      // Sstream Type
      using ostream_type     =   std::basic_ostream<char_type>;           // Ostream Type
      using istream_type     =   std::basic_istream<char_type>;           // Istream Type
      using ifstream_type    =   std::basic_ifstream<char_type>;          // Ifstream Type
      using ofstream_type    =   std::basic_ofstream<char_type>;          // Ofstream Type

    public:                                                               // public methods

      /**
       * @brief Get the BasicFiglet character
       */
      virtual fig_char_type get_fig_char(char_type ch) const = 0;

      /**
       * @brief Get the Hard Blank character
       */
      virtual char_type get_hard_blank() const = 0;

      /**
       * @brief Get the Height of font
       */
      virtual size_type get_height() const = 0;

      /**
       * @brief Get the Shrink Level of font
       */
      virtual shrink_type get_shrink_level() const = 0;
    };

    /**
     * @brief Base Style Type for Figlet
     */
    template <class string_type_t>
    struct basic_base_figlet_style
    {
    public:                                                               // public type definition
      using string_type      =   string_type_t;                           // String Type
      using char_type        =   typename string_type_t::value_type;      // Character Type
      using traits_type      =   typename string_type_t::traits_type;     // Traits Type
      using size_type        =   typename string_type_t::size_type;       // Size Type

      using fig_char_type    =   std::vector<string_type_t>;              // Figlet char
      using fig_str_type     =   std::vector<string_type_t>;              // Figlet String

    private:                                                              // private typedefs
      using sstream_type     =   std::basic_stringstream<char_type>;      // Sstream Type
      using ostream_type     =   std::basic_ostream<char_type>;           // Ostream Type
      using istream_type     =   std::basic_istream<char_type>;           // Istream Type
      using ifstream_type    =   std::basic_ifstream<char_type>;          // Ifstream Type
      using ofstream_type    =   std::basic_ofstream<char_type>;          // Ofstream Type

    protected:                                                            // Protected Members
      char_type hard_blank;                                               // hard blank character of the Figlet
      size_type height;                                                   // height of the Figlet

    protected:                                                            // Protected Methods
      /**
       * @brief verify the height of the fig chars
       *
       * @param fig_chs fig characters
       */
      void verify_height(const std::vector<fig_char_type> &fig_chs) const
      {
        for(const auto &fig_ch: fig_chs)
        {
          if(fig_ch.size() != this->height)
          {
            throw std::runtime_error("Invalid Fig char Height");
          }
        }
      }

      /**
       * @brief removes hardblank from fig string
       *
       * @param figs fig string
       * @param hb hardblank
       */
      fig_str_type rm_hardblank(fig_str_type &figs) const
      {
        for (size_type i = 0; i < figs.size(); ++i)
        {
          std::replace(figs[i].begin(), figs[i].end(), this->hard_blank, traits_type::to_char_type(' '));
        }

        return figs;
      }

    public:
      basic_base_figlet_style(): hard_blank(' '), height(0) {}  // Default Constructor

      /**
       * @brief set the hard blank character of fig string
       */
      void set_hard_blank(char_type hb)
      {
        this->hard_blank = hb;
      }

      /**
       * @brief set the height of fig string
       */
      void set_height(size_type h)
      {
        this->height = h;
      }

      /**
       * @brief Get the Shrink Level
       */
      virtual shrink_type get_shrink_level() const = 0;

      /**
       * @brief Get the Fig string
       */
      virtual fig_str_type get_fig_str(std::vector<fig_char_type> fig_chs) const = 0;
    };
  }
}

#endif  // SRILAKSHMIKANTHANP_LIBFIGLET_ABSTRACT_HPP
