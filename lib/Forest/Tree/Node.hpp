#pragma once

#include <optional>
#include <vector>

#include "../../../include/Founder.hpp"

struct Node;
enum class Type { Legal, Natural, Company };

struct Edge {
  Node* from;
  Node* to;
  std::optional<double> weight;

  Edge(Node* from, Node* to, std::optional<double> weight = std::nullopt)
      : from(from), to(to), weight(weight) {}

  ~Edge() { from = nullptr; }
};

struct Node {
  Founder* founder;
  Type type;
  std::vector<Node*> previous;

  Edge* parent;
  std::vector<Edge*> edges;

  Node(Founder* founder, Type type, Edge* parent = nullptr)
      : founder(founder), parent(parent), type(type) {}

  Edge* addEdge(Node* to, std::optional<double> weight = std::nullopt) {
    edges.push_back(new Edge(this, to, weight));
    return edges.back();
  }

  void addPrevious(Node* node) { previous.push_back(node); }
  void addPrevious(std::vector<Node*> nodes) {
    previous.insert(previous.end(), nodes.begin(), nodes.end());
  }

  bool isLeaf() { return edges.empty(); }

  ~Node() {
    delete parent;
    for (auto edge : edges) {
      delete edge;
    }
  }
};
