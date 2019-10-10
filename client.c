#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main(){

        FILE *file;
        char file_name[1000];
        char c, text[100];
        printf("Please, enter the file name, which you want to get a copy:\n");
        scanf("%s", file_name);
        file = fopen(file_name, "r");
        if (file == NULL){
                printf("Error during opening file\n");
                return 1;
        }
        c = fgetc(file);

        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd == -1){
                printf("Error during creating socket.\n");
                return 1;
        }

        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(PORT);
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);

        if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1){
                printf("Error during connecting to the server.\n");
                return 1;
        }


        while (c != EOF){
                send(socket_fd, (const void *)&c, 1, 0);
                c = fgetc(file);
        }
        fclose(file);
        close(socket_fd);
        return 0;
}
