#include <cstdint>
#include <unordered_map>
#include <vector>

#include "../include/Company.hpp"
#include "../include/Fizik.hpp"
#include "../include/Urik.hpp"
#include "../lib/Forest/Forest.hpp"
#include "../lib/ParserWrap.hpp"

int main() {
  std::unordered_map<int64_t, Company *> companies;
  std::vector<Urik> legal;
  std::vector<Fizik> natural;

  std::unordered_map<int64_t, std::vector<Urik *>> legal_map;
  std::unordered_map<int64_t, std::vector<Fizik *>> natural_map;

  std::string data_path(std::getenv("DATA_NEW"));

  companies = parser::parseCompanies(data_path + "company.tsv");
  legal = parser::parseUriks(data_path + "founder_legal.tsv");
  natural = parser::parseFiziks(data_path + "founder_natural.tsv");

  legal_map = frq::groupLegal(legal);
  natural_map = frq::groupNatural(natural);

  Forest forest(companies, legal_map, natural_map);
  fl::writeToFile("data/output.tsv",
                  forest.getBeneficiaries(natural, companies));
}
