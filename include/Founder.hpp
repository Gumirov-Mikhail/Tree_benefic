#pragma once

#include <cstdint>

class Founder {
 private:
  int64_t id;
  int64_t inn;

 public:
  Founder() : id(0), inn(0) {}
  Founder(int64_t id, int64_t inn) : id(id), inn(inn) {}

  int64_t GetId() const { return id; }
  int64_t GetInn() const { return inn; }

  ~Founder() {}
};
