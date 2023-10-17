#pragma once

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include "printer.hpp"
#include "text_file.hpp"


bool remove_file(const std::string& fname)
{
  std::remove(fname.data()); // delete file
  bool still_there = !!std::ifstream(fname);
  if(still_there) {
    print::err("@remove_file: File still present, unable to remove: " + fname);
    return false;
  }
  return true;
}

// struct global_write_file_t
// {
// public:
//   static text_file_w* file;
//   static void open(std::string path)
//   {
//     close();
//     file = new text_file_w(path);
//   }

//   static void close()
//   {
//     if (file) { // != nullptr
//       file->close();
//       file = nullptr;
//     }
//   }

//   static void write(const std::string& str) { file->write(str); }
//   // std::ostream& get_ostream() const { return file->get_ostream(); }
//   // std::ostream& operator<<(std::string s) { return ((*file) << s); }

// // private:
// };

struct global_write_file_t
{
public:
  text_file_w* file;
  void open(std::string path)
  {
    close();
    file = new text_file_w(path);
  }

  void close()
  {
    if (file) { // != nullptr
      file->close();
      file = nullptr;
    }
  }

  void write(const std::string& str) { file->write(str); }
  std::ostream& get_ostream() const { return file->get_ostream(); }
  // std::ostream& operator<<(std::string s) { return ((*file) << s); }
  std::ostream& operator<<(auto a) { return ((*file) << a); }

// private:
};

global_write_file_t write_f{};
global_write_file_t mem_test_write_f{};
// using write_t = global_write_file_t();
