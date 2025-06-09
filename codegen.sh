#!/usr/bin/env bash

if [ -d "json/xJSON" ]; then
  echo ''
else
  mkdir -p json/xJSON
fi
if [ -d "grammar/generated/xJSON" ]; then
  echo ''
else
  mkdir -p grammar/generated/xJSON
fi
python liu-xJSON/generate.py json
python xnf-json2C/codegen.py json template grammar/generated
