#include <iostream>
#include <string>
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include "deps.hpp"

using namespace std;

namespace {
  bool debug = 0;
}

// PATH_MAP/I_PATH_MAP: NetDrive:=/abs/path
// O_PATH_MAP: /long/path/to=$simple
int main(int argc, char *argv[]) {
  if (argc != 2) { cerr << "expecting 1 arg" << endl; exit(-1); }
  Deps deps;
  for (std::string line; std::getline(std::cin, line);) {
    char fname[1024], inced[1024];
    sscanf(line.c_str(), "%s %s", fname, inced);
    if (debug) { cout << fname << ", " << inced << "\n"; }
    std::string incNormed  = inced;
    if (inced[0] != '/') {
      char* fdir = dirname(fname); //const_cast<char*>(fname.c_str()));
      if (strcmp(fdir, ".")) {
        incNormed = std::string(fdir) + "/" + inced;
        if (debug) { cout << inced << " --> " << incNormed << endl; }
      }
    }
    deps.addInclude(fname, incNormed);
  }
  auto aff = deps.listAffected(argv[1], false);
  cout << " ---- quering for '" << argv[1] << "'\n";
  for (auto& kv : aff) {
    cout << kv.first << " (" << kv.second << ")\n";
  }
}
