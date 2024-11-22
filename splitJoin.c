#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define BUFFER_SIZE 4096 // Taille Max Buffer

//Definition des fonction

//Fonction pour split
void split_file(const char *filename, size_t part_size);
//Fonction pour Fusionner
void join_files(const char *output_file, int part_count, char *part_files[]);

int main(int argc, char *argv[]) 
{
    if (argc < 3) {
        fprintf(stderr, "Usage : %s [<nom_fichier> <taille_partie> ] || [ nomFichierfusione fichier1 fichier2 ... ]\n", argv[0]);
        return 1;
    }
    if( argc == 3)
    {
        const char *filename = argv[1];
        size_t part_size = strtoul(argv[2], NULL, 10);
        if (part_size == 0)
        {
            fprintf(stderr, "La taille de la partie doit être un entier positif.\n");
            return EXIT_FAILURE;
        }
        split_file(filename, part_size);

        return EXIT_SUCCESS;
    }
    else
    {
        const char *output_file = argv[1];
        join_files(output_file, argc - 2, &argv[2]);
        return EXIT_SUCCESS;
    }

}
//Fonction pour split
void split_file(const char *filename, size_t part_size)
{
    int fd = open(filename, O_RDONLY);
    if(fd < 0)
    {
        perror("Erreur lors de l'ouverture du fichier source");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    size_t current_size = 0;
    int part_number = 1;

    char part_filename[256];
    int part_fd = -1;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) 
    {
        for (ssize_t i = 0; i < bytes_read; i++)
        {
            if (current_size == 0)
            {
                // Créer un nouveau fichier pour une nouvelle partie
                snprintf(part_filename, sizeof(part_filename), "%s.part%d", filename, part_number);
                part_fd = open(part_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (part_fd < 0) {
                    perror("Erreur lors de la création d'un fichier de partie");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
                printf("Création du fichier : %s\n", part_filename);
                part_number++;
            }
            // Écrire l'octet courant dans le fichier de la partie
            if(write(part_fd, &buffer[i], 1) < 0) 
            {
                perror("Erreur lors de l'écriture d'une partie");
                close(fd);
                close(part_fd);
                exit(EXIT_FAILURE);
            }
            current_size++;
            // Si la taille maximale de la partie est atteinte, fermer le fichier
            if (current_size >= part_size) 
            {
                close(part_fd);
                part_fd = -1;
                current_size = 0;
            }
        }
    }
    if (part_fd >= 0) {
        close(part_fd);
    }
    if (bytes_read < 0) {
        perror("Erreur lors de la lecture du fichier source");
    }

    close(fd);
}
//Fonction pour Fusionner
void join_files(const char *output_file, int part_count, char *part_files[]) 
{
    int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0) 
    {
        perror("Erreur lors de la création du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    for (int i = 0; i < part_count; i++) 
    {
        int part_fd = open(part_files[i], O_RDONLY);
        if (part_fd < 0) 
        {
            perror("Erreur lors de l'ouverture d'un fichier de partie");
            close(out_fd);
            exit(EXIT_FAILURE);
        }
        printf("Fusion du fichier : %s\n", part_files[i]);

        while ((bytes_read = read(part_fd, buffer, BUFFER_SIZE)) > 0) 
        {
            if (write(out_fd, buffer, bytes_read) < 0) 
            {
                perror("Erreur lors de l'écriture dans le fichier de sortie");
                close(part_fd);
                close(out_fd);
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read < 0)
        {
            perror("Erreur lors de la lecture d'un fichier de partie");
            close(part_fd);
            close(out_fd);
            exit(EXIT_FAILURE);
        }
        close(part_fd);
    }
    close(out_fd);
    printf("Fichier fusionné avec succès dans : %s\n", output_file);
}
