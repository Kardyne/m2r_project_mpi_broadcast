#!/bin/bash

mkdir -p processed_data

for file in ./data/*.csv; do
    head -n 1 $file > ./processed_data/$(basename $file)
done