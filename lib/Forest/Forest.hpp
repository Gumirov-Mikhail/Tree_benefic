#pragma once

#include <unordered_map>

#include "Tree/CompanyTree.hpp"

class Forest {
 private:
  std::vector<CompanyTree> trees;

 public:
  Forest() = default;
  Forest(const std::unordered_map<int64_t, Company*>& companies,
         const std::unordered_map<int64_t, std::vector<Urik*>>& legal_map,
         const std::unordered_map<int64_t, std::vector<Fizik*>>& natural_map) {
    trees.reserve(companies.size());
    for (auto& company : companies) {
      trees.emplace_back(company.second);
      trees.back().buildTree(legal_map, natural_map);
      trees.back().compressLeaves(trees.back().root);
    }
  }

  std::vector<std::pair<Company*, std::vector<Fizik*>>> getBeneficiaries(
      std::vector<Fizik>& natural,
      std::unordered_map<int64_t, Company*>& companies) {
    std::vector<std::pair<Company*, std::vector<Fizik*>>> res;
    for (auto& tree : trees) {
      res.push_back(tree.getBeneficiaries(natural, companies, tree.countSum()));
    }
    return res;
  }

  ~Forest() = default;
};
