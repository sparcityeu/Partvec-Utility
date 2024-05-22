/*
 This is a work in progress.
 Complexity can be improved.
 */

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "mmio.c"

int main(int argc, char *argv[])
{
    clock_t program_begins = clock();
    /* Declarations for reading the matrix */

    int ret_code;
    MM_typecode matcode;
    FILE *f, *p;
    int M, N, nz;
    int i, *I_complete, *J_complete, *permvec;
    float *val_complete;

    /* Declarations for measuring time */
    double elapsedTime = 0.0;
    double elapsed_time_patoh_part_only = 0.0;

    /* Processing the input */

    if (argc != 3)
    {
        printf("Could not process arguments.\n");
        printf("Usage: [martix-market-pathname] [permvec-pathname]\n");
        exit(1);
    }
    else
    {

        if ((f = fopen(argv[1], "r")) == NULL)
        {
            printf("Could not locate the matrix file. Please make sure the pathname is valid.\n");
            exit(1);
        }
    }

    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) != 0)
    {
        printf("Could not read matrix dimensions.\n");
        exit(1);
    }

    if ((strcmp(matcode, "MCRG") == 0) || (strcmp(matcode, "MCIG") == 0))
    {

        I_complete = (int *)calloc(nz, sizeof(int));
        J_complete = (int *)calloc(nz, sizeof(int));
        val_complete = (float *)calloc(nz, sizeof(int));

        for (i = 0; i < nz; i++)
        {
            fscanf(f, "%d %d %f", &I_complete[i], &J_complete[i], &val_complete[i]);
            fscanf(f, "%*[^\n]\n");
            /* adjust from 1-based to 0-based */
            I_complete[i]--;
            J_complete[i]--;
        }
    }

    /* If the matrix is symmetric, we need to construct the other half */

    else if ((strcmp(matcode, "MCRS") == 0) || (strcmp(matcode, "MCIS") == 0) || (strcmp(matcode, "MCPS") == 0))
    {

        I_complete = (int *)calloc(2 * nz, sizeof(int));
        J_complete = (int *)calloc(2 * nz, sizeof(int));
        val_complete = (float *)calloc(2 * nz, sizeof(int));

        int i_index = 0;

        for (i = 0; i < nz; i++)
        {
            fscanf(f, "%d %d %f", &I_complete[i], &J_complete[i], &val_complete[i]);
            fscanf(f, "%*[^\n]\n");

            if (I_complete[i] == J_complete[i])
            {
                /* adjust from 1-based to 0-based */
                I_complete[i]--;
                J_complete[i]--;
            }
            else
            {
                /* adjust from 1-based to 0-based */
                I_complete[i]--;
                J_complete[i]--;
                J_complete[nz + i_index] = I_complete[i];
                I_complete[nz + i_index] = J_complete[i];
                val_complete[nz + i_index] = val_complete[i];
                i_index++;
            }
        }
        nz += i_index;
    }
    else
    {
        printf("This matrix type is not supported: %s \n", matcode);
        exit(1);
    }

    fclose(f);

    if ((p = fopen(argv[2], "r")) == NULL)
    {
        printf("Could not locate the permvec file. Please make sure the pathname is valid.\n");
        exit(1);
    }

    int reordered_index = 0;

    // Final version will create .mtx file instead of .txt

    char reorderedMatName[256];
    memset(reorderedMatName, 0, 256 * sizeof(char));
    strcat(strcat(reorderedMatName, strcat(basename(argv[1]), "_reordered")), ".txt");

    FILE *reordered_matrix;
    reordered_matrix = fopen(reorderedMatName, "w");
    fprintf(reordered_matrix, "%d %d %d\n", M, N, nz);
    int rowCounter = 1;

    for (int i = 0; i < M; i++)
    {
        int permInstance = 0;

        fscanf(p, "%d", &permInstance);

        for (int j = 0; j < nz; j++)
        {
            if (I_complete[j] == permInstance)
            {
                /*
                I_reordered[reordered_index] = I_complete[j];
                J_reordered[reordered_index] = J_complete[j];
                val_reordered[reordered_index] = val_complete[j];
                reordered_index += 1;
                */
                fprintf(reordered_matrix, "%d %d %f\n", rowCounter, (J_complete[j] + 1), val_complete[j]);
            }
        }
        rowCounter += 1;
    }

    fclose(p);
    free(I_complete);
    free(J_complete);
    free(val_complete);
}