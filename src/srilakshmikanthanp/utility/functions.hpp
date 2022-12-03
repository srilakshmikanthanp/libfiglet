// Copyright (c) 2022 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRILAKSHMIKANTHANP_UTILITIES_FUNCTIONS_HPP
#define SRILAKSHMIKANTHANP_UTILITIES_FUNCTIONS_HPP

namespace srilakshmikanthanp
{
  namespace libfiglet
  {
    /**
     * @brief convert ascii string to string_type_t
     *
     * @param str string to convert
     * @return string_type_t string converted
     */
    template <class string_type_t>
    string_type_t cvt(const std::string str)
    {
      return string_type_t(str.begin(), str.end());
    }
  }
}

#endif
