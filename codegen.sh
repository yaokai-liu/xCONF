#!/usr/bin/env bash

if [ -d "json/xCONF" ]; then
  echo ''
else
  mkdir -p json/xCONF
fi
if [ -d "grammar/generated/xCONF" ]; then
  echo ''
else
  mkdir -p grammar/generated/xCONF
fi
if [ -d "json/Path" ]; then
  echo ''
else
  mkdir -p json/Path
fi
if [ -d "grammar/generated/Path" ]; then
  echo ''
else
  mkdir -p grammar/generated/Path
fi
python liu-xCONF/generate.py json
python xnf-json2C/codegen.py json template grammar/generated
