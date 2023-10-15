/*
 * Search space for pattern closest to given pattern.
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include "RDtree.hpp"
#include "fileio.h"

/* test parameters */
#define NUM_VALS 50
#define NUM_PATTERNS 100000
#define MAX_FIND 1
#define NUM_SEARCH NUM_PATTERNS

/* distance function */
float Dist(void *v1, void *v2)
{
    register int i;
    float dist,d;
    unsigned char *p1,*p2;

    p1 = (unsigned char *)v1;
    p2 = (unsigned char *)v2;
    for (i = 0, dist = 0.0f; i < NUM_VALS; i++)
    {
        d = (float)(p1[i] - p2[i]);
        if (d < 0.0f) d = -d;
        dist += d;
    }
    return(dist);
}

/* load pattern */
void *loadPatt(FILE * fp)
{
    unsigned char *p = new unsigned char[NUM_VALS];
    unsigned char c;
    for (int i = 0; i < NUM_VALS; i++)
    {
      FREAD_CHAR(&c, fp);
      p[i] = c;
    }
    return p;
}

/* save pattern */
void savePatt(void *pattern, FILE * fp)
{
    unsigned char *p = (unsigned char *)pattern;
    unsigned char c;
    for (int i = 0; i < NUM_VALS; i++)
    {
      c = p[i];
      FWRITE_CHAR(&c, fp);
    }
}

void PrintPattern(unsigned char *);
void PrintPattern2(void *p, FILE *fp);

int main(int argc, char *argv[])
{
    int i,j;
    unsigned char s[NUM_VALS],t[NUM_VALS],*p;
    double d,d2;
    RDtree *tree;
    RDtree::RDsearch *searchList;

    int seed = (int)time(NULL);
    printf("Seed=%d\n", seed);
    srand(seed);

    /* initialize search pattern */
    for (j = 0; j < NUM_VALS; j++)
    {
        s[j] = (rand() % 27) + 'a';
    }
    printf("Search pattern:\n");
    PrintPattern(s);

    /* add patterns to tree */
    tree = new RDtree(Dist);
    d = -1.0;
    for (i = 0; i < NUM_PATTERNS; i++)
    {
        p = new unsigned char[NUM_VALS];
        for (j = 0; j < NUM_VALS; j++)
        {
            p[j] = (rand() % 26) + 'a';
        }
        tree->insert(p,NULL);
        d2 = Dist((void *)s,(void *)p);
        if (d < 0.0 || d2 < d) d = d2;
    }

    /* add a search target pattern */
    for (j = 0; j < NUM_VALS; j++)
    {
        t[j] = s[j];
        /*		if ((rand() % 10) == 0) t[j] = (rand() % 26) + 'a'; */
        if ((rand() % 5) == 0)
        {
            if (rand() % 2) t[j] += rand() % 5; else t[j] -= rand() % 5;
        }
    }
    tree->insert(t,NULL);
    printf("Target:\n");
    PrintPattern(t);

    //tree->remove(t);

    //printf("Tree:\n");
    //tree->print(NULL, PrintPattern2);

    // save and load.
    //printf("Save:\n");
    //tree->save("treesave.out", savePatt);
    //printf("Load:\n");
    //tree->load("treesave.out", loadPatt);

    /* search */
    printf("Search:\n");
    searchList = tree->search(s,MAX_FIND,NUM_SEARCH);

    /* print search results */
    printf("Result:\n");
    PrintPattern((unsigned char *)searchList->node->pattern);
    printf("Distance=%f\n", searchList->distance);
#ifdef WIN32
	getchar();
#endif

    return(0);
}


/* print pattern */
void PrintPattern(unsigned char *p)
{
    for (int j = 0; j < NUM_VALS; j++)
    {
        printf("%c", p[j]);
    }
    printf("\n");
}

/* print pattern */
void PrintPattern2(void *p, FILE *fp)
{
  unsigned char *p2 = (unsigned char *)p;
    for (int j = 0; j < NUM_VALS; j++)
    {
        fprintf(fp, "%c", p2[j]);
    }
}

