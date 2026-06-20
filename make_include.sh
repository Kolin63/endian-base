#!/usr/bin/env sh

# makes an include directory that has everything that a mod would need in its
# include directory

engine_dir="$1"

if [[ -z "$engine_dir" || "$engine_dir" == "-h" || "$engine_dir" == "--help" ]]; then
  echo "Usage: $0 [endian engine directory]"
  exit 0
fi

if [[ -d "include" ]]; then
  rm include/*
else
  mkdir "include"
fi

ln -s "${engine_dir}/src/api.h" "include"
ln -s "${engine_dir}/src/bot.h" "include"
ln -s "${engine_dir}/src/function.h" "include"
ln -s "${engine_dir}/src/fid.h" "include"
ln -s "${engine_dir}/src/namespace.h" "include"
ln -s "${engine_dir}/dependencies/registry.h" "include"
ln -s "${engine_dir}/src/save.h" "include"
ln -s "${engine_dir}/src/user.h" "include"
ln -s "${engine_dir}/src/jsmn_iterator.h" "include"
ln -s "${engine_dir}/src/json_macros.h" "include"
