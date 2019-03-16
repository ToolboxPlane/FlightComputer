#!/usr/bin/env bash
gcc -fpreprocessed -dD -E main.cpp | sed '/^#/ d' | sed '/^[[:space:]]*$/d' | sed '1,1d' | sed '$d'

