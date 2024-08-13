//Name: Maria Ebrahim
//Pledge: I pledge my honor that I have abided by the Stevens Honor System.
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void parse_connect(int argc, char** argv, int* server_fd) {
    int flag_f = 0;
    int flag_i = 0;
    int flag_p = 0;
    int flag_h = 0;

    char *f_arg;
    char *i_arg;
    int p_arg;

    int c;

    while ((c = getopt (argc, argv, ":f:i:p:h")) != -1){
        switch (c){
        case ':':
        switch(optopt){
            case 'f':
                printf("Error: missing argument for flag %c\n", optopt);
                break;
            case 'i':
                printf("Error: missing argument for flag %c\n", optopt);
                break;
            case 'p':
                printf("Error: missing argument for flag %c\n", optopt);
                break;
            default:
                fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt, strerror(errno));
                exit(EXIT_FAILURE);}
        case 'f':
                flag_f = 1;
                f_arg = optarg;
                break;
        case 'i':
            flag_i = 1;
            i_arg = optarg;
            break;
        case 'p':
            flag_p = 1;
            p_arg = atoi(optarg);
            break;
        case 'h':
            flag_h = 1;
            break;
        default:
            fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }


    if(flag_h == 1){
        printf("Usage: %s [-f question_file] [-i IP_address] [-p port_number] [-h]\n"
       "    -f question_file    Default to \"question.txt\";\n"
       "    -i IP_address       Default to \"127.0.0.1\";\n"
       "    -p port_number      Default to 25555;\n"
       "    -h                  Display this help info.\n", argv[0]);

       exit(EXIT_SUCCESS);
    }

    if(flag_f == 0){
        f_arg = "question.txt";
    }

    if(flag_p == 0){
        p_arg = 25555; 
    }

    if(flag_i == 0){
        i_arg = "127.0.0.1";
    }
    // struct sockaddr_in server_addr;
    // socklen_t addr_size = sizeof(server_addr);

    // *server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // if(*server_fd<0){printf("socket\n");}
    
    // memset(&server_addr, 0, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // printf("%d\n", p_arg);
    // server_addr.sin_port = htonl(p_arg);
    // printf("%s\n", i_arg);
    // server_addr.sin_addr.s_addr = inet_addr(i_arg);

    // int connected = connect(*server_fd, (struct sockaddr*)&server_addr, addr_size);
    // if(connected<0){ perror("");}

}


int main(int argc, char** argv){
    int server_fd;
    //parse_connect(argc, argv, &server_fd);
    //printf("%d\n", server_fd);
    struct sockaddr_in server_addr;
    socklen_t addr_size = sizeof(server_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd<0){printf("socket\n");}
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    //printf("%d\n", p_arg);
    server_addr.sin_port = htonl(25555);
    //printf("%s\n", i_arg);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connected = connect(server_fd, (struct sockaddr*)&server_addr, addr_size);
    if(connected<0){ perror("");}

    //while(1){
        char buffer[1024];
        char buffer2[1024];
        read(server_fd, buffer, sizeof(buffer));
        printf("%s\n", buffer);
        fgets(buffer2, sizeof(buffer2), stdin);
        write(server_fd, buffer, strlen(buffer));
    //}
    return 0;
}

