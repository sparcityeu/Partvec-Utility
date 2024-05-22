/*
 This is a work in progress.
 Complexity can be improved.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

int main(int argc, char * argv[]) {

  int * permvec;
  char partvecFileName[256];
  memset(partvecFileName, 0, 256 * sizeof(char));
  strcat(partvecFileName, argv[1]);

  // Setting the name of the output file 

  char permvecFileName[256];
  memset(permvecFileName, 0, 256 * sizeof(char));
  strcat(permvecFileName, basename(argv[1]));
  permvecFileName[strlen(permvecFileName) - 8] = 'm';
  permvecFileName[strlen(permvecFileName) - 10] = 'e';

  // Setting parameters for convrsation

  int partvecIndexCounter[128] = {
    0
  };
  int parttvecIndexPointer[128] = {
    0
  };
  int partvecLength = 0;

  // Reading the partvec

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(partvecFileName, "r");
  if (fp == NULL) {
    printf("Failed to read partvec.\n");
    return 1;
  }

  while ((read = getline( & line, & len, fp)) != -1) {
    partvecIndexCounter[atoi(line) - 1] += 1;
    partvecLength += 1;
  }
  fclose(fp);

  FILE * fp2;
  char * line2 = NULL;
  size_t len2 = 0;
  ssize_t read2;

  fp2 = fopen(partvecFileName, "r");
  if (fp2 == NULL) {
    printf("Failed to read partvec.\n");
    return 1;
  }

  permvec = (int * ) calloc(partvecLength, sizeof(int));

  int currentRow = 0;
  while ((read2 = getline( & line2, & len2, fp2)) != -1) {
    int partnumber = atoi(line2);
    int partindex = 0;
    for (int i = 0; i < atoi(line2); i++) {
      if (i != atoi(line2)) {
        partindex += partvecIndexCounter[i - 1];
      }
    }
    partindex += parttvecIndexPointer[atoi(line2) - 1];
    parttvecIndexPointer[atoi(line2) - 1] += 1;
    permvec[partindex] = currentRow;
    currentRow += 1;
  }
  fclose(fp);

  FILE * permvec_fp;
  permvec_fp = fopen(permvecFileName, "w");
  for (int i = 0; i < partvecLength; i++) {
    fprintf(permvec_fp, "%d\n", permvec[i]);
  }
  fclose(permvec_fp);

  return 0;

}