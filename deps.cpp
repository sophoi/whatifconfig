#include "deps.hpp"

std::map<std::string, std::string> Deps::listAffected(std::string fname,
                                                      bool checkUseful)
{
  std::map<std::string, std::string> seen;
  std::set<std::string> changing = {fname};
  while (not changing.empty()) {
    std::set<std::string> nextChanging;
    for (auto f : changing) {
      if (not checkUseful or store_[f].useful) {
        for (auto incer : store_[f].des) {
          if (seen.emplace(incer, f).second) {
            nextChanging.insert(incer);
          }
        }
      }
    }
    changing.swap(nextChanging);
  }
  return seen;
}

void Deps::markUsful(const std::vector<std::string> &envs)
{
  std::set<std::string> marked;
  std::set<std::string> seeds(std::begin(envs), std::end(envs));
  while (not seeds.empty()) {
    std::set<std::string> nextSeeds;
    for (auto f : seeds) {
      store_[f].useful = true;
      for (auto inced : store_[f].anc) {
        if (not store_[inced].useful and marked.insert(inced).second) {
          nextSeeds.insert(inced);
        }
      }
    }
    seeds.swap(nextSeeds);
  }
}
