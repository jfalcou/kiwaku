#pragma once

#include <unistd.h>
#include <limits.h>
#include <string>

namespace sutils
{
  std::string get_computer_name()
  {
    // Does not handle error codes returned by gethostname.
    // As described in: https://man7.org/linux/man-pages/man2/gethostname.2.html
    
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    std::string name(hostname);
    return name;
  }
  std::string get_computer()  { return get_computer_name(); }
  std::string get_host_name() { return get_computer_name(); }


  std::string get_user_name()
  {
    // Does not handle error codes returned by getlogin_r.
    // As described in: https://man7.org/linux/man-pages/man2/gethostname.2.html
    
    char username[LOGIN_NAME_MAX];
    getlogin_r(username, LOGIN_NAME_MAX);
    std::string name(username);
    return name;
  }
  std::string get_login() { return get_user_name(); }
  std::string get_user()  { return get_user_name(); }
}
