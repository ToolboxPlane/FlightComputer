#!/usr/bin/env bash
code=$(gcc -fpreprocessed -dD -E main.cpp | sed '/^#/ d' | sed '/^[[:space:]]*$/d' | sed '1,1d' | sed '$d')
python Scripts/graph.py "${code}"
dot -Tpdf -o /tmp/graph.pdf tmp.dot
rm tmp.dot
xdg-open /tmp/graph.pdf
