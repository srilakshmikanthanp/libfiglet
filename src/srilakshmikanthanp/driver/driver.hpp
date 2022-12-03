// Copyright (c) 2022 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Header Guards
#ifndef SRILAKSHMIKANTHANP_LIBFIGLET_DRIVER_HPP
#define SRILAKSHMIKANTHANP_LIBFIGLET_DRIVER_HPP

#include "../abstract/abstract.hpp"
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
     * @brief basic figlet class
     */
    template <class string_type_t>
    class basic_figlet
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

    public:                                                               // Public types
      using base_figlet_style_ptr  =  std::shared_ptr<basic_base_figlet_style<string_type>>;
      using base_figlet_font_ptr   =  std::shared_ptr<basic_base_figlet_font<string_type>>;

    private:                                                              // Private members
      base_figlet_style_ptr style;                                        // Figlet Style
      base_figlet_font_ptr font;                                          // Figlet Font

    private:                                                              // private utilities
      /**
       * @brief Check the shrink level, set font and style
       */
      void set_font_and_style(base_figlet_font_ptr font, base_figlet_style_ptr style)
      {
        if (font->get_shrink_level() < style->get_shrink_level())
        {
          throw std::runtime_error("The Shrink Level of Font is less than Style");
        }

        this->style = style; // Set style
        this->font = font;   // Set font

        // Set the height of the style
        this->style->set_height(this->font->get_height());

        // Set the hard blank of the style
        this->style->set_hard_blank(this->font->get_hard_blank());
      }

    public:                                                             // Public members
      basic_figlet(const basic_figlet &) = default;                     // copy constructor
      basic_figlet(basic_figlet &&) = default;                          // move constructor
      basic_figlet() = delete;                                          // constructor

      /**
       * @brief Construct a new basic figlet object
       */
      basic_figlet(base_figlet_font_ptr font, base_figlet_style_ptr style)
      {
        this->set_font_and_style(font, style);
      }

      /**
       * @brief set the style
       */
      void set_style(base_figlet_style_ptr style)
      {
        this->set_font_and_style(this->font, style);
      }

      /**
       * @brief set the font
       */
      void set_font(base_figlet_font_ptr font)
      {
        this->set_font_and_style(font, this->style);
      }

      /**
       * @brief Get the style
       */
      base_figlet_style_ptr get_style() const
      {
        return this->style;
      }

      /**
       * @brief Get the font
       */
      base_figlet_font_ptr get_font() const
      {
        return this->font;
      }

      /**
       * @brief Get the figlet string
       */
      string_type operator()(const string_type &str) const
      {
        // Attributes for Transform
        std::vector<fig_char_type> fig_chs;
        const auto hard_blank = this->font->get_hard_blank();
        const auto height = this->font->get_height();

        // Transform to fig char
        std::transform(
          str.begin(), str.end(), std::back_inserter(fig_chs),
          [this](auto ch){
            return this->font->get_fig_char(ch);
          }
        );

        // Get the figlet string
        const auto fig_str = this->style->get_fig_str(fig_chs);

        // add with new line
        string_type value;

        for (const auto &fig : fig_str)
        {
          value += fig + traits_type::to_char_type('\n');
        }

        return value;
      }
    };
  } // namespace libfiglet
} // namespace srilakshmikanthanp

#endif
