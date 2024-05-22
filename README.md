# Partvec-Utility

This repository contains two separate programs, and necessary files to compile them::


- **part_to_perm.c**: This program converts a partition vector, represented as a list of integers, into a permutation vector.

- **perm_to_mat.c**: This program applies a permutation vector to a matrix read in Matrix Market format and outputs a new matrix with rows reordered with respect to the provided permutation vector.

## Compilation and Usage of part_to_perm.c

1. **Compilation**: Compile the code using GCC.

2. **Execution**: Run the program with the path to the partition vector file "./a.out [path-to-partvec]".

3. **Output**: The program generates a file containing the permutation vector. The output filename is created from the input filename with the last 8 characters altered to end in 'permvec' (program assumes the provided partitioning vector file name ends with "partvec"). 

## Notes
- Make sure that the input file format is correct and contains one integer per line.
- Future versions of this program may include optimisations.



## Compilation and Usage of perm_to_mat.c

1. **Compilation**: Compile the code using GCC.


2. **Execution**: Run the program by providing the path to the Matrix Market file and the permutation vector file "./a.out [path-to-matrix] [path-to-permvec]".

3. **Output**: The program generates a file containing the permutated matrix. The output filename is created from the input matrix filename with `_reordered.txt` added to the end. 

## Notes

- The input matrix must be in the Matrix Market Format, and the permutation vector file should contain one integer per line.
- The program does not check whether the provided permutation vector contains invalid or contradicting permutations.
- The program currently outputs the resulting matrix as a text file instead of Matrix Market Format.
- Future versions of this program may include optimisations.
