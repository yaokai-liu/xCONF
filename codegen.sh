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
python liu-xCONF/generate.py json
python xnf-json2C/codegen.py json template grammar/generated
