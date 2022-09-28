// Copyright (c) 2022 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/**
 * @mainpage Figlet
 * @brief Generate large out of ordinary text
 * With C++ using this library
 * @author Sri Lakshmi Kanthan P
 */

#ifndef FIGLET_HPP
#define FIGLET_HPP

#include <algorithm>
#include <ostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>

namespace srilakshmikanthanp // Sri Lakshmi Kanthan P
{
  namespace libfiglet // Library for FIGlet
  {
    /**
     * @brief Some of the common typedefs for the Library
     */
    template <class string_type>
    struct types
    {
      using string_type = string_type;                         // String Type
      using char_type = typename string_type::value_type;      // Char Type
      using traits_type = typename string_type::traits_type;   // Traits Type
      using size_type = typename string_type::size_type;       // Size Type
      using fig_char_type = std::vector<string_type>;          // Figlet char
      using fig_str_type = std::vector<string_type>;           // Figlet String
      using ostream_type = std::basic_ostream<char_type>;      // Ostream Type
      using istream_type = std::basic_istream<char_type>;      // Istream Type
      using fstream_type = std::basic_fstream<char_type>;      // Fstream Type
      using sstream_type = std::basic_stringstream<char_type>; // Sstream Type
    };

    /**
     * @brief Shrink level of the Font
     */
    enum class shrink_type : size_t
    {
      UNTOUCHED = 0,
      Kerning = 1,
      SMUSHED = 2
    };

    /**
     * @brief Base Font Type for Figlet
     */
    template <class string_type>
    class basic_base_figlet_font
    {
      /**
       * @brief Get the Hard Blank character
       */
      virtual types<string_type>::string_type get_hard_blank() const = 0;

      /**
       * @brief Get the Height of font
       */
      virtual types<string_type>::size_type get_height() const = 0;

      /**
       * @brief Get the Shrink Level of font
       */
      virtual shrink_type get_shrink_level() const = 0;

      /**
       * @brief Get the BasicFiglet character
       */
      virtual types<string_type>::fig_char_type get_fig_char(char_type) const = 0;
    };

    /**
     * @brief Base Style Type for Figlet
     */
    template <class string_type>
    class basic_base_figlet_style
    {
      /**
       * @brief Get the Fig string
       */
      virtual types<string_type>::fig_str_type get_fig_str(
          std::vector<types<string_type>::fig_char_type> fig_chs,
          types<string_type>::char_type hardblank,
          types<string_type>::size_type height) const = 0;

      /**
       * @brief Get the Shrink Level
       */
      virtual shrink_type get_shrink_level() const = 0;
    };

    /**
     * @brief basic figlet class
     */
    template <class string_type>
    class basic_figlet
    {
    public: // Public types
      using base_figlet_style_ptr = std::shared_ptr<basic_base_figlet_style<string_type>>;
      using base_figlet_font_ptr = std::shared_ptr<basic_base_figlet_font<string_type>>;

    private:                       // Private members
      base_figlet_style_ptr style; // Figlet Style
      base_figlet_font_ptr font;   // Figlet Font

    private: // private utilities
      /**
       * @brief Check the shrink level, set font and style
       */
      void set_font_and_style(base_figlet_font_ptr font, base_figlet_style_ptr style)
      {
        if (font->get_shrink_level() < style->get_shrink_level())
        {
          throw std::runtime_error("Invalid shrink level");
        }

        this->style = style; // Set style
        this->font = font;   // Set font
      }

    public:                                        // Public members
      basic_figlet(const BasicFiglet &) = default; // copy constructor
      basic_figlet(BasicFiglet &&) = default;      // move constructor
      basic_figlet() = delete;                     // constructor

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
      types<string_type>::fig_str_type operator()(const types<string_type>::string_type &str) const
      {
        // Attributes for Transform
        std::vector<types<string_type>::fig_char_type> fig_chs;
        const auto hard_blank = this->font->get_hard_blank();
        const auto height = this->font->get_height();

        // Transform to fig char
        std::transform(str.begin(), str.end(), std::back_inserter(fig_chs), [this](auto ch)
                       { return this->font->get_fig_char(ch); });

        // Get the figlet string
        return this->style->get_fig_str(fig_chs, hard_blank, height);
      }
    };
  }
}

#endif
