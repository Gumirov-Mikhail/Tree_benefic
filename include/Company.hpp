#pragma once

#include <cstdint>
#include <string>

#include "Founder.hpp"

class Company : public Founder {
 private:
  std::string ogrn;
  std::string full_name;

 public:
  Company() : Founder(), ogrn(""), full_name("") {}
  Company(int64_t id, const std::string& ogrn, int64_t inn,
          const std::string& full_name)
      : Founder(id, inn), ogrn(ogrn), full_name(full_name) {}

  std::string GetOgrn() const { return ogrn; }
  std::string GetFullName() const { return full_name; }

  ~Company() {};
};
