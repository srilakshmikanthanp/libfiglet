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
#include <iomanip>
#include <regex>
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
    template <class string_type_t>
    struct fig_types_t
    {
      using string_type = string_type_t;                         // String Type
      using char_type = typename string_type_t::value_type;    // Character Type
      using traits_type = typename string_type_t::traits_type; // Traits Type
      using size_type = typename string_type_t::size_type;     // Size Type

      using fig_char_type = std::vector<string_type_t>; // Figlet char
      using fig_str_type = std::vector<string_type_t>;  // Figlet String

      using ostream_type = std::basic_ostream<char_type>;      // Ostream Type
      using istream_type = std::basic_istream<char_type>;      // Istream Type
      using ifstream_type = std::basic_ifstream<char_type>;    // Ifstream Type
      using ofstream_type = std::basic_ofstream<char_type>;    // Ofstream Type
      using sstream_type = std::basic_stringstream<char_type>; // Sstream Type

      enum class shrink_type : size_type // Font Shrink
      {
        FULL_WIDTH,
        KERNING,
        SMUSHED
      };
    };

    /**
     * @brief Base Font Type for Figlet
     */
    template <class fig_types>
    struct basic_base_figlet_font
    {
      /**
       * @brief Get the Hard Blank character
       */
      virtual typename fig_types::char_type get_hard_blank() const = 0;

      /**
       * @brief Get the Height of font
       */
      virtual typename fig_types::size_type get_height() const = 0;

      /**
       * @brief Get the Shrink Level of font
       */
      virtual typename fig_types::shrink_type get_shrink_level() const = 0;

      /**
       * @brief Get the BasicFiglet character
       */
      virtual typename fig_types::fig_char_type get_fig_char(
          typename fig_types::char_type ch) const = 0;
    };

    /**
     * @brief Base Style Type for Figlet
     */
    template <class fig_types>
    struct basic_base_figlet_style
    {
      /**
       * @brief removes hardblank from fig string
       *
       * @param figs fig string
       * @param hb hardblank
       */
      typename fig_types::fig_str_type rm_hb(typename fig_types::fig_str_type &figs, typename fig_types::char_type hb) const
      {
        for (typename fig_types::size_type i = 0; i < figs.size(); ++i)
        {
          std::replace(figs[i].begin(), figs[i].end(), hb, ' ');
        }

        return figs;
      }

      /**
       * @brief convert ascii string to string_type
       *
       * @param str string to convert
       * @return string_type string converted
       */
      typename fig_types::string_type cvt(const std::string &str) const
      {
        return typename fig_types::string_type(str.begin(), str.end());
      }

      /**
       * @brief Get the Fig string
       */
      virtual typename fig_types::fig_str_type get_fig_str(
          std::vector<typename fig_types::fig_char_type> fig_chs,
          typename fig_types::char_type hardblank,
          typename fig_types::size_type height) const = 0;

      /**
       * @brief Get the Shrink Level
       */
      virtual typename fig_types::shrink_type get_shrink_level() const = 0;
    };

    /**
     * @brief basic figlet class
     */
    template <class fig_types>
    class basic_figlet
    {
    public: // Public types
      using base_figlet_style_ptr = std::shared_ptr<basic_base_figlet_style<fig_types>>;
      using base_figlet_font_ptr = std::shared_ptr<basic_base_figlet_font<fig_types>>;

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
      basic_figlet(const basic_figlet &) = default; // copy constructor
      basic_figlet(basic_figlet &&) = default;      // move constructor
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
      typename fig_types::string_type operator()(const typename fig_types::string_type &str) const
      {
        // Attributes for Transform
        std::vector<typename fig_types::fig_char_type> fig_chs;
        const auto hard_blank = this->font->get_hard_blank();
        const auto height = this->font->get_height();

        // Transform to fig char
        std::transform(
          str.begin(), str.end(), std::back_inserter(fig_chs), 
          [this](auto ch) { 
            return this->font->get_fig_char(ch); 
          }
        );

        // Get the figlet string
        const auto fig_str = this->style->get_fig_str(fig_chs, hard_blank, height);

        // add with new line
        typename fig_types::string_type value;

        for (const auto &fig : fig_str)
        {
          value += fig + '\n';
        }

        return value;
      }
    };

    /**
     * @brief Figlet flf Font Type
     */
    template <class fig_types>
    class basic_flf_font : public basic_base_figlet_font<fig_types>
    {
    private: // Private types definition
      using map_type = std::map<typename fig_types::char_type, typename fig_types::fig_char_type>;
      using isbuf_it = std::istreambuf_iterator<typename fig_types::char_type>;

    private: // Private configs
      typename fig_types::char_type hard_blank;
      typename fig_types::size_type height;
      typename fig_types::shrink_type shrink;

    private: // Private characters
      map_type fig_chars;

    private: // Private utilities
      /**
       * @brief Read the Config from the stream
       */
      void read_config_and_remove_comments(typename fig_types::istream_type &is)
      {
        // flf header line container
        typename fig_types::string_type flf_config_line;

        // get the first line
        std::getline(is, flf_config_line);

        // string stream for the first line
        typename fig_types::sstream_type ss(flf_config_line);

        // token
        typename fig_types::string_type token;

        // Read header
        ss >> std::setw(5) >> token;

        // check
        if (token != "flf2a")
        {
          throw std::runtime_error("Invalid flf header");
        }

        // Read hard blank
        ss >> this->hard_blank;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid flf header");
        }

        // Read height
        ss >> this->height;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid flf header");
        }

        // Read baseline
        ss >> token;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid flf header");
        }

        // Read max length
        ss >> token;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid flf header");
        }

        // Read old layout
        ss >> token;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid flf header");
        }

        // set shrink level
        const auto old_layout = std::stoi(token);

        if (old_layout < 0) // less than 0 then FULL_WIDTH
        {
          this->shrink = fig_types::shrink_type::FULL_WIDTH;
        }

        if (old_layout == 0) // equal to 0 then KERNING
        {
          this->shrink = fig_types::shrink_type::KERNING;
        }

        if (old_layout > 0) // greater than 0 then SMUSHED
        {
          this->shrink = fig_types::shrink_type::SMUSHED;
        }

        // Read comment lines
        ss >> token;

        // check and set
        const auto comment_lines = std::stoi(token);

        // ignore comment lines
        for (auto i = 0; i < comment_lines; ++i)
        {
          std::getline(is, token);
        }

        // Read print direction
        ss >> token;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid flf header");
        }

        // Read full layout
        ss >> token;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid flf header");
        }

        // Read codetag count
        ss >> token;

        // check stream
        if (ss.fail())
        {
          throw std::runtime_error("Invalid flf header");
        }
      }

      /**
       * @brief string to fig char
       */
      typename fig_types::fig_char_type str_to_fig_char(const typename fig_types::string_type &str) const
      {
        // fig char container
        typename fig_types::fig_char_type fig_char;

        // string stream
        typename fig_types::sstream_type ss(str);

        // line
        typename fig_types::string_type line;

        // read lines
        while (std::getline(ss, line))
        {
          // pop the last char
          line.pop_back();

          // push the line
          fig_char.push_back(line);
        }

        // check height
        if (fig_char.size() != this->height)
        {
          throw std::runtime_error("Invalid flf font");
        }

        // return
        return fig_char;
      }

      /**
       * @brief Read the characters from the stream
       */
      void read_chars(typename fig_types::istream_type &is)
      {
        // Regular expression for flf char
        std::regex pattern("(.*(?=[@#])[\\s\\S]*?[@#](?=[@#]))", std::regex_constants::ECMAScript);

        // Read the whole file
        const auto file_str = typename fig_types::string_type(isbuf_it(is), isbuf_it());

        // get all the matches
        std::match_results<typename fig_types::string_type::const_iterator> res;

        // start char
        typename fig_types::char_type ch = ' ';

        // start
        auto start = file_str.cbegin();

        // end
        auto end = file_str.cend();

        // read all the characters
        for (ch = ' '; std::regex_search(start, end, res, pattern) && ch <= '~'; ++ch)
        {
          // get the fig char
          const auto fig_char = this->str_to_fig_char(res[0].str());

          // insert the fig char
          this->fig_chars.insert({ch, fig_char});

          // update start
          start = res.suffix().first + 1;
        }

        // check
        if (--ch != '~')
        {
          throw std::runtime_error("Invalid flf characters");
        }
      }

    public:                                             // Public constructors
      basic_flf_font() = delete;                        // default constructor
      basic_flf_font(const basic_flf_font &) = default; // copy constructor
      basic_flf_font(basic_flf_font &&) = default;      // move constructor

      /**
       * @brief From istream
       */
      explicit basic_flf_font(typename fig_types::istream_type &is)
      {
        // read config and remove comments
        this->read_config_and_remove_comments(is);

        // read characters
        this->read_chars(is);
      }

      /**
       * @brief From file
       */
      explicit basic_flf_font(const typename fig_types::string_type &file)
      {
        // file stream
        typename fig_types::ifstream_type ifs(file);

        // check
        if (!ifs.is_open())
        {
          throw std::runtime_error("Cannot open file");
        }

        // read config and remove comments
        this->read_config_and_remove_comments(ifs);

        // read characters
        this->read_chars(ifs);
      }

    public: // Public overrides
      /**
       * @brief Get the Hard Blank character
       */
      typename fig_types::char_type get_hard_blank() const override
      {
        return this->hard_blank;
      }

      /**
       * @brief Get the height of the font
       */
      typename fig_types::size_type get_height() const override
      {
        return this->height;
      }

      /**
       * @brief Get the shrink level
       */
      typename fig_types::shrink_type get_shrink_level() const override
      {
        return this->shrink;
      }

      /**
       * @brief Get the fig char
       */
      typename fig_types::fig_char_type get_fig_char(typename fig_types::char_type ch) const override
      {
        // check
        if (ch < ' ' || ch > '~')
        {
          throw std::runtime_error("Invalid character");
        }

        // return
        return this->fig_chars.at(ch);
      }

    public: // static methods

      /**
       * @brief Make a flf font type as shared pointer
       */
      static typename basic_figlet<fig_types>::base_figlet_font_ptr 
      make_shared(const typename fig_types::string_type &file)
      {
        return std::make_shared<basic_flf_font>(file);
      }

      /**
       * @brief Make a flf font type as shared pointer
       */
      static typename basic_figlet<fig_types>::base_figlet_font_ptr  
      make_shared(typename fig_types::istream_type &is)
      {
        return std::make_shared<basic_flf_font>(is);
      }
    };

    /**
     * @brief Figlet full width style
     */
    template <typename fig_types>
    struct basic_full_width_style : public basic_base_figlet_style<fig_types>
    {
      /**
       * @brief get shrink level
       */
      typename fig_types::shrink_type get_shrink_level() const override
      {
        return fig_types::shrink_type::FULL_WIDTH;
      }

      /**
       * @brief get the fig str
       */
      typename fig_types::fig_str_type get_fig_str(
          std::vector<typename fig_types::fig_char_type> fig_chs,
          typename fig_types::char_type hardblank,
          typename fig_types::size_type height) const override
      {
        // fig str container type
        typename fig_types::fig_str_type fig_str(height);

        // for each fig char
        for (const auto &fig_char : fig_chs)
        {
          // check height
          if (fig_char.size() != height)
          {
            throw std::runtime_error("Invalid fig char height");
          }

          // size type
          using size_type = typename fig_types::size_type;

          // for each line
          for (size_type i = 0; i < height; ++i)
          {
            fig_str[i] += fig_char[i];
          }
        }

        // return
        return this->rm_hb(fig_str, hardblank);
      }

    public: // static methods

      /**
       * @brief Make a full width style as shared pointer
       */
      static typename basic_figlet<fig_types>::base_figlet_style_ptr
      make_shared()
      {
        return std::make_shared<basic_full_width_style>();
      }
    };
  } // namespace libfiglet
} // namespace srilakshmikanthanp

/**
 * @brief Default figlet types
 */
namespace srilakshmikanthanp
{
  namespace libfiglet
  {
    // std::string type for figlet
    using s_fig_type = fig_types_t<std::string>;

    // std::wstring type for figlet
    using w_fig_type = fig_types_t<std::wstring>;
  } // namespace libfiglet
} // namespace srilakshmikanthanp

/**
 * @brief std::string types
 */
namespace srilakshmikanthanp
{
  namespace libfiglet
  {
    // std::string type for flf font
    using flf_font = basic_flf_font<s_fig_type>;

    // std::string type for figlet
    using figlet = basic_figlet<s_fig_type>;

    // std::string type for full width style
    using full_width = basic_full_width_style<s_fig_type>;
  } // namespace libfiglet
} // namespace srilakshmikanthanp

/**
 * @brief std::wstring types
 */
namespace srilakshmikanthanp
{
  namespace libfiglet
  {
    // std::wstring type for flf font
    using wflf_font = basic_flf_font<w_fig_type>;

    // std::wstring type for figlet
    using wfiglet = basic_figlet<w_fig_type>;

    // std::wstring type for full width style
    using wfull_width = basic_full_width_style<w_fig_type>;
  } // namespace libfiglet
} // namespace srilakshmikanthanp

#endif
