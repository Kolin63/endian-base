#!/usr/bin/env sh

# makes an include directory that has everything that a mod would need in its
# include directory

online_dir="$1"

if [[ -z "$online_dir" || "$online_dir" == "-h" || "$online_dir" == "--help" ]]; then
  echo "Usage: $0 [endian online directory]"
  exit 0
fi

if [[ -d "include" ]]; then
  rm include/*
else
  mkdir "include"
fi

ln -s "${online_dir}/src/api.h" "include"
ln -s "${online_dir}/src/bot.h" "include"
ln -s "${online_dir}/src/function.h" "include"
ln -s "${online_dir}/src/namespace.h" "include"
ln -s "${online_dir}/dependencies/registry.h" "include"
ln -s "${online_dir}/src/save.h" "include"
ln -s "${online_dir}/src/user.h" "include"
ln -s "${online_dir}/src/jsmn_iterator.h" "include"
ln -s "${online_dir}/src/json_macros.h" "include"
