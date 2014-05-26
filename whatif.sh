#!/bin/bash

# use awk here to guarantee 2 strings per line separated with one space
grep -Hr "^[ \t]*#include " site | sed 's/:#include / /' | awk '{print $1,$2}' | ./whatif $*
