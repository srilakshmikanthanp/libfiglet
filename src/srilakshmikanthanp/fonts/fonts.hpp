// Copyright (c) 2022 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Header Guard
#ifndef SRILAKSHMIKANTHANP_LIBFIGLET_FONTS_HPP
#define SRILAKSHMIKANTHANP_LIBFIGLET_FONTS_HPP

#include "../abstract/abstract.hpp"
#include "../utility/functions.hpp"
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
     * @brief Figlet flf Font Type
     */
    template <class string_type_t>
    class basic_flf_font : public basic_base_figlet_font<string_type_t>
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

    private:                                                              // Private types definition
      using map_type = std::map<char_type, fig_char_type>;
      using ibuff_it = std::istreambuf_iterator<char_type>;

    private:                                                              // Private configs
      char_type hard_blank;
      size_type height;
      shrink_type shrink;

    private:                                                              // Private characters
      map_type fig_chars;

    private:                                                              // Private utilities
      /**
       * @brief Read the Config from the stream
       */
      void read_config_and_remove_comments(istream_type &is)
      {
        // flf header line container
        string_type flf_config_line;

        // get the first line
        std::getline(is, flf_config_line);

        // string stream for the first line
        sstream_type ss(flf_config_line);

        // token
        string_type token;

        // Read header
        ss >> std::setw(5) >> token;

        // check
        if (token != cvt<string_type>("flf2a"))
        {
          throw std::runtime_error("Invalid flf2a header");
        }

        // Read hard blank
        ss >> this->hard_blank;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid hard blank");
        }

        // Read height
        ss >> this->height;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid height");
        }

        // Read baseline
        ss >> token;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid baseline");
        }

        // Read max length
        ss >> token;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid max length");
        }

        // Read old layout
        ss >> token;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid old layout");
        }

        // set shrink level
        const auto old_layout = std::stoi(token);

        if (old_layout < 0) // less than 0 then FULL_WIDTH
        {
          this->shrink = shrink_type::FULL_WIDTH;
        }

        if (old_layout == 0) // equal to 0 then KERNING
        {
          this->shrink = shrink_type::KERNING;
        }

        if (old_layout > 0) // greater than 0 then SMUSHED
        {
          this->shrink = shrink_type::SMUSHED;
        }

        // Read comment lines
        ss >> token;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid comment lines");
        }

        // check and set
        const auto comment_lines = std::stoi(token);

        // ignore comment lines
        for (auto i = 0; i < comment_lines; ++i)
        {
          std::getline(is, token);
        }
      }

      /**
       * @brief Read the characters from the stream
       */
      void read_chars(istream_type &is)
      {
        // lambda function to convert the lines to fig char
        auto to_fig_char = [this](const std::vector<string_type> &lines) {
          // regex pattern to match
          std::basic_regex<char_type> pattern(cvt<string_type>("(.)\\1?\n"), std::regex::ECMAScript);

          // fig char container
          fig_char_type fig_char;

          // line
          string_type line;

          // read lines
          for (const auto &line : lines)
          {
            fig_char.push_back(std::regex_replace(line + cvt<string_type>("\n"), pattern, cvt<string_type>("")));
          }

          // check height
          if (fig_char.size() != this->height)
          {
            throw std::runtime_error("Height not match");
          }

          // return
          return fig_char;
        };

        // lambda function to read lines from the stream
        auto read_lines = [this](istream_type &is, size_type n) {
          // lines container
          std::vector<string_type> lines;

          // line
          string_type line;

          // read lines
          for (auto i = 0; i < n && std::getline(is, line); ++i)
          {
            lines.push_back(line);
          }

          // return
          return lines;
        };

        // read all the characters (ch <= '~' must be first)
        for (char_type ch = ' '; ch <= '~'; ++ch)
        {
          // get the fig char
          const auto fig_char = to_fig_char(read_lines(is, this->height));

          // if length less
          if (fig_char.size() < this->height)
          {
            throw std::runtime_error("Invalid fig char");
          }

          // insert the fig char
          this->fig_chars.insert({ch, fig_char});
        }
      }

      /**
       * @brief Init from the Stream
       */
      void init(istream_type &is)
      {
        // read config and remove comments
        this->read_config_and_remove_comments(is);

        // read characters
        this->read_chars(is);
      }

    public:                                                               // Public constructors
      basic_flf_font() = delete;                                          // default constructor
      basic_flf_font(const basic_flf_font &) = default;                   // copy constructor
      basic_flf_font(basic_flf_font &&) = default;                        // move constructor

      /**
       * @brief From istream
       */
      explicit basic_flf_font(istream_type &is)
      {
        this->init(is);
      }

      /**
       * @brief From file
       */
      explicit basic_flf_font(const std::string &file)
      {
        // file stream
        ifstream_type ifs(file);

        // check
        if (!ifs.is_open())
        {
          throw std::runtime_error("Cannot open font : " + file);
        }

        // read
        this->init(ifs);
      }

    public: // Public overrides
      /**
       * @brief Get the Hard Blank character
       */
      char_type get_hard_blank() const override
      {
        return this->hard_blank;
      }

      /**
       * @brief Get the height of the font
       */
      size_type get_height() const override
      {
        return this->height;
      }

      /**
       * @brief Get the shrink level
       */
      shrink_type get_shrink_level() const override
      {
        return this->shrink;
      }

      /**
       * @brief Get the fig char
       */
      fig_char_type get_fig_char(char_type ch) const override
      {
        // check
        if (ch < ' ' || ch > '~')
        {
          throw std::runtime_error("Invalid character : " + std::to_string(ch));
        }

        // return
        return this->fig_chars.at(ch);
      }

    public: // static methods
      /**
       * @brief Make a flf font type as shared pointer
       */
      static auto make_shared(const std::string &file)
      {
        return std::make_shared<basic_flf_font>(file);
      }

      /**
       * @brief Make a flf font type as shared pointer
       */
      static auto make_shared(istream_type &is)
      {
        return std::make_shared<basic_flf_font>(is);
      }
    };
  }
}

#endif // BASIC_FIGLET_FIG_FONT_HPP
