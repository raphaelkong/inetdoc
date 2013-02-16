#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>

#define MAX_MSG 80
#define MSG_ARRAY_SIZE (MAX_MSG+1)
// Utilisation d'une constante x dans la définition
// du format de saisie
#define str(x) # x
#define xstr(x) str(x)

int main()
{
  int socketDescriptor;
  int msgLength;
  unsigned short int serverPort;
  struct sockaddr_in serverAddress;
  struct hostent *hostInfo;
  char msg[MSG_ARRAY_SIZE];

  puts("Entrez le nom du serveur ou son adresse IP : ");

  memset(msg, 0, sizeof msg);  // Mise à zéro du tampon
  scanf("%"xstr(MAX_MSG)"s", msg);

  // gethostbyname() reçoit un nom d'hôte ou une adresse IP en notation
  // standard 4 octets en décimal séparés par des points puis renvoie un
  // pointeur sur une structure hostent. Nous avons besoin de cette structure
  // plus loin. La composition de cette structure n'est pas importante pour
  // l'instant.
  if ((hostInfo = gethostbyname(msg)) == NULL) {
    herror("gethostbyname:");
    exit(EXIT_FAILURE);
  }

  puts("Entrez le numéro de port du serveur : ");
  scanf("%hu", &serverPort);

  // Création de socket. "PF_INET" correspond à la famille de protocole IPv4.
  // "SOCK_DGRAM" correspond à un service de datagramme non orienté connexion.
  // "IPPROTO_UDP" désigne le protocole UDP utilisé au niveau transport.
  if ((socketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    perror("socket:");
    exit(EXIT_FAILURE);
  }

  // Initialisation des champs de la structure serverAddress
  serverAddress.sin_family = hostInfo->h_addrtype;
  memcpy((char *) &serverAddress.sin_addr.s_addr,
         hostInfo->h_addr_list[0], hostInfo->h_length);
  serverAddress.sin_port = htons(serverPort);

  if (connect(socketDescriptor,
             (struct sockaddr *) &serverAddress,
             sizeof(serverAddress)) == -1) {
    perror("connect");
    close(socketDescriptor);
    exit(EXIT_FAILURE);
  }

  puts("\nEntrez quelques caractères au clavier.");
  puts("Le serveur les modifiera et les renverra.");
  puts("Pour sortir, entrez une ligne avec le caractère '.' uniquement.");
  puts("Si une ligne dépasse "xstr(MAX_MSG)" caractères,");
  puts("seuls les "xstr(MAX_MSG)" premiers caractères seront utilisés.\n");

  // Invite de commande pour l'utilisateur et lecture des caractères jusqu'à la
  // limite MAX_MSG. Puis suppression du saut de ligne en mémoire tampon.
  puts("Saisie du message : ");
  memset(msg, 0, sizeof msg);  // Mise à zéro du tampon
  scanf(" %"xstr(MAX_MSG)"[^\n]%*c", msg);

  // Arrêt lorsque l'utilisateur saisit une ligne ne contenant qu'un point
  while (strcmp(msg, ".")) {
    if ((msgLength = strlen(msg)) > 0) {
      // Envoi de la ligne au serveur
      if (sendto(socketDescriptor, msg, msgLength, 0,
                 (struct sockaddr *) &serverAddress,
                 sizeof(serverAddress)) == -1) {
        perror("sendto:");
        close(socketDescriptor);
        exit(EXIT_FAILURE);
      }

      // Lecture de la ligne modifiée par le serveur.
      memset(msg, 0, sizeof msg);  // Mise à zéro du tampon
      if (recv(socketDescriptor, msg, sizeof msg, 0) == -1) {
        perror("recv:");
        close(socketDescriptor);
        exit(EXIT_FAILURE);
      }
    }

    printf("Message traité : %s\n", msg);

    // Invite de commande pour l'utilisateur et lecture des caractères jusqu'à la
    // limite MAX_MSG. Puis suppression du saut de ligne en mémoire tampon.
    // Comme ci-dessus.
    puts("Saisie du message : ");
    memset(msg, 0, sizeof msg);  // Mise à zéro du tampon
    scanf(" %"xstr(MAX_MSG)"[^\n]%*c", msg);
  }

  close(socketDescriptor);

  return 0;
}