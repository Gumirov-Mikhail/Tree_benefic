#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "Founder.hpp"

class Urik : public Founder {
 private:
  int64_t company_id;
  std::string ogrn;
  std::string full_name;
  std::optional<double> share;
  std::optional<double> share_percent;

 public:
  Urik()
      : Founder(),
        company_id(0),
        ogrn(""),
        full_name(""),
        share(std::nullopt),
        share_percent(std::nullopt) {}

  Urik(int64_t id, int64_t company_id, const std::string& ogrn, int64_t inn,
       const std::string& full_name, std::optional<double> share = std::nullopt,
       std::optional<double> share_percent = std::nullopt)
      : Founder(id, inn),
        company_id(company_id),
        ogrn(ogrn),
        full_name(full_name),
        share(share),
        share_percent(share_percent) {}

  int64_t GetCompanyId() const { return company_id; }
  std::string GetOgrn() const { return ogrn; }
  std::string GetFullName() const { return full_name; }
  std::optional<double> GetShare() const { return share; }
  std::optional<double> GetSharePercent() const { return share_percent; }
};
