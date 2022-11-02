// Copyright (c) 2022 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRILAKSHMIKANTHANP_LIBFIGLET_STYLES_HPP
#define SRILAKSHMIKANTHANP_LIBFIGLET_STYLES_HPP

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
     * @brief Figlet full width style
     */
    template <typename string_type_t>
    struct basic_full_width_style : public basic_base_figlet_style<string_type_t>
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
       * @brief get shrink level
       */
      shrink_type get_shrink_level() const override
      {
        return shrink_type::FULL_WIDTH;
      }

      /**
       * @brief get the fig str
       */
      fig_str_type get_fig_str(std::vector<fig_char_type> fig_chs) const override
      {
        // fig str container type
        fig_str_type fig_str(this->height);

        // for each fig char
        for (const auto &fig_char : fig_chs)
        {
          // check height
          if (fig_char.size() != this->height)
          {
            throw std::runtime_error("Invalid height");
          }

          // for each line
          for (size_type i = 0; i < this->height; ++i)
          {
            fig_str[i] += fig_char[i];
          }
        }

        // return
        return this->rm_hb(fig_str);
      }

    public: // static methods
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

    protected:                                                            // protected methods
      /**
       * @brief Trim deep the figlet string and char
       */
      void trim_deep(fig_str_type &fig_str, fig_char_type &fig_ch) const
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

          for (auto itr = fig_ch[i].begin(); itr != fig_ch[i].end(); ++itr)
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

          fig_ch[i].erase(0, siz);
        }
      }

    public:                                                               // Public overrides
      /**
       * @brief get shrink level
       */
      shrink_type get_shrink_level() const override
      {
        return shrink_type::KERNING;
      }

      /**
       * @brief get the fig str
       */
      fig_str_type get_fig_str(std::vector<fig_char_type> fig_chs) const override
      {
        // fig str container type
        fig_str_type fig_str(this->height);

        // for each fig char
        for (auto fig_ch : fig_chs)
        {
          // check height
          if (fig_ch.size() != this->height)
          {
            throw std::runtime_error("Invalid height");
          }

          // trim
          this->trim_deep(fig_str, fig_ch);

          // for each line
          for (size_type i = 0; i < this->height; ++i)
          {
            fig_str[i] += fig_ch[i];
          }
        }

        // return
        return this->rm_hb(fig_str);
      }

    public: // static methods
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
    public: // public type definition
      using string_type      =   string_type_t;                           // String Type
      using char_type        =   typename string_type_t::value_type;      // Character Type
      using traits_type      =   typename string_type_t::traits_type;     // Traits Type
      using size_type        =   typename string_type_t::size_type;       // Size Type

      using fig_char_type    =   std::vector<string_type_t>;              // Figlet char
      using fig_str_type     =   std::vector<string_type_t>;              // Figlet String

    private: // private typedefs
      using sstream_type     =   std::basic_stringstream<char_type>;      // Sstream Type
      using ostream_type     =   std::basic_ostream<char_type>;           // Ostream Type
      using istream_type     =   std::basic_istream<char_type>;           // Istream Type
      using ifstream_type    =   std::basic_ifstream<char_type>;          // Ifstream Type
      using ofstream_type    =   std::basic_ofstream<char_type>;          // Ofstream Type

    protected:                                                         // protected methods
      /**
       * @brief Smush Rules for the characters
       *
       * @param lc left character
       * @param rc right character
       * @return smushed character
       */
      auto smush_rules(char_type lc, char_type rc) const
      {
        //()
        if (lc == ' ')
        {
          return rc;
        }

        if (rc == ' ')
        {
          return lc;
        }

        //(Equal character smush)
        if (lc == rc)
        {
          return rc;
        }

        //(Underscores smush)
        if (lc == '_' && this->cvt("|/\\[]{}()<>").find(rc) != string_type::npos)
        {
          return rc;
        }

        if (rc == '_' && this->cvt("|/\\[]{}()<>").find(lc) != string_type::npos)
        {
          return lc;
        }

        //(Hierarchy Smushing)
        auto find_class = [](char_type ch)
        {
          if (ch == '|')
          {
            return 1;
          }

          if (ch == '/' || ch == '\\')
          {
            return 3;
          }

          if (ch == '[' || ch == ']')
          {
            return 4;
          }

          if (ch == '{' || ch == '}')
          {
            return 5;
          }

          if (ch == '(' || ch == ')')
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
        if (lc == '[' && rc == ']')
        {
          return '|';
        }

        if (lc == ']' && rc == '[')
        {
          return '|';
        }

        if (lc == '{' && rc == '}')
        {
          return '|';
        }

        if (lc == '}' && rc == '{')
        {
          return '|';
        }

        if (lc == '(' && rc == ')')
        {
          return '|';
        }

        if (lc == ')' && rc == '(')
        {
          return '|';
        }

        //(Big X smush)
        if (lc == '/' && rc == '\\')
        {
          return '|';
        }

        if (lc == '\\' && rc == '/')
        {
          return 'Y';
        }

        if (lc == '>' && rc == '<')
        {
          return 'X';
        }

        //(universal smush)
        return lc;
      }

      /**
       * @brief smush algorithm on kerned Fig string and character
       *
       * @param fig_str figlet string
       * @param fig_ch  figlet character
       */
      void do_smush(fig_str_type &fig_str, fig_char_type fig_ch) const
      {
        // trim the ends of the fig str and fig char
        this->trim_deep(fig_str, fig_ch);

        // is smushable
        bool smushable = true;

        // determine if smushable
        for (size_type i = 0; i < this->height; ++i)
        {
          if ((fig_str[i].back() == this->hard_blank) && !(fig_ch[i].front() == this->hard_blank))
          {
            smushable = false; break;
          }
          else if (fig_str[i].size() == 0 || fig_str[i].size() == 0)
          {
            smushable = false; break;
          }
        }

        // do the work
        if (smushable)
        {
          for (size_type i = 0; i < fig_str.size(); ++i)
          {
            fig_str[i].back() = this->smush_rules(fig_str[i].back(), fig_ch[i].front());
            fig_ch[i].erase(fig_ch[i].begin());
            fig_str[i] += fig_ch[i];
          }
        }
        else
        {
          for (size_type i = 0; i < fig_str.size(); ++i)
          {
            fig_str[i] += fig_ch[i];
          }
        }
      }

    public: // public methods
      /**
       * @brief Get the shrink level
       */
      shrink_type get_shrink_level() const override
      {
        return shrink_type::SMUSHED;
      }

      /**
       * @brief Get the Fig string
       */
      virtual fig_str_type get_fig_str(std::vector<fig_char_type> fig_chs) const
      {
        fig_str_type fig_str(this->height);

        for (auto &fig_char : fig_chs)
        {
          // check height
          if (fig_char.size() != this->height)
          {
            throw std::runtime_error("Invalid height");
          }

          // smush
          this->do_smush(fig_str, fig_char);
        }

        return this->rm_hb(fig_str);
      }

    public: // static methods
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
