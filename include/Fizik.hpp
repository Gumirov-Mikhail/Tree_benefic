#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "Founder.hpp"

class Fizik : public Founder {
 private:
  int64_t company_id;
  std::string last_name;
  std::string first_name;
  std::string second_name;

  std::optional<double> share;
  std::optional<double> share_percent;

 public:
  Fizik()
      : Founder(),
        company_id(0),
        last_name(""),
        first_name(""),
        second_name(""),
        share(std::nullopt),
        share_percent(std::nullopt) {}

  Fizik(int64_t id, int64_t company_id, int64_t inn,
        const std::string& last_name, const std::string& first_name,
        const std::string& second_name,
        std::optional<double> share = std::nullopt,
        std::optional<double> share_percent = std::nullopt)
      : Founder(id, inn),
        company_id(company_id),
        last_name(last_name),
        first_name(first_name),
        second_name(second_name),
        share(share),
        share_percent(share_percent) {}

  int64_t GetCompanyId() const { return company_id; }
  std::string GetLastName() const { return last_name; }
  std::string GetFirstName() const { return first_name; }
  std::string GetSecondName() const { return second_name; }
  std::string GetFullName() const {
    return last_name + " " + first_name + " " + second_name;
  }
  std::optional<double> GetShare() const { return share; }
  std::optional<double> GetSharePercent() const { return share_percent; }

  void SetShare(std::optional<double> share) { this->share = share; }
  void SetSharePercent(std::optional<double> share_percent) {
    this->share_percent = share_percent;
  }

  ~Fizik() {}
};
