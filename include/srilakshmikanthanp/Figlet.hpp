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
         * @brief Get the Fig string
         * 
         * @param figchs figlet characters
         * @param hardblank hardblank
         * @param height height
         * @param shirnk shrinkklevel
         * @return Figs_type Fig string
         */
        virtual Figs_type getFigs(
            std::vector<Figc_type> figchs,
            char_type hardblank,
            size_type height,
            size_type shirnk) const = 0;
    };

    /**
     * @brief class Figlet
     */
    template <class FontT, class StyleT>
    class Figlet
    {
    public:
        using string_type = typename std::enable_if<std::is_same<typename FontT::string_type, typename StyleT::string_type>::value, typename FontT::string_type>::type;
        using char_type = typename string_type::value_type;
        using traits_type = typename string_type::traits_type;
        using size_type = typename string_type::size_type;
        using Figc_type = std::vector<string_type>;
        using Figs_type = std::vector<string_type>;

    private:
        using ostream_type = std::basic_ostream<char_type, traits_type>;

    private:
        std::shared_ptr<Font<string_type>> font;
        std::shared_ptr<Style<string_type>> &style;
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
        Figlet(std::shared_ptr<Font<string_type>> font,
               std::shared_ptr<Style<string_type>> style)
            : font(font), style(style)
        {
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
        Figc_type nextcharacter(std::basic_istream<char_type, traits_type> &stream)
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
                this->Figch[ch] = this->nextcharacter(stream);
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
            std::ifstream stream(font);

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

    protected:
        static string_type cvt(const std::string &str)
        {
            return string_type(str.begin(), str.end());
        }

        void error_check(const Figc_type &figc, size_type hg) const
        {
            if (figc.size() != hg)
            {
                throw std::runtime_error("Height is not equal");
            }
        }

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
         * @brief Get the Fig string
         * 
         * @param figchs figlet characters
         * @param hardblank hardblank
         * @param height height
         * @param shirnk shrinkklevel
         * @return Figs_type Fig string
         */
        Figs_type getFigs(
            std::vector<Figc_type> figchs,
            char_type hardblank,
            size_type height,
            size_type shirnk) const override
        {
            Figs_type figs(height, cvt(""));

            for (const auto &figc : figchs)
            {
                this->error_check(figc, height);
            }

            for (size_type i = 0; i < height; ++i)
            {
                for (const auto &figc : figchs)
                {
                    figs[i] += figc[i];
                }
            }

            remove_hardblank(figs, hardblank);
            return figs;
        }
    };
}

#endif
