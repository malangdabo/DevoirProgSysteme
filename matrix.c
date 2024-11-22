#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//Explicite l'utulisation du Progrmme
void usage();
//Focntion de Creation de Matrice 
int ** creerMatrice(int);
//Destruction  de la Mtarice
void destruction(int **,int);

int main(int argc ,char * argv[])
{
    //Declaration de la matrice
    int **Matrice = NULL ;

    int opt,testC,testA,testD,testB ,dimension ,fichier;
    testC = testA = dimension =0 ;
    testB = 'b' ;
    char *nomFichier=NULL;

    while( (opt = getopt( argc ,argv ,"cad:btf:")) != -1 )
    {
        switch(opt)
        {
            case 'c': testC = 1; break ; //Cree Matrice de Dimension Aleatoir
            case 'a': testA = 1 ; break ; // Afficher contenue du Fichier
            case 'd': dimension = atoi(optarg); break ; //Dimension donnee
            case 'b': testB = 1; break; // En fichier Binaire
            case 't': testB = 1; break;// En fichier Texte
            case 'f': nomFichier = optarg; break; // Nom du fichier 
            case ':': printf("Argument Manquant \n");break;
            case '?': printf("Option Inconnu\n");break;
            default:
                usage();return 1;
        }
    }
    
    // if( argc < 4 || argc > 7)
    // {
	// 	printf(" Erreur : Nombre d'arguemnts Insuffisant\n");
	// 	usage();
    //     return 0;
	// }

    if(dimension == 0 || nomFichier == NULL)
    {
        printf("La Dimension ou le Nom Du fichier doit etre DONNE.\n");
        usage();
        return 1;
    }
    //Lorsque la Creation de Matrice est Necessaire
    if(testC)
    {
        //Creation de la Matrice
        Matrice=creerMatrice(dimension);
         //Ouverture du Fichier
        fichier= open(nomFichier, O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
        if(fichier == -1 )
        {
            perror("Erreur lors de l'Ouverture du Fichier en Ecriture Seule  .\n");
            return  0;	
        }

        if( testB == 'b')
        {
            if(write(fichier,&dimension ,sizeof(int)) < 0 )
            { 
                printf("Erreur d'ecriture des Dimensions de la Matrice\n");
                return 0 ;
            }
            for(int j=0 ; j < dimension ; ++j)
            {
                if(write(fichier,Matrice[j],dimension*sizeof(int)))
                {
                    perror("Erreur d'Ecriture sur le Fichier Binaire");
                    close(fichier);
                    return 0 ;
                }
            }
            close(fichier); 
             int teste ;
            for(int j=0 ; j < dimension ; ++j)
            {
                teste = write(fichier,Matrice[j],dimension*sizeof(int));
                if(teste == 0)
                {
                    perror("Erreur d'Ecriture sur le Fichier Binaire \n");
                    return 0 ;
                }
            }

        }
        else 
        {
            int testeTexte = 0 ;
            char textBuff[255];//buffer texte
            for(int i=0 ; i < dimension ; ++i)
            {
                for(int j=0 ; j < dimension ; ++j)
                {
                    //Construction d'une Chaine a partir des variables
                    sprintf(textBuff,"%d", Matrice[i][j]);
                    //ecriture de la chaine
                    testeTexte = write (fichier,textBuff, strlen(textBuff)); 
                    if(testeTexte < 0 )
                    {
                        perror("Erreur d'Ecriture dans le Fichier \n");
                        close(fichier);
                        return 0 ;
                    }
                }
                if(write(fichier, "\n",1) < 0)
                {
                    perror("Erreur d'Ecriture dans le Fichier \n");
                    close(fichier);
                    return 0 ;
                }
            }
            close(fichier);
        }
    }
    if(testA)
    {
        fichier= open(nomFichier, O_RDONLY);
        if(fichier == -1 )
        {
            perror("Erreur lors de l'Ouverture du fichier en Lecture Seule.\n");
            return  0;	
        }
        //Cas Fichier Binaire
        if( testB == 'b')
        {
            //Lire les Dimensions
            int dim;
            if(read(fichier,&dim,sizeof(int)) < 0)
            {
                perror("Erreur lors de la lecture du fichier.\n");
                close(fichier);
                return  0;
            }
            printf("La Matrice est de dimensions : %d X %d .\n",dim,dim);
            
            //Lire les Donnees de la Matrice
            //Matrice poue le stockage des Donnees du fichier
            int Matrice1[dimension][dimension]; 
            
            for(int j=0 ; j < dimension ; j++)
            {
                while(read(fichier,Matrice1[j],sizeof(int)) < 0)
                {
                    perror("Erreur lors de Lecture sur le Fichier.\n");
                    close(fichier);
                    return 0 ;
                }
            }
            //Fermeture du FICHIER 
            close(fichier);
            //afficher la Matrice
            printf("\nLa Matrice : \n");

            for(int i=0 ; i < dimension ; ++i)
            {
                for(int j=0 ; j < dimension ; ++j)
                {
                    printf("%d",Matrice1[i][j]);

                }
                printf("\n");
            }
        }
        else
        {
            char buff[255];
            ssize_t nbRead;
     
            while( (nbRead = read(fichier,buff, 255 -1)) > 0)
            {
                buff[nbRead] = '\0';
                printf("%s", buff);
            }
            if( nbRead < 0)
            {
                perror("Erreur lors de Lecture sur le Fichier.\n");
            }
            //Fermeture du  FICHIER 
            close(fichier);
        }
    }
    //Liberation de la Matrice    
    destruction(Matrice,dimension);
    return 0 ;
}

//**********************************|   Definition de FONCTIONS   |************************************


//Explicite l'utulisation du Progrmme
void usage()
{
    printf("./matrix.exe [-c] [-a] -d <Dimension> [-f] nomFichier [-b] [-t].\n");
    printf("-c : Cree une Matrice Aleatoire .\n");
    printf("-a : Afficher le contenu du Fichier .\n");
    printf("-d : Dimension de la Matrice  .\n");
    printf("-b : Format Binaire (format par defaut).\n");
    printf("-t : Format Texte .\n");
    printf("-f : Nom du Fichier .\n");
}

//Fonction de Creation de Matrice 
int ** creerMatrice(int taille)
{
    int **matrice;
    int k =0;      //Inserer la Matrice avec les valeurs 1,2,3,...
    matrice = malloc(taille*sizeof(int*));
    if( !(matrice))
    {
        printf("Erreur lors de l'allocation de la Matrice\n");
        exit(-1);
    }
    for(int i = 0 ; i < taille ; i++)
    {
        (matrice)[i] = malloc(sizeof(int));
        if( !(matrice)[i])
        {
            printf("Erreur lors de l'allocation\n");
            exit(-1);
        }
    }
    for(int i = 0 ; i < taille ; i++)
    {
        for(int j = 0 ; j < taille ; j++)
            matrice[i][j] = ++k ;// On met des entier dans la matrice creee de 1 ,2,...
    }
    return matrice;
}

//Destruction de la Matrice
void destruction(int ** matrice,int taille)
{
    if(matrice)
    {
        for(int i = 0 ; i < taille ; i++)
            free(matrice[i]);
        free(matrice);
    }
}



// if(testC)
//     {
//         //Creation de la Matrice
//         Matrice=creerMatrice(dimension);
//         if( testB == 'b')
//         {
//             int teste ;
//             for(int j=0 ; j < dimension ; ++j)
//             {
//                 teste = write(fichier,Matrice[j],dimension*sizeof(int));
//                 if(teste == 0)
//                 {
//                     perror("Erreur d'Ecriture");
//                     return 0 ;
//                 }
//             }

//         }
//         else 
//         {
//             int testeTexte = 0 ;
//             char textBuff[255];//buffer texte
//             for(int i=0 ; i < dimension ; ++i)
//             {
//                 for(int j=0 ; j < dimension ; ++j)
//                 {
//                     //Construction d'une Chaine a partir des variables
//                     sprintf(textBuff,"%d", Matrice[i][j]);
//                     //ecriture de la chaine
//                     testeTexte = write (fichier,textBuff, strlen(textBuff)); 
//                     if(testeTexte <= 0 )
//                     {
//                         perror("Erreur d'Ecriture\n");
//                         return 0 ;
//                     }
//                 }
//             }

//         }
//     }
//     else if (testA)
//     {
//         if( testB == 'b')
//         {
//             int nbRead ;
//             while(read(fichier,&nbRead,sizeof(int)) && fichier != EOF)
//                 printf("%d\n",nbRead);
//         }
//         else
//         {
//             char buff;
//             int nbRead = 0 ;
//             do{
            
//                 nbRead = read(fichier,&buff, sizeof(char));                
//                 if(nbRead > 0)
//                     printf("%c", buff);
                
//             } while(nbRead > 0 );
//         }
//     }
//     //Fermeture du  FICHIER 
//     close(fichier);
//     //Liberation de la Matrice    
//     destruction(Matrice,dimension);
//     return 0 ;
