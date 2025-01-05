#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct 
{
    char nom[50];
    char prenom[50];
    int montant;
}client;

// Prototypes

client saisie ();
void affiche (client);

int main (void)
{
    client newClient = saisie();
    affiche (newClient);

    return EXIT_SUCCESS;
}

client saisie ()
{
    client newClient;
    char tmpNom[50];
    char tmpPrenom[50];
    printf("Saisir votre nom : \n");
    scanf(" %s",tmpNom);
    strcpy(newClient.nom, tmpNom);
    printf("Saisir votre prenom : \n");
    scanf(" %s",tmpPrenom);
    strcpy(newClient.prenom, tmpPrenom);
    printf("Enter le montant de depot: \n");
    scanf(" %d", &(newClient.montant));
    return newClient;
}

void affiche (client newClient)
{
    printf ("Votre nom: %s\n",newClient.nom);
    printf ("Votre prenom: %s\n",newClient.prenom);
    printf("Votre montant: %d\n",newClient.montant);
    return;
}
