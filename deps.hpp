// walk dir and gen dep graph, how to be fast easily?
// query dep graph on a proposed file change
// call icfx on file change, with path finder to resolve paths properly

// 2-level tree with a source and multiple or zero targets
// can represent either included or including relationship
// for each #include pair, update 2 Shrubs' target lists
// a map index all Shrub sources for quick lookup
// a Node should contain 1. filename 2. dir (prod/staging/local/fossil)
// 2 Forests should get compared, but how to represent them flexibly?
//   prod vs. staging
//   staging vs. newer local change
//   arbitrary cmp like staging/{x.000,y} vs. prod/{x.001,y.000}
// a Forest is constructed from prod or fossil by stamping on change sets

#include <string>
#include <set>
#include <map>
#include <vector>
#include <unordered_map>

struct Rel {
  bool useful;
  std::set<std::string> anc;  // ancesters/included

  std::set<std::string> des;  // descendents/includers
};

class Deps {
  // file -> includers (affected)
  std::unordered_map<std::string, Rel> store_;

public:
  // mammal(includer) : #include animal(included)
  inline void addInclude(std::string includer, std::string included) {
    store_[includer].anc.insert(includer);
    store_[included].des.insert(includer);
  }
  std::map<std::string, std::string> listAffected(std::string fname,
                                                  bool checkUseful);
  void markUsful(const std::vector<std::string> &envs);
};
