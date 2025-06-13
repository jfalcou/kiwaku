#pragma once

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

#include "printer.hpp"

// ======================= TEXT_FILE =======================

namespace sutils
{
  struct read_stream_t
  {
    virtual std::string read_line() = 0;
  };

  struct write_stream_t
  {
    virtual void write(const std::string&) = 0;
  };


  struct text_file_r: public read_stream_t
  {
  public:
    text_file_r(const std::string&);
    ~text_file_r();
    void close();
    std::string read_line();
  private:
    std::ifstream* f;
    bool is_open = false;
  };

  struct text_file_w: public write_stream_t
  {
  public:
    text_file_w(const std::string&);
    ~text_file_w();
    void close();
    void write(const std::string&);
    std::ostream& get_ostream() const { return *f; }
    // std::ostream& operator<<(std::string s) { return (*f) << s; }
    std::ostream& operator<<(auto s) { return (*f) << s; }
  private:
    std::ofstream* f;
    bool is_open = false;
  };


  struct write_string_t: public write_stream_t
  {
  public:
    void write_line(const std::string& s) { data << s << "\n"; }
    std::ostream& operator<<(std::string s) { return data << s; }
    std::string str() { return data.str(); }
  private:
    std::ostringstream data;
  };

  struct read_string_t: public read_stream_t
  {
  public:
    read_string_t(std::stringstream* data_) { data = data_; }
    std::string read_line();
  private:
    std::stringstream* data;
  };










  std::string read_string_t::read_line()
  {
    std::string res;
    std::getline(*data, res);
    return res;
  }

  // === text_file_r ===

  text_file_r::~text_file_r() { close(); }

  void text_file_r::close()
  {
    if (is_open) {
      is_open = false;
      f->close();
      delete f;
    }
  }

  std::string text_file_r::read_line()
  {
    std::string res;
    std::getline(*f, res);
    return res;
  }

  text_file_r::text_file_r(const std::string& fname)
  {
    f = new std::ifstream(fname);
    if (!f->good()) {
      std::cout << "FILE ERROR: Failed to open input file " << fname << "\n";
      std::exit(1);
    } else {
      is_open = true;
    }
  }



  // === text_file_w ===

  text_file_w::~text_file_w() { close(); }

  void text_file_w::close()
  {
    if (is_open) {
      is_open = false;
      f->close();
      delete f;
    }
  }

  void text_file_w::write(const std::string& line)
  {
    (*f) << line;
  }

  text_file_w::text_file_w(const std::string& fname)
  {
    f = new std::ofstream(fname);
    if (!f->good()) {
      std::cout << "FILE ERROR: Failed to open output file " << fname << "\n";
      std::exit(1);
    } else {
      is_open = true;
    }
  }
}