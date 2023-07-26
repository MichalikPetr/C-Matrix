#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matice.h"


matice inicializace(int m, int n)
{
    matice mat;
    mat.m = m;
    mat.n = n;

    mat.data = malloc(m * sizeof(float *));
    if (mat.data == NULL)
    {
        chyba = CHYBA_ALOKACE;
        return mat;
    }

    for (int i = 0; i < m; i++)
    {
        mat.data[i] = malloc(n * sizeof(float));
        if (mat.data[i] == NULL)
        {
            chyba = CHYBA_ALOKACE;
            return;
        }
    }

    chyba = BEZ_CHYBY;
    return mat;
}

matice nulova(int m, int n)
{
    matice mat = inicializace(m, n);
    if (chyba != BEZ_CHYBY)
    {
        return mat;
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mat.data[i][j] = 0;
        }
    }

    chyba = BEZ_CHYBY;
    return mat;
}
matice jednotkova(int m, int n)
{
    matice mat = inicializace(m, n);
    if (chyba != BEZ_CHYBY)
    {
        return mat;
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j) {mat.data[i][j] = 1;}
            else {mat.data[i][j] = 0;}
        }
    }

    chyba = BEZ_CHYBY;
    return mat;
}

void odstran(matice mat)
{
    if (mat.m == NULL || mat.n == NULL)
    {
        chyba = CHYBA_ALOKACE;
        return;
    }
    for (int i = 0; i < mat.m; i++)
    {
        free(mat.data[i]);
        mat.data[i] = NULL;
    }
    free(mat.data);
    mat.data = NULL;

    chyba = BEZ_CHYBY;
}

void vypis(matice mat)
{
    for (int i = 0; i < mat.m; i++)
    {
        for (int j = 0; j < mat.n; j++)
        {
            printf("%f ", mat.data[i][j]);
        }
        printf("\n");
    }

    chyba = BEZ_CHYBY;
}

matice plus(matice mat1, matice mat2)
{
    matice result;
    if ((mat1.m != mat2.m) || (mat1.n != mat2.n))
    {
        chyba = CHYBA_TYPU;
        return result;
    }

    result = inicializace(mat1.m, mat1.n);
    if (chyba != BEZ_CHYBY)
    {
        return result;
    }

    for (int i = 0; i < result.m; i++)
    {
        for (int j = 0; j < result.n; j++)
        {
            result.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
        }
    }

    chyba = BEZ_CHYBY;
    return result;
}

matice minus(matice mat1, matice mat2)
{
    matice result;
    if ((mat1.m != mat2.m) || (mat1.n != mat2.n))
    {
        chyba = CHYBA_TYPU;
        return result;
    }

    result = inicializace(mat1.m, mat1.n);
    if (chyba != BEZ_CHYBY)
    {
        return result;
    }

    for (int i = 0; i < result.m; i++)
    {
        for (int j = 0; j < result.n; j++)
        {
            result.data[i][j] = mat1.data[i][j] - mat2.data[i][j];
        }
    }

    chyba = BEZ_CHYBY;
    return result;
}
matice nasobeni(matice mat, float skalar)
{
    matice result = inicializace(mat.m, mat.n);
    if (chyba != BEZ_CHYBY)
    {
        return result;
    }

    for (int i = 0; i < result.m; i++)
    {
        for (int j = 0; j < result.n; j++)
        {
            result.data[i][j] = mat.data[i][j] * skalar;
        }
    }

    chyba = BEZ_CHYBY;
    return result;
}

matice transpozice(matice mat)
{
    matice result = inicializace(mat.n, mat.m);
    if (chyba != BEZ_CHYBY)
    {
        return result;
    }

    for (int i = 0; i < mat.m; i++)
    {
        for (int j = 0; j < mat.n; j++)
        {
            result.data[j][i] = mat.data[i][j];
        }
    }

    chyba = BEZ_CHYBY;
    return result;
}

matice krat(matice mat1, matice mat2)
{
    matice result;
    if (mat1.n != mat2.m)
    {
        chyba = CHYBA_TYPU;
        return result;
    }

    result = inicializace(mat1.m, mat2.n);
    if (chyba != BEZ_CHYBY)
    {
        return result;
    }

    for (int i = 0; i < result.m; i++)
    {
        for (int j = 0; j < result.n; j++)
        {
            result.data[i][j] = 0;
            for (int k = 0; k < mat1.n; k++)
            {
                result.data[i][j] += mat1.data[i][k] * mat2.data[k][j];
            }
        }
    }

    chyba = BEZ_CHYBY;
    return result;
}

matice nacti_ze_souboru(const char *soubor)
{
    FILE *maticeSoubor;
    maticeSoubor = fopen(soubor, "r");
    matice result;
    if (maticeSoubor == NULL)
    {
        chyba = CHYBA_OTEVRENI;
        return result;
    }
    int m = 0;
    int n = 0;
    char lineBuffer[BUFFER_SIZE];
    char *token;

    while (fgets(lineBuffer, BUFFER_SIZE, maticeSoubor))
    {
        m++;
    }

    token = strtok(lineBuffer, " ");
    while (token)
    {
        token = strtok(NULL, " ");
        n++;
    }

    rewind(maticeSoubor);

    result = inicializace(m, n);
    if (chyba != BEZ_CHYBY)
    {
        return result;
    }

    m = 0;
    n = 0;

    while (fgets(lineBuffer, BUFFER_SIZE, maticeSoubor))
    {
        token = strtok(lineBuffer, " ");
        while (token)
        {
            result.data[m][n] = atof(token);
            token = strtok(NULL, " ");
            n++;
        }
        m++;
        n = 0;
    }

    maticeSoubor = fclose(maticeSoubor);
    if (maticeSoubor == EOF)
    {
        chyba = CHYBA_ZAVRENI;
        return result;
    }

    chyba = BEZ_CHYBY;
    return result;
}

void uloz_do_souboru(matice mat, const char *soubor)
{
    FILE *maticeSoubor;
    maticeSoubor = fopen(soubor, "w");
    if (maticeSoubor == NULL)
    {
        chyba = CHYBA_OTEVRENI;
        return;
    }

    char numberBuffer[BUFFER_SIZE];

    for (int i = 0; i < mat.m; i++)
    {
        for (int j = 0; j < mat.n; j++)
        {
            sprintf(numberBuffer, "%f ", mat.data[i][j]);
            fwrite(numberBuffer, sizeof(char), strlen(numberBuffer), maticeSoubor);
        }
        fwrite("\n", sizeof(char), 1, maticeSoubor);
    }

    maticeSoubor = fclose(maticeSoubor);
    if (maticeSoubor == EOF)
    {
        chyba = CHYBA_ZAVRENI;
        return;
    }
    chyba = BEZ_CHYBY;
}

int velikost(matice mat, int dimenze)
{
    int result;
    switch (dimenze)
    {
    case 0:
        result = mat.n;
        break;
    case 1:
        result = mat.m;
        break;
    default:
        chyba = CHYBA_JINA;
        return;
    }
    return result;
}

float prvek(matice mat, int i, int j)
{
    float result;
    if (i > mat.m || j > mat.n || i < 0 || j < 0)
    {
        chyba = CHYBA_JINA;
        return result;
    }

    chyba = BEZ_CHYBY;
    result = mat.data[i][j];
    return result;
}

void nastav_prvek(matice mat, int i, int j, float hodnota)
{
    if (i > mat.m || j > mat.n || i < 0 || j < 0)
    {
        chyba = CHYBA_JINA;
        return;
    }

    chyba = BEZ_CHYBY;
    mat.data[i][j] = hodnota;
}
