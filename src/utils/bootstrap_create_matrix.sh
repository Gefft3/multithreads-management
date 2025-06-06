#!/bin/bash 

root='../../matrices/mock/'

matrix_a_rows=19
matrix_a_cols=20
matrix_a_filename=$root"A.txt"

matrix_b_rows=20
matrix_b_cols=19
matrix_b_filename=$root"B.txt"

# make sure the matrices directory exists
if [ ! -d "$root" ]; then
  mkdir -p "$root"
fi

python3 create_matrix.py $matrix_a_rows $matrix_a_cols $matrix_a_filename $matrix_b_rows $matrix_b_cols $matrix_b_filename

if [ $? -ne 0 ]; then
  echo "Error creating matrices."
  exit 1
fi
echo "Matrices created successfully."
