#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include <netdb.h>

void displayClientMenu();
void handleClientQuery(int socketClient, int typeQuery);
bool askEnd(int socketClient);
void traitementCaseOne(char *myBook);

int main(int argc, char *argv[])
{
    // create the client part of the tcpi ip server
    struct sockaddr_in addrClient;
    int socketClient;
    const char *namehost = "F203-24";
    // def the structure of the server
    struct hostent *infos_server = NULL;

    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient == -1)
    {
        printf("Error creating socket");
        exit(-1);
    }

    infos_server = gethostbyname(namehost);
    // check if infos_server is still null
    if (infos_server == NULL)
    {
        printf("Error getting host by name");
        exit(-1);
    }

    // give value to the addrClient
    addrClient.sin_family = AF_INET;
    memcpy(&addrClient.sin_addr.s_addr, infos_server->h_addr, sizeof(u_long));
    addrClient.sin_port = htons(9999);

    // connect to the server
    if (connect(socketClient, (struct sockaddr *)&addrClient, sizeof(addrClient)) == -1)
    {
        printf("Error connecting to server");
        exit(-1);
    }
    else
    {
        printf("Connected to server \n");
    }
    //================================================================================================
    bool isRunning = true;

    // debut reception données et envoie de reponse
    while (isRunning)
    {
        // test reception menu
        displayClientMenu();
       /* char messageRecu[282];
        read(socketClient, &messageRecu, 282);
        printf("%s", messageRecu);*/

        // char *str = "Hello world";
        // write(socketClient, &str, strlen(str));

        //envoie de la reponse 1/2/3/4
        int typeQuery = 0;
        scanf("%d", &typeQuery);
        write(socketClient, &typeQuery, sizeof(int));







        // reception de sendResponseToQuery
        printf("Vous avez selectionner la requete 1, veuillez entrer une reference. \nPar exemple: 10 \n");
        /*int size = 0;
        read(socketClient, &size, sizeof(int));
        printf("size = %d \n", size);

        char msgAns[size];
        read(socketClient, msgAns, size);
        printf("%s \n", msgAns);*/
        //=============================






        handleClientQuery(socketClient, typeQuery);






        isRunning = askEnd(socketClient);

        // envoie 0 au serveur si isRunning = true sinon envoie 1 au serveur
        int endClient = 0;
        if (isRunning == true)
        {
            endClient = 0;
        }
        else
        {
            endClient = 1;
        }
        write(socketClient, &endClient, sizeof(int));
    }
    //=============

    close(socketClient);
    return 0;
}

// function

int searchSize(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i + 1;
}

void sendMessage(int socketClient, char *message)
{
    int size = searchSize(message) * sizeof(char);
    write(socketClient, &size, sizeof(int));
    write(socketClient, *&message, size);
}

void handleClientQuery(int socketClient, int typeQuery)
{
    switch (typeQuery)
    {
    case 1:
//envoie de la reference envoyé par le client
        char *reference = (char *)malloc((4) * sizeof(char));
        scanf("%s", reference);
        sendMessage(socketClient, reference);
//================================================================================================
       
       
       
        // reception de queryTreatment
        int size = 0;
        read(socketClient, &size, sizeof(int));
        printf("size = %d \n", size);
        char myBook[size];
        read(socketClient, &myBook, size);
        printf("%s \n", myBook);

        traitementCaseOne(myBook);
        break;

        /*case 2:
            break;
        case 3:
            break;
        case 4:
            break;*/
    }
}

bool askEnd(int socketClient)
{

    bool res = true;
    // reception du message questionnant si le client veut continuer
   /* int size = 0;
    read(socketClient, &size, sizeof(int));
    printf("size = %d \n", size);
    char endTreatment[size];
    read(socketClient, &endTreatment, size);
    printf("%s \n", endTreatment);*/
//================================================================================================
    
    printf("Si vous n'avez plus de questions tapez 'oui' sinon tapez 'non'\n");
    
    char *answer;
    scanf("%s", answer);
    if (strcmp(answer, "non") == 0)
    {
        res = false;
    }
    return res;
}

void traitementCaseOne(char *myBook)
{
    char cRef[4], cAuthor[50], cTitle[50], cType[50], cNbPages[50], cRate[50];

    strcpy(cRef, strtok(myBook, "&"));
    strcpy(cAuthor, strtok(NULL, "&"));
    strcpy(cTitle, strtok(NULL, "&"));
    strcpy(cType, strtok(NULL, "&"));
    strcpy(cNbPages, strtok(NULL, "&"));
    strcpy(cRate, strtok(NULL, "&"));

    printf("Reference: %s \n", cRef);
    printf("Author: %s \n", cAuthor);
    printf("Title: %s \n", cTitle);
    printf("Type: %s \n", cType);
    printf("NbPages: %s \n", cNbPages);
    printf("Rate: %s \n", cRate);

    if (atoi(cNbPages) > 300)
    {
       printf("\nLe nombre page du livre est superieur à 300 !!!! \n");
    }
    else
    {
        printf("\nLe nombre de pages inferieur à 300 !! \n ");
    }
}

void displayClientMenu()
{
    printf("Quel type de requete voulez vous faire ?\n1.  Recherche par reference \n2.  Recherche par mot clé \n3.  Recherche par auteur et par genre littéraire\n4.  Recherche par auteur et par critère: nombre de pages ou note des lecteurs\n Entrez le numero de la requete souhaité (1/2/3/4): \n");
}
