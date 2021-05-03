// Copyright (c) 2021 Sri Lakshmi Kanthan P
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

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

/**
 * @brief Figlet namespace 
 */
namespace srilakshmikanthanp::Figlet
{
    /**
     * @brief Font
     */
    template <class StringType>
    struct Font
    {
        using string_type = StringType;
        using char_type = typename string_type::value_type;
        using traits_type = typename string_type::traits_type;
        using size_type = typename string_type::size_type;
        using Figc_type = std::vector<string_type>;
        using Figs_type = std::vector<string_type>;

        /**
         * @brief Get the Hard Blank character
         * 
         * @return char_type hardblank
         */
        virtual char_type getHardBlank() const = 0;

        /**
         * @brief Get the Height
         * 
         * @return size_type height
         */
        virtual size_type getHeight() const = 0;

        /**
         * @brief Get the Shrink
         * 
         * @return size_type shrink value
         * 0 - untouched
         * 1 - kerning
         * 2 - smushed
         */
        virtual size_type getShrink() const = 0;

        /**
         * @brief Get the Figlet character
         * 
         * @return Figc_type Figlet character
         */
        virtual Figc_type getFigc(char_type) const = 0;
    };

    /**
     * @brief Styles
     */
    template <class StringType>
    struct Style
    {
        using string_type = StringType;
        using char_type = typename string_type::value_type;
        using traits_type = typename string_type::traits_type;
        using size_type = typename string_type::size_type;
        using Figc_type = std::vector<string_type>;
        using Figs_type = std::vector<string_type>;

        /**
         * @brief Get the Shrinl Level
         * 
         * @return size_type
         * 0 - untouched
         * 1 - kerning
         * 2 - smushed
         */
        virtual size_type getShrinkLevel() const = 0;

        /**
         * @brief Get the Fig string
         * 
         * @param figchs figlet characters
         * @param hardblank hardblank
         * @param height height
         * @param shirnk shrinkklevel
         * @return Figs_type Fig string
         */
        virtual Figs_type getFigs (
            std::vector<Figc_type> figchs,
            char_type hardblank,
            size_type height,
            size_type shirnk
        ) const = 0;
    };

    /**
     * @brief class Figlet
     */
    template <class StringType>
    class Figlet
    {
    public:
        using string_type = StringType;
        using char_type = typename string_type::value_type;
        using traits_type = typename string_type::traits_type;
        using size_type = typename string_type::size_type;
        using Figc_type = std::vector<string_type>;
        using Figs_type = std::vector<string_type>;

    private:
        using ostream_type = std::basic_ostream<char_type, traits_type>;
        using base_font = std::shared_ptr<Font<string_type>>;
        using base_style = std::shared_ptr<Style<string_type>>;

    private:
        base_font font;
        base_style style;
        string_type str;

    public:
        Figlet() = delete;
        Figlet(const Figlet &) = default;
        Figlet(Figlet &&) = default;

        /**
         * @brief Construct a new Figlet object
         * 
         * @param font font type
         * @param style style type
         */
        Figlet(base_font font, base_style style)
            : font(font), style(style)
        {
            if (font->getShrink() < style->getShrinkLevel())
            {
                throw std::runtime_error("style not supported");
            }
        }

        /**
         * @brief Set the Font object
         * 
         * @param font 
         */
        void setFont(base_font font)
        {
            this->font = font;
        }

        /**
         * @brief Set the Style object
         * 
         * @return style 
         */
        void setStyle(base_style style)
        {
            this->style = style;
        }

        /**
         * @brief Get the Font object
         * 
         * @return font 
         */
        base_font &getFont()
        {
            return this->font;
        }

        /**
         * @brief Get the Style object
         * 
         * @param style 
         */
        base_style &getStyle()
        {
            return this->style;
        }

        /**
         * @brief set stringto figlet
         * 
         * @param str string 
         * @return Figlet& *this
         */
        Figlet &operator()(const string_type &str)
        {
            this->str = str;
            return *this;
        }

        /**
         * @brief operator << printer
         * 
         * @param os ostream
         * @param figlet figlet
         * @return ostream_type& os
         */
        friend ostream_type &operator<<(ostream_type &os, Figlet &figlet)
        {
            std::vector<Figc_type> figchars;
            char_type hb = figlet.font->getHardBlank();
            size_type hg = figlet.font->getHeight();
            size_type sk = figlet.font->getShrink();

            for (auto ch : figlet.str)
            {
                figchars.push_back(figlet.font->getFigc(ch));
            }

            Figs_type figs = figlet.style->getFigs(figchars, hb, hg, sk);

            for (const auto &str : figs)
            {
                os << str << "\n";
            }

            return os;
        }
    };

    /**
     * @brief FigletFont
     * 
     * @tparam StringType 
     */
    template <class StringType>
    class FigletFont : public Font<StringType>
    {
    public:
        using string_type = StringType;
        using char_type = typename string_type::value_type;
        using traits_type = typename string_type::traits_type;
        using size_type = typename string_type::size_type;
        using Figc_type = std::vector<string_type>;
        using Figs_type = std::vector<string_type>;

    private:
        char_type HardBlank;
        size_type Height;
        size_type Shrink;
        std::map<char_type, Figc_type> Figch;

    private:
        /**
         * @brief converts ascii str to string_type
         * @param str string to convert
         * @return string_type converted string
         **/
        string_type cvt(const std::string &str)
        {
            return string_type{str.begin(), str.end()};
        }

        /**
         * @brief reads settings
         * 
         * @param stream 
         */
        void start(std::basic_istream<char_type, traits_type> &stream)
        {
            string_type str;
            size_type dummy;
            size_type cmnt;
            int old_tag;

            stream >> str;

            if (str.substr(0, 5) != cvt("flf2a"))
            {
                throw std::runtime_error("flf2a not found !");
            }

            str.erase(0, 5);

            this->HardBlank = str.at(0); // HardBlank

            stream >> this->Height; // Height
            stream >> dummy;
            stream >> dummy;
            stream >> old_tag;
            stream >> cmnt; // comment line

            std::getline(stream, str);

            for (size_type i = 0; i < cmnt; i++)
            {
                std::getline(stream, str);
            }

            if (old_tag < 0)
            {
                this->Shrink = 0;
            }
            else if (old_tag == 0)
            {
                this->Shrink = 1;
            }
            else
            {
                this->Shrink = 2;
            }
        }

        /**
         * @brief next character from font
         * 
         * @param stream stream
         * @return Figc_type figlt character
         */
        Figc_type next(std::basic_istream<char_type, traits_type> &stream)
        {
            std::vector<string_type> ret;

            for (size_type i = 1; i <= this->Height; ++i)
            {
                string_type str;
                std::getline(stream, str);

                if (str.back() == '\n' || str.back() == '\r')
                {
                    str.pop_back();
                }

                str.pop_back();
                ret.push_back(str);

                if (i == this->Height)
                {
                    ret.back().pop_back();
                }
            }

            return ret;
        }

        void read(std::basic_istream<char_type, traits_type> &stream)
        {
            this->start(stream);

            for (char_type ch = ' '; ch <= '~'; ch++)
            {
                this->Figch[ch] = this->next(stream);
            }
        }

    public:
        FigletFont() = delete;
        FigletFont(const FigletFont &) = default;
        FigletFont(FigletFont &&) = default;

        /**
         * @brief Construct a new Figlet Font
         * 
         * @param font 
         */
        FigletFont(string_type font)
        {
            std::basic_istringstream<char_type, traits_type> sstream(font);
            std::basic_ifstream<char_type, traits_type> stream(font);

            if (stream.is_open())
            {
                this->read(stream);
            }
            else
            {
                this->read(sstream);
            }
        }

        /**
         * @brief Get the Hard Blank character
         * 
         * @return char_type hardblank
         */
        char_type getHardBlank() const override
        {
            return this->HardBlank;
        }

        /**
         * @brief Get the Height
         * 
         * @return size_type height
         */
        size_type getHeight() const override
        {
            return this->Height;
        }

        /**
         * @brief Get the Shrink
         * 
         * @return size_type shrink value
         * 0 - untouched
         * 1 - kerning
         * 2 - smushed
         */
        size_type getShrink() const override
        {
            return this->Shrink;
        }

        /**
         * @brief Get the Figlet character
         * 
         * @return Figc_type Figlet character
         */
        Figc_type getFigc(char_type ch) const override
        {
            auto ret = this->Figch.at(ch);
            return ret;
        }

        /**
         * @brief A factory for this type return smart pointer to this type
         * 
         * @return std::shared_ptr<FigletFont<string_type>> 
         */
        static std::shared_ptr<FigletFont<string_type>> make(std::string font)
        {
            return std::make_shared<FigletFont<string_type>>(font);
        }
    };

    /**
     * @brief Full Width Style
     * 
     * @tparam StringType 
     */
    template <class StringType>
    class FullWidth : public Style<StringType>
    {
    public:
        using string_type = StringType;
        using char_type = typename string_type::value_type;
        using traits_type = typename string_type::traits_type;
        using size_type = typename string_type::size_type;
        using Figc_type = std::vector<string_type>;
        using Figs_type = std::vector<string_type>;

    public:
        static const inline size_type ShrinkLevel = 0;

    protected:
        /**
         * @brief convert ascii string to string_type
         * 
         * @param str string to convert
         * @return string_type string converted
         */
        string_type cvt(const std::string &str) const
        {
            return string_type(str.begin(), str.end());
        }

        /**
         * @brief checks height offig character
         * 
         * @param figc fig character
         * @param hg height
         */
        void error_check(const Figc_type &figc, size_type hg) const
        {
            if (figc.size() != hg)
            {
                throw std::runtime_error("Height is not equal");
            }
        }

        /**
         * @brief removes hardblank from figstring
         * 
         * @param figs fig string
         * @param hb hardblank
         */
        void remove_hardblank(Figs_type &figs, char_type hb) const
        {
            for (size_type i = 0; i < figs.size(); ++i)
            {
                std::replace(figs[i].begin(), figs[i].end(), hb, ' ');
            }
        }

    public:
        FullWidth() = default;
        FullWidth(const FullWidth &) = default;
        FullWidth(FullWidth &&) = default;

        /**
         * @brief Get the Shrink
         * 
         * @return size_type shrink value
         * 0 - untouched
         * 1 - kerning
         * 2 - smushed
         */
        size_type getShrinkLevel() const override
        {
            return 0;
        }

        /**
         * @brief Get the Fig string
         * 
         * @param figchs figlet characters
         * @param hardblank hardblank
         * @param height height
         * @param shirnk shrinkklevel
         * @return Figs_type Fig string
         */
        Figs_type getFigs (
            std::vector<Figc_type> figchs,
            char_type hardblank,
            size_type height,
            size_type shirnk
        ) const override
        {
            Figs_type figs(height, cvt(""));

            for (const auto &figc : figchs)
            {
                this->error_check(figc, height);
            }

            for (const auto &figc : figchs)
            {
                for (size_type i = 0; i < height; ++i)
                {
                    figs[i] += figc[i];
                }
            }

            remove_hardblank(figs, hardblank);
            return figs;
        }

        /**
         * @brief A factory for this type return smart pointer to this type
         * 
         * @return std::shared_ptr<FullWidth<string_type>> 
         */
        static std::shared_ptr<FullWidth<string_type>> make()
        {
            return std::make_shared<FullWidth<string_type>>();
        }
    };

    /**
     * @brief kerned Style
     * 
     * @tparam gap gap between characters
     * @tparam StringType 
     */
    template <class StringType>
    class Kerning : public FullWidth<StringType>
    {
    public:
        using string_type = StringType;
        using char_type = typename string_type::value_type;
        using traits_type = typename string_type::traits_type;
        using size_type = typename string_type::size_type;
        using Figc_type = std::vector<string_type>;
        using Figs_type = std::vector<string_type>;

    public:
        static const inline size_type ShrinkLevel = 1;

    private:
        size_type gap;

    protected:
        /**
         * @brief trims in a deep
         * 
         * @param figs fig string
         * @param figc fig character
         */
        void trim_deep(Figs_type &figs, Figc_type &figc) const
        {
            std::vector<size_type> elem;

            for (size_type i = 0; i < figs.size(); ++i)
            {
                int lcount = 0, rcount = 0;

                for (auto itr = figs[i].rbegin(); itr != figs[i].rend(); ++itr)
                {
                    if (*itr == ' ')
                        ++lcount;
                    else
                        break;
                }

                for (auto itr = figc[i].begin(); itr != figc[i].end(); ++itr)
                {
                    if (*itr == ' ')
                        ++rcount;
                    else
                        break;
                }

                elem.push_back(lcount + rcount);
            }

            size_type space = *std::min_element(elem.begin(), elem.end());

            for (size_type i = 0; i < figs.size(); ++i)
            {
                size_type siz = space;

                while (siz > 0 && figs[i].back() == ' ')
                {
                    figs[i].pop_back();
                    --siz;
                }

                figc[i].erase(0, siz);
            }
        }

    public:
        /**
         * @brief Construct a new Kerning
         * 
         * @param gap gap size
         */
        Kerning(size_type gap = 0) : gap(gap) {}
        Kerning(const Kerning &) = default;
        Kerning(Kerning &&) = default;

        /**
         * @brief Get the Shrink
         * 
         * @return size_type shrink value
         * 0 - untouched
         * 1 - kerning
         * 2 - smushed
         */
        size_type getShrinkLevel() const override
        {
            return 1;
        }

        /**
         * @brief Get the Fig string
         * 
         * @param figchs figlet characters
         * @param hardblank hardblank
         * @param height height
         * @param shirnk shrinkklevel
         * @return Figs_type Fig string
         */
        Figs_type getFigs (
            std::vector<Figc_type> figchs,
            char_type hardblank,
            size_type height,
            size_type shirnk
        ) const override
        {
            Figs_type figs(height, this->cvt(""));

            for (auto &figc : figchs)
            {
                this->error_check(figc, height);
            }

            for (auto &figc : figchs)
            {
                this->trim_deep(figs, figc);

                for (size_type i = 0; i < height; ++i)
                {
                    figs[i] += figc[i];
                }
            }

            this->remove_hardblank(figs, hardblank);
            return figs;
        }

        /**
         * @brief A factory for this type return smart pointer to this type
         * 
         * @return std::shared_ptr<Kerning<string_type>> 
         */
        static std::shared_ptr<Kerning<string_type>> make(size_type gap)
        {
            return std::make_shared<Kerning<string_type>>(gap);
        }
    };

    /**
     * @brief Smushing style
     * 
     * @tparam StringType 
     */
    template <class StringType>
    class Smushed : public Kerning<StringType>
    {
    public:
        using string_type = StringType;
        using char_type = typename string_type::value_type;
        using traits_type = typename string_type::traits_type;
        using size_type = typename string_type::size_type;
        using Figc_type = std::vector<string_type>;
        using Figs_type = std::vector<string_type>;

    public:
        static const inline size_type ShrinkLevel = 2;

    private:
        /**
         * @brief smush rules
         * @param lc left character
         * @param rc right character
         * @return smushed character
         */
        char_type smush_rules(char_type lc, char_type rc) const
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
            auto find_class = [](char_type ch) -> size_type {
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

            //(universel smush)
            return lc;
        }

        /**
         * @brief smush algoriths on kerned Fig string and character
         * 
         * @param figs 
         * @param figc 
         */
        void smush(Figs_type &figs, Figc_type figc, char_type hb) const
        {
            bool smushble = true;

            for (size_type i = 0; i < figs.size(); ++i)
            {
                if (figs[i].size() == 0 || figc[i].size() == 0)
                {
                    smushble = false;
                }
                else if ((figs[i].back() == hb) && !(figc[i].front() == hb))
                {
                    smushble = false;
                }
            }

            if (smushble)
            {
                for (size_type i = 0; i < figs.size(); ++i)
                {
                    char_type val = smush_rules(figs[i].back(), figc[i].front());
                    figs[i].pop_back();
                    figc[i].erase(0, 1);
                    figs[i] += string_type(1, val) + figc[i];
                }
            }
            else
            {
                for (size_type i = 0; i < figs.size(); ++i)
                {
                    figs[i] += figc[i];
                }
            }
        }

    public:
        Smushed() = default;
        Smushed(const Smushed &) = default;
        Smushed(Smushed &&) = default;
        /**
         * @brief Get the Fig string
         * 
         * @param figchs figlet characters
         * @param hardblank hardblank
         * @param height height
         * @param shirnk shrinkklevel
         * @return Figs_type Fig string
         */
        Figs_type getFigs (
            std::vector<Figc_type> figchs,
            char_type hardblank,
            size_type height,
            size_type shirnk
        ) const override
        {
            Figs_type figs(height, this->cvt(""));

            for (auto &figc : figchs)
            {
                this->error_check(figc, height);
            }

            for (auto &figc : figchs)
            {
                this->trim_deep(figs, figc);
                this->smush(figs, figc, hardblank);
            }

            this->remove_hardblank(figs, hardblank);
            return figs;
        }

        /**
         * @brief A factory for this type return smart pointer to this type
         * 
         * @return std::shared_ptr<Smushed<string_type>> 
         */
        static std::shared_ptr<Smushed<string_type>> make()
        {
            return std::make_shared<Smushed<string_type>>();
        }
    };
}

/**
 * @brief namespace Figlet
 */
namespace srilakshmikanthanp::Figlet
{
    /// @brief char figlet font
    using CharFigletFont = FigletFont<std::string>;

    /// @brief char full width style
    using CharFullWidth = FullWidth<std::string>;

    /// @brief char Kerning
    using CharKerning = Kerning<std::string>;

    /// @brief char smushed
    using CharSmushed = Smushed<std::string>;

    /// @brief char Figlet
    using CharFiglet = Figlet<std::string>;
}

#endif