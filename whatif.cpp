#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include "deps.hpp"

using namespace std;

namespace {
  bool debug = 0;
}

// PATH_MAP/I_PATH_MAP: NetDrive:=/abs/path
// O_PATH_MAP: /long/path/to=$simple
// WHATEXT
int main(int argc, char *argv[]) {
  if (argc != 2 and argc != 3) { cerr << "expecting 1 or 2 args" << endl; exit(-1); }
//  if (argc == 3) {
    std::string argLeft(argv[1]), argRite(argv[2]);
    if (not (argLeft.find(".ini") and argRite.find(".ini"))) {
      cerr << "only .ini files are support for now\n";
      exit(-1);
    }
    char *tmp = basename(argv[1]);
    if (!tmp) { cerr << "basename failed on '" << argv[1] << "'\n"; exit(-1); }
    argLeft = tmp;

    tmp = basename(argv[2]);
    if (!tmp) { cerr << "basename failed on '" << argv[2] << "'\n"; exit(-1); }
    argRite = tmp;
    
    tmp = getenv("WHATEXT");
    std::string whatFile, whatExt;
    if (debug) { cout << argLeft << " vs. " << argRite << " on " << tmp << endl; }
    if (argLeft != argRite and tmp) {
      whatExt = tmp;
      if (argLeft + "." + whatExt == argRite) {
        whatFile = argLeft;
      } else if (argLeft == argRite + "." + whatExt) {
        whatFile = argRite;
      }
      if (debug) { cout << "WHATEXT: " << whatExt << ", whatFile: " << whatFile << endl; }
    }
    tmp = getenv("INIDIFF_BIN");
    if (! tmp) {
      cerr << "INIDIFF_BIN not configured" << endl;
      exit(-1);
    }
    std::string inidiff(tmp);
    std::string cmd = inidiff + " " + argv[1] + " " + argv[2];
    int r = system(cmd.c_str());
    if (r != 0) {
      cerr << cmd << " failed with code: " << r << endl;
      exit(-1);
    }
    if (whatFile.empty()) {
      return 0; // assert argc == 3
    }
//  }
  Deps deps;
  char* fdir;
  for (std::string line; std::getline(std::cin, line);) {
    char fname[1024], inced[1024];
    sscanf(line.c_str(), "%s %s", fname, inced);
    if (debug) { cout << fname << ", " << inced << "\n"; }
    std::string incNormed  = inced;
    if (inced[0] != '/') {
      fdir = dirname(fname); //const_cast<char*>(fname.c_str()));
      if (strcmp(fdir, ".")) {
        incNormed = std::string(fdir) + "/" + inced;
        if (debug) { cout << inced << " --> " << incNormed << endl; }
      }
    }
    deps.addInclude(fname, incNormed);
  }
  auto aff = deps.listAffected(std::string(fdir) + "/" + whatFile, false);
  for (auto& kv : aff) {
    cout << ">>>> change in '" << whatFile << "' affected: " << kv.first << " (#include " << kv.second << ")\n";
    cmd = inidiff + " " + kv.first + " " + kv.first + ":?" + whatExt;
    if (debug) { cout << "     cmd: " << cmd << endl; }
    r = system(cmd.c_str());
  }
}
