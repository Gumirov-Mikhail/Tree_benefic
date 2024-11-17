#pragma once

#include <algorithm>
#include <cstdint>
#include <deque>
#include <unordered_map>
#include <vector>

#include "../../../include/Company.hpp"
#include "../../../include/Fizik.hpp"
#include "../../../include/Urik.hpp"
#include "Node.hpp"
#include "Tree.hpp"

class CompanyTree : public Tree {
  int64_t companyId;
  std::deque<Node*> queue;

  void fillLegal(const std::unordered_map<int64_t, std::vector<Urik*>>& map) {
    std::deque<Node*> leftovers;
    while (!queue.empty()) {
      Node* node = queue.front();
      auto it = map.find(node->founder->GetId());
      queue.pop_front();
      leftovers.push_back(node);
      if (it == map.end()) continue;
      auto& children = it->second;
      for (auto& child : children) {
        bool found = false;
        for (auto& prev : node->previous) {
          if (prev->founder->GetInn() == child->GetInn()) {
            found = true;
            break;
          }
        }
        if (!found) {
          queue.push_back(
              addChild(node, child, Type::Legal, child->GetSharePercent()));
        }
      }
    }
    std::swap(queue, leftovers);
  }

  void fillNatural(
      const std::unordered_map<int64_t, std::vector<Fizik*>>& map) {
    std::deque<Node*> leaves;
    for (auto node : queue) {
      auto it = map.find(node->founder->GetId());
      if (it == map.end()) continue;
      auto& children = it->second;
      for (auto& child : children) {
        leaves.push_back(
            addChild(node, child, Type::Natural, child->GetSharePercent()));
      }
    }
    std::swap(queue, leaves);
  }

 public:
  CompanyTree(Founder* root) : Tree(root), companyId(root->GetId()) {
    queue.push_back(this->root);
  }
  ~CompanyTree() = default;

  void buildTree(
      const std::unordered_map<int64_t, std::vector<Urik*>>& legal,
      const std::unordered_map<int64_t, std::vector<Fizik*>>& natural) {
    fillLegal(legal);
    fillNatural(natural);
  }

  bool replaceWithNaturalLeaf(Node* node) {
    if (node->edges.empty()) return false;
    if (node->parent == nullptr) return false;
    if (node->edges.size() != 1 || !node->edges[0]->to->isLeaf()) return false;
    if (node->edges[0]->to->type != Type::Natural) return false;

    Urik* urik = (Urik*)node->founder;
    Fizik* fizik = (Fizik*)node->edges[0]->to->founder;

    if (urik->GetShare().has_value() && fizik->GetShare().has_value() &&
        !node->edges[0]->weight.has_value()) {
      node->edges[0]->weight =
          urik->GetShare().value() / fizik->GetShare().value();
      if (node->edges[0]->weight > 1) {
        node->edges[0]->weight = std::nullopt;
      }
    }

    Edge* papa = node->parent;
    Node* to_replace = node->edges[0]->to;
    std::optional<double> weight = node->edges[0]->weight;

    to_replace->parent = papa;
    papa->to = to_replace;

    node->edges[0]->from = nullptr;
    node->edges[0]->to = nullptr;
    node->parent = nullptr;
    node->founder = nullptr;
    node->previous = {};

    if (weight.has_value() && papa->weight.has_value()) {
      double mult = weight.value() * papa->weight.value();
      papa->weight = mult;
      fizik->SetSharePercent(mult);
    }

    delete node;
    return true;
  }

  void compressLeaves(Node* node) {
    if (node == nullptr) return;
    for (auto& edge : node->edges) {
      compressLeaves(edge->to);
    }
    if (replaceWithNaturalLeaf(node)) return;
  }

  std::pair<int64_t, std::unordered_map<int64_t, double>> countSum() {
    std::pair<int64_t, std::unordered_map<int64_t, double>> res = {0, {}};
    for (auto& leaf : queue) {
      Fizik* fizik = (Fizik*)leaf->founder;
      if (!fizik->GetShare().has_value()) continue;
      res.first += fizik->GetShare().value();
      res.second[fizik->GetInn()] += fizik->GetShare().value();
    }
    return res;
  }

  std::pair<Company*, std::vector<Fizik*>> getBeneficiaries(
      std::vector<Fizik>& natural,
      std::unordered_map<int64_t, Company*>& companies,
      std::pair<int64_t, std::unordered_map<int64_t, double>> totalsum) {
    std::pair<Company*, std::vector<Fizik*>> res;

    for (auto& [inn, sum] : totalsum.second) {
      if (sum / totalsum.first > 0.25) {
        for (auto& fizik : natural) {
          if (fizik.GetInn() == inn) {
            fizik.SetSharePercent(sum / totalsum.first);
            res.second.push_back(const_cast<Fizik*>(&fizik));
            break;
          }
        }
      }
    }
    if (!res.second.empty()) {
      res.first = companies[companyId];
    }
    return res;
  }

  void healTree() {}

  void optimizeTree() {}
};
