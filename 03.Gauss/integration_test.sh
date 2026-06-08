#!/bin/bash

status=true

cat > input.csv <<'CSV'
a1,a2,b
2,1,4
1,3,7
CSV

cat > expected.csv <<'CSV'
x
1.000000
2.000000
CSV

./gauss input.csv actual.csv

if cmp -s expected.csv actual.csv; then
  echo "CSV solve ok!"
else
  echo "CSV solve failed!" >&2
  status=false
fi

$status
