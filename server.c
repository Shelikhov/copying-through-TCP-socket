#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define LISTEN_BACKLOG 1
#define TRUE 1

int function_with_new_socket(int new_socket, char * file_name);

int main(){

        char file_name[100];
        printf("Please enter file name where you want to copy:\n");
        scanf("%s", file_name);

        int socket_fd, new_socket_fd;
        struct sockaddr_in server_address, client_address;
        socklen_t client_address_size;

        socket_fd = socket(AF_INET, SOCK_STREAM, 0);

        if (socket_fd == -1){
                printf("Error during creating of socket.\n");
                return 1;
        }
        else{
                printf("Socket created.\n");
        }

        memset(&server_address, 0, sizeof(struct sockaddr_in));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(PORT);
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);


        if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in)) < 0){
                printf("Error during binding procedure.\n");
                return 1;
        }
        else{
                printf("Binding procedure is success.\n");
        }

        if (listen(socket_fd, LISTEN_BACKLOG) == -1){
                printf("Error during listening procedure.\n");
                return 1;
        }
        else{
                printf("Listening procedure is success.\n");
        }

        client_address_size = sizeof(client_address);
        new_socket_fd = accept(socket_fd, (struct sockaddr*)&client_address, &client_address_size);
        if (new_socket_fd == -1){
                printf("Error during accpting procedure.\n");
                return 1;
        }
        else{
                printf("Accepting is success.\n");
        }

        function_with_new_socket(new_socket_fd, file_name);

        close(socket_fd);

        return 0;
}

int function_with_new_socket(int new_socket, char * file_name){

        FILE *file;
        file = fopen(file_name, "w");
        if (file == NULL){
                printf("Error during open file.\n");
                return 1;
        }

        char c;
        while(TRUE){
                recv(new_socket, (void *)&c, 1, 0);
                fputc(c, file);
        }
        fclose(file);
        close(new_socket);
        return 0;
}
