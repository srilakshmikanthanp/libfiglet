// Copyright (c) 2022 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRILAKSHMIKANTHANP_LIBFIGLET_STYLES_HPP
#define SRILAKSHMIKANTHANP_LIBFIGLET_STYLES_HPP

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
     * @brief Figlet full width style
     */
    template <typename string_type_t>
    struct basic_full_width_style : public basic_base_figlet_style<string_type_t>
    {
    public:                                                         // public type definition
      using string_type   = string_type_t;                          // String Type
      using char_type     = typename string_type_t::value_type;     // Character Type
      using traits_type   = typename string_type_t::traits_type;    // Traits Type
      using size_type     = typename string_type_t::size_type;      // Size Type

      using fig_char_type = std::vector<string_type_t>;             // Figlet char
      using fig_str_type  = std::vector<string_type_t>;             // Figlet String

    private:                                                        // private typedefs
      using sstream_type  = std::basic_stringstream<char_type>;     // Sstream Type
      using ostream_type  = std::basic_ostream<char_type>;          // Ostream Type
      using istream_type  = std::basic_istream<char_type>;          // Istream Type
      using ifstream_type = std::basic_ifstream<char_type>;         // Ifstream Type
      using ofstream_type = std::basic_ofstream<char_type>;         // Ofstream Type

    protected:                                                      // Protected methods
      /**
       * @brief Add the Fig String and the Figlet Char
       */
      void add_fig_str_and_fig_char(fig_str_type& fig_str, const fig_char_type& fig_char) const
      {
        for (size_type i = 0; i < fig_char.size(); ++i)
        {
          fig_str[i] += fig_char[i];
        }
      }

    public:                                                         // public methods
      /**
       * @brief get the fig str
       */
      fig_str_type get_fig_str(std::vector<fig_char_type> fig_chrs) const override
      {
        // fig str container type
        fig_str_type fig_str(this->height);

        // verify height
        this->verify_height(fig_chrs);

        // for each fig char
        for (auto &fig_chr : fig_chrs)
        {
          this->add_fig_str_and_fig_char(fig_str, fig_chr);
        }

        // return
        return this->rm_hardblank(fig_str);
       }

       /**
        * @brief get shrink level
        */
       shrink_type get_shrink_level() const override
       {
         return shrink_type::FULL_WIDTH;
       }

     public:                                                        // static methods
      /**
       * @brief Make a full width style as shared pointer
       */
       static auto make_shared()
       {
         return std::make_shared<basic_full_width_style>();
       }
    };

    /**
      * @brief Figlet kerning style
      */
    template <typename string_type_t>
    struct basic_kerning_style : public basic_full_width_style<string_type_t>
    {
    public:                                                          // public type definition
      using string_type   = string_type_t;                           // String Type
      using char_type     = typename string_type_t::value_type;      // Character Type
      using traits_type   = typename string_type_t::traits_type;     // Traits Type
      using size_type     = typename string_type_t::size_type;       // Size Type

      using fig_char_type = std::vector<string_type_t>;              // Figlet char
      using fig_str_type  = std::vector<string_type_t>;              // Figlet String

    private:                                                         // private typedefs
      using sstream_type  = std::basic_stringstream<char_type>;      // Sstream Type
      using ostream_type  = std::basic_ostream<char_type>;           // Ostream Type
      using istream_type  = std::basic_istream<char_type>;           // Istream Type
      using ifstream_type = std::basic_ifstream<char_type>;          // Ifstream Type
      using ofstream_type = std::basic_ofstream<char_type>;          // Ofstream Type

    protected:                                                       // protected methods
      /**
       * @brief Trim deep the figlet string and char
       */
      void trim_fig_str_and_fig_char(fig_str_type &fig_str, fig_char_type &fig_chr) const
      {
        // left spaces and right spaces
        std::vector<size_type> elem;

        // count space
        for (size_type i = 0; i < fig_str.size(); ++i)
        {
          int l_count = 0, r_count = 0;

          for (auto itr = fig_str[i].rbegin(); itr != fig_str[i].rend(); ++itr)
          {
            if (*itr == ' ')
              ++l_count;
            else
              break;
          }

          for (auto itr = fig_chr[i].begin(); itr != fig_chr[i].end(); ++itr)
          {
            if (*itr == ' ')
              ++r_count;
            else
              break;
          }

          elem.push_back(l_count + r_count);
        }

        // minimum
        const auto min = *std::min_element(elem.begin(), elem.end());

        // for each line
        for (size_type i = 0; i < fig_str.size(); ++i)
        {
          size_type siz = min + 1;

          while (--siz > 0 && fig_str[i].back() == ' ')
          {
            fig_str[i].pop_back();
          }

          fig_chr[i].erase(0, siz);
        }
      }

    public:                                                          // Public overrides
      /**
       * @brief get the fig str
       */
      fig_str_type get_fig_str(std::vector<fig_char_type> fig_chrs) const override
      {
        // fig str container type
        fig_str_type fig_str(this->height);

        // verify height
        this->verify_height(fig_chrs);

        // for each fig char
        for (auto &fig_chr : fig_chrs)
        {
          this->trim_fig_str_and_fig_char(fig_str, fig_chr);
          this->add_fig_str_and_fig_char(fig_str, fig_chr);
        }

        // return
        return this->rm_hardblank(fig_str);
      }

      /**
       * @brief get shrink level
       */
      shrink_type get_shrink_level() const override
      {
        return shrink_type::KERNING;
      }

    public:                                                           // static methods
      /**
       * @brief Make a kerning style as shared pointer
       */
      static auto make_shared()
      {
        return std::make_shared<basic_kerning_style>();
      }
    };

    /**
     * @brief Figlet smushed style
     */
    template <typename string_type_t>
    struct basic_smushed_style : public basic_kerning_style<string_type_t>
    {
    public:                                                          // public type definition
      using string_type   = string_type_t;                           // String Type
      using char_type     = typename string_type_t::value_type;      // Character Type
      using traits_type   = typename string_type_t::traits_type;     // Traits Type
      using size_type     = typename string_type_t::size_type;       // Size Type

      using fig_char_type = std::vector<string_type_t>;              // Figlet char
      using fig_str_type  = std::vector<string_type_t>;              // Figlet String

    private:                                                         // private typedefs
      using sstream_type  = std::basic_stringstream<char_type>;      // Sstream Type
      using ostream_type  = std::basic_ostream<char_type>;           // Ostream Type
      using istream_type  = std::basic_istream<char_type>;           // Istream Type
      using ifstream_type = std::basic_ifstream<char_type>;          // Ifstream Type
      using ofstream_type = std::basic_ofstream<char_type>;          // Ofstream Type

    protected:                                                       // protected methods
      /**
       * @brief Smush Rules for the characters
       */
      auto smush_rules(char_type lc, char_type rc) const
      {
        //()
        if (lc == traits_type::to_char_type(' '))
        {
          return rc;
        }

        if (rc == traits_type::to_char_type(' '))
        {
          return lc;
        }

        //(Equal character smush)
        if (lc == rc)
        {
          return rc;
        }

        //(Underscores smush)
        if (lc == traits_type::to_char_type('_') && cvt<string_type>("|/\\[]{}()<>").find(rc) != string_type::npos)
        {
          return rc;
        }

        if (rc == traits_type::to_char_type('_') && cvt<string_type>("|/\\[]{}()<>").find(lc) != string_type::npos)
        {
          return lc;
        }

        //(Hierarchy Smushing)
        auto find_class = [](char_type ch)
        {
          if (ch == traits_type::to_char_type('|'))
          {
            return 1;
          }

          if (ch == traits_type::to_char_type('/') || ch == traits_type::to_char_type('\\'))
          {
            return 3;
          }

          if (ch == traits_type::to_char_type('[') || ch == traits_type::to_char_type(']'))
          {
            return 4;
          }

          if (ch == traits_type::to_char_type('{') || ch == traits_type::to_char_type('}'))
          {
            return 5;
          }

          if (ch == traits_type::to_char_type('(') || ch == traits_type::to_char_type(')'))
          {
            return 6;
          }

          return 0;
        };

        size_type c_lc = find_class(lc);
        size_type c_rc = find_class(rc);

        if (c_lc > c_rc)
        {
          return lc;
        }

        if (c_rc > c_lc)
        {
          return rc;
        }

        //(Opposite smush)
        if (lc == traits_type::to_char_type('[')  &&  rc == traits_type::to_char_type(']'))
        {
          return traits_type::to_char_type('|');
        }

        if (lc == traits_type::to_char_type(']')  &&  rc == traits_type::to_char_type('['))
        {
          return traits_type::to_char_type('|');
        }

        if (lc == traits_type::to_char_type('{')  &&  rc == traits_type::to_char_type('}'))
        {
          return traits_type::to_char_type('|');
        }

        if (lc == traits_type::to_char_type('}')  &&  rc == traits_type::to_char_type('{'))
        {
          return traits_type::to_char_type('|');
        }

        if (lc == traits_type::to_char_type('(')  &&  rc == traits_type::to_char_type(')'))
        {
          return traits_type::to_char_type('|');
        }

        if (lc == traits_type::to_char_type(')')  &&  rc == traits_type::to_char_type('('))
        {
          return traits_type::to_char_type('|');
        }

        //(Big X smush)
        if (lc == traits_type::to_char_type('/')  &&  rc == traits_type::to_char_type('\\'))
        {
          return traits_type::to_char_type('|');
        }

        if (lc == traits_type::to_char_type('\\') &&  rc == traits_type::to_char_type('/'))
        {
          return traits_type::to_char_type('Y');
        }

        if (lc == traits_type::to_char_type('>')  &&  rc == traits_type::to_char_type('<'))
        {
          return traits_type::to_char_type('X');
        }

        //(universal smush)
        return lc;
      }

      /**
       * @brief smush algorithm on kerned Fig string and character
       */
      void smush_fig_str_and_fig_char(fig_str_type &fig_str, fig_char_type fig_chr) const
      {
        // determine if smushable if not the just add and return
        for (size_type i = 0; i < this->height; ++i)
        {
          if ((fig_str[i].back() == this->hard_blank) && !(fig_chr[i].front() == this->hard_blank))
          {
            return this->add_fig_str_and_fig_char(fig_str, fig_chr);
          }
          else if (fig_str[i].size() == 0 || fig_str[i].size() == 0)
          {
            return this->add_fig_str_and_fig_char(fig_str, fig_chr);
          }
        }

        // smush the fig str and fig char
        for (size_type i = 0; i < fig_str.size(); ++i)
        {
          fig_str[i].back() = this->smush_rules(fig_str[i].back(), fig_chr[i].front());
          fig_chr[i].erase(fig_chr[i].begin());
        }

        // Add the fig char to the fig str
        this->add_fig_str_and_fig_char(fig_str, fig_chr);
      }

    public:                                                            // public methods
      /**
       * @brief Get the Fig string
       */
      fig_str_type get_fig_str(std::vector<fig_char_type> fig_chrs) const
      {
        // fig str container type
        fig_str_type fig_str(this->height);

        // verify the height
        this->verify_height(fig_chrs);

        // smush the chars
        for (auto &fig_char : fig_chrs)
        {
          this->trim_fig_str_and_fig_char(fig_str, fig_char);
          this->smush_fig_str_and_fig_char(fig_str, fig_char);
        }

        // remove hardblank
        return this->rm_hardblank(fig_str);
      }

      /**
       * @brief Get the shrink level
       */
      shrink_type get_shrink_level() const override
      {
        return shrink_type::SMUSHED;
      }

    public:                                                                // static methods
      /**
       * @brief Make a kerning style as shared pointer
       */
      static auto make_shared()
      {
        return std::make_shared<basic_smushed_style>();
      }
    };
  }
}

#endif // SRILAKSHMIKANTHANP_LIBFIGLET_STYLES_HPP
