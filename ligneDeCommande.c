#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
// gcc Devoir1.c -o Devoir.exe -lm : Ajouter -lm pour faire la liaison entre le bibliotheqe math.h et 
// le compilateur

int puissance(int x)
{
        return (int)pow(2,x);
}

int dupliquer(int x)
{
        return 2*x;
}

int main( int argc , char* argv[])
{
    int opt;
    int testF,testG,resultat ;
    testF = testG = -1 ;

    if(argc < 3 || argc > 4)
    {
        if(argc < 3 )
                printf("Nombre d'Arguments Insuffisant.\n");
        if(argc > 4 )
                printf("Nombre d'Arguments Depasse.\n");
        printf("Usage : %s [ -f entier] || [ -g entier] || [-f][-g] entier [-f][-g] \n",argv[0]);
        exit(-1);
    }

    switch(argc)
    {   
        case 3 :
                while( (opt = getopt( argc ,argv ,":f:g:")) != -1 )
                {
                        switch(opt)
                        {

                                case 'f':testF = atoi(optarg);
                                        break;
                                case 'g':testG = atoi(optarg);
                                        break;
                                case ':':printf("Argument Manquant \n");
                                        break;
                                case '?':printf("Option Inconnu\n");
                                        break;
                                default:printf(" Usage : %s [-f entier] [-f entier] \n",argv[0]);
                                        exit(-1);
                        }
                }
                if (testF != -1)
                {
                        resultat = puissance(testF);
                }
                else if(testG != -1)
                {
                        resultat = dupliquer(testG);
                }
                break;
        case  4:
                if(strcmp(argv[1],"-f") ==0 && strcmp(argv[3],"-g") ==0)
                        resultat = puissance(dupliquer(atoi(argv[2])));
                else if(strcmp(argv[1],"-g") ==0 && strcmp(argv[3],"-f") ==0)
                        resultat = dupliquer(puissance(atoi(argv[2])));
                else{
                        printf("Une des commande fournie est INEXISTANTE.\n");
                        printf("Usage Correcte : %s [-f][-g] entier [-f][-g] \n",argv[0]);
                        return 0 ;
                }
                break;
    }
    
    printf("RESULTAT : %d \n\n",resultat);
  
}