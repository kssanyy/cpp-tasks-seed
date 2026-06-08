#!/bin/bash

set -e

./gauss AB.csv solution.csv
cmp solution.csv expected.csv
