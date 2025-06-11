#pragma once

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace sutils
{
  inline void log(const std::string& s)
  {
    std::cout << s << "\n";
  }


  struct printer_t
  {
    inline static void head(const std::string& str)
    {
      std::string hd = "===================================================";
      std::string hp = "-";
      
      std::string header = hp + " " + str + " " + hp;
      uint rpad_len = (hd.size() - header.size()) / 2;
      std::string rpad(rpad_len, '=');
      uint lpad_len = hd.size() - (rpad_len + header.length());
      std::string lpad(lpad_len, '=');

      std::cout << hd << "\n";
      std::cout << hd << "\n";
      std::cout << rpad << header << lpad << "\n";
      std::cout << hd << "\n";
      std::cout << hd << "\n\n";
    }

    inline static void err(const std::string& str) {
      error(str);
    }
    inline static void warn(const std::string& str) {
      warning(str);
    }


    inline static void line(const std::string& str) {
      std::cout << str << "\n";
    }
    inline static void str(const std::string& str) {
      std::cout << str;
    }
    inline static void error(const std::string& str) {
      std::cout << "!!!!!!!       ERROR   : " << str << "\n";
    }
    inline static void ok(const std::string& str) {
      std::cout << "OK      : " << str << "\n";
    }
    inline static void warning(const std::string& str) {
      std::cout << "!!!!!!!       WARNING : " << str << "\n";
    }

    inline static std::string pad_right(std::string str, const std::size_t len, const char paddingChar = ' ') {
      if (len <= str.size()) return str;
      str.insert(0, len - str.size(), paddingChar);
      return str;
    }

    inline static std::string pad_right(int value, const std::size_t len, const char paddingChar = ' ') {
      std::string str = std::to_string(value);
      return pad_right(str, len, paddingChar);
    }
    
  };

  using print = printer_t;

  inline std::string pad_right(std::string str, const std::size_t len, const char paddingChar = ' ')
  {
    return print::pad_right(str, len, paddingChar);
  }

  inline std::string pad_right(int value, const std::size_t len, const char paddingChar = ' ')
  {
    return print::pad_right(value, len, paddingChar);
  }


  struct array_printer_t
  {
    // Data contains:
    // - header
    // - lines
    std::vector<std::vector<std::string>> data;

    std::size_t get_line_length()
    {
      if (data.size() == 0)
      {
        print::err("array_printer_t.get_line_length: no line to draw.");
        return 0;
      }
      std::size_t line_length = data[0].size();
      if (line_length == 0)
      {
        print::err("array_printer_t.get_line_length: (line_length == 0).");
        return 0;
      }
      return line_length;
    }

    std::vector<std::size_t> get_sizes()
    {
      std::vector<std::size_t> line_sizes;
      std::vector<std::size_t> error_sz;
      // if (data.size() == 0)
      // {
      //   print::err("array_printer_t.get_sizes: no line to draw.");
      //   return error_sz;
      // }
      // std::size_t line_length = data[0].size();
      std::size_t line_length = get_line_length();
      if (line_length == 0)
      {
        print::err("array_printer_t.get_sizes: (line_length == 0).");
        return error_sz;
      }
      line_sizes.resize(line_length); // size of the first line (header)
      for (std::size_t& s : line_sizes)
      {
        s = 0;
      }
      std::size_t line_index = 0;
      for (const std::vector<std::string>& line : data)
      {
        if (line_length != line.size())
        {
          print::err( "array_printer_t.get_sizes: (line_length("
                      + std::to_string(line_length) + ") != "
                      + "line[" + std::to_string(line_index) + "].size()"
                      + std::to_string(line.size()) + ").");
          return error_sz;
        }
        
        for (std::size_t str_ind = 0; str_ind < line_length; ++str_ind)
        {
          const std::string& word = line[str_ind];
          if (line_sizes[str_ind] < word.size())
          {
            line_sizes[str_ind] = word.size();
          }
        }

        ++line_index;
      }
      return line_sizes;
    }

    void print()
    {
      std::vector<std::size_t>  line_sizes  = get_sizes();
      std::size_t               line_length = get_line_length(); 
      std::size_t extra_pad = 2;
      for (const std::vector<std::string>& line : data)
      {
        for (std::size_t str_ind = 0; str_ind < line_length; ++str_ind)
        {
          const std::string& word = line[str_ind];
          std::string word_padded = print::pad_right(word, line_sizes[str_ind] + extra_pad);
          std::cout << word_padded;
        }
        std::cout << "\n";
      }
    }

    void add_delimiter(std::string delimiter)
    {
      if (data.size() != 0)
      {
        std::vector<std::string> nline;
        nline.resize(get_line_length());
        for (auto& s : nline)
        {
          s = delimiter;
        }
        data.push_back(nline);
      }
    }


    void add(const std::vector<std::string>& line)
    {
      data.push_back(line);
    }

  };
}