#pragma once

#include <fmt/core.h>

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/Company.hpp"
#include "../include/Fizik.hpp"
#include "../include/Urik.hpp"
#include "tsvParser.hpp"

namespace parser {
std::optional<double> stringToDouble(const std::string& str) {
  double value = std::stod(str);
  return (value == 0) ? std::nullopt : std::optional<double>(value);
}

std::unordered_map<int64_t, Company*> parseCompanies(const std::string& path) {
  tsv::File file;
  file.OpenFile(path);

  std::unordered_map<int64_t, Company*> companies;

  int64_t id, inn;
  std::string full_name, ogrn;

  for (auto& [i, row] : file.Rows) {
    if (i == 0) continue;

    for (auto& [j, column] : row.Columns) {
      if (j == 0) id = column.GetBigNumber();
      if (j == 1) ogrn = column.GetString();
      if (j == 2) inn = column.GetBigNumber();
      if (j == 3) full_name = column.GetString();
    }

    companies[id] = new Company(id, ogrn, inn, full_name);
  }
  return companies;
}

std::vector<Urik> parseUriks(const std::string& path) {
  tsv::File file;
  file.OpenFile(path);

  std::unordered_map<int64_t, std::vector<Urik*>> legal_map;
  std::vector<Urik> uriks;
  uriks.reserve(file.Rows.size() - 1);

  int64_t id, company_id, inn;
  std::string ogrn, full_name, share_str, share_percent_str;
  std::optional<double> share, share_percent;

  for (auto& [i, row] : file.Rows) {
    if (i == 0) continue;
    for (auto& [j, column] : row.Columns) {
      switch (j) {
        case 0:
          id = column.GetBigNumber();
          break;
        case 1:
          company_id = column.GetBigNumber();
          break;
        case 2:
          ogrn = column.GetString();
          break;
        case 3:
          inn = column.GetBigNumber();
          break;
        case 4:
          full_name = column.GetString();
          break;
        case 5:
          share_str = column.GetString();
          break;
        case 6:
          share_percent_str = column.GetString();
          break;
      }
    }

    share = stringToDouble(share_str);
    share_percent = stringToDouble(share_percent_str);

    uriks.emplace_back(id, company_id, ogrn, inn, full_name, share,
                       share_percent);
  }

  return uriks;
}

std::vector<Fizik> parseFiziks(const std::string& path) {
  tsv::File file;
  file.OpenFile(path);

  std::vector<Fizik> fiziks;
  fiziks.reserve(file.Rows.size() - 1);

  int64_t id, company_id, inn;
  std::string last_name, first_name, second_name, share_str, share_percent_str;
  std::optional<double> share, share_percent;

  for (auto& [i, row] : file.Rows) {
    if (i == 0) continue;
    for (auto& [j, column] : row.Columns) {
      switch (j) {
        case 0:
          id = column.GetBigNumber();
          break;
        case 1:
          company_id = column.GetBigNumber();
          break;
        case 2:
          inn = column.GetBigNumber();
          break;
        case 3:
          last_name = column.GetString();
          break;
        case 4:
          first_name = column.GetString();
          break;
        case 5:
          second_name = column.GetString();
          break;
        case 6:
          share_str = column.GetString();
          break;
        case 7:
          share_percent_str = column.GetString();
          break;
      }
    }

    share = stringToDouble(share_str);
    share_percent = stringToDouble(share_percent_str);

    fiziks.emplace_back(id, company_id, inn, last_name, first_name, second_name,
                        share, share_percent);
  }
  return fiziks;
}
}  // namespace parser

namespace frq {
std::unordered_map<int64_t, std::vector<Urik*>> groupLegal(
    const std::vector<Urik>& uriks) {
  std::unordered_map<int64_t, std::vector<Urik*>> legal_map;
  for (const auto& urik : uriks) {
    legal_map[urik.GetCompanyId()].push_back(const_cast<Urik*>(&urik));
  }
  return legal_map;
}
std::unordered_map<int64_t, std::vector<Fizik*>> groupNatural(
    const std::vector<Fizik>& fiziks) {
  std::unordered_map<int64_t, std::vector<Fizik*>> natural_map;
  for (const auto& fizik : fiziks) {
    natural_map[fizik.GetCompanyId()].push_back(const_cast<Fizik*>(&fizik));
  }
  return natural_map;
}
}  // namespace frq

namespace fl {

std::string formatSharePercent(double value) {
  return fmt::format("{:.2f}", value);
}

void writeToFile(const std::string& path,
                 const std::vector<std::pair<Company*, std::vector<Fizik*>>>&
                     beneficiaries) {
  tsv::File export_file;

  for (const auto& [company, natural] : beneficiaries) {
    if (natural.empty()) continue;
    export_file += {{std::to_string(company->GetId())},
                    {company->GetOgrn()},
                    {std::to_string(company->GetInn())},
                    {company->GetFullName()}};
    for (const auto& fizik : natural) {
      export_file += {{std::to_string(fizik->GetInn())},
                      {fizik->GetFullName()},
                      {formatSharePercent(fizik->GetSharePercent().value())}};
    }
  }
  export_file.ToFile(path);
}

}  // namespace fl
