#pragma once

#include "Node.hpp"

class Tree {
 public:
  Node* root;

  Tree() : root(nullptr) {}
  Tree(Founder* root) : root(new Node(root, Type::Company)) {}
  ~Tree() = default;

  Node* addChild(Node* parent, Founder* child, Type type,
                 std::optional<double> weight) {
    if (parent) {
      Edge* edge = parent->addEdge(new Node(child, type), weight);
      edge->to->parent = edge;
      edge->to->addPrevious(parent->previous);
      edge->to->addPrevious(parent);
      return edge->to;
    }
    return nullptr;
  }
};
