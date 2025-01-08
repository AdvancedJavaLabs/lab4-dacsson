#ifndef PRODUCT_H
#define PRODUCT_H

#include <sstream>
#include <string>
#include <sys/types.h>

struct Product {
  uid_t id;
  std::string name;

  std::string to_string(){
    std::ostringstream ss;
    ss << id << "," << name;
    return ss.str();
  } 
};

#endif
