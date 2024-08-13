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

#define MAX_CONN 2

struct Entry {
    char prompt[1024];
    char options[3][50];
    int answer_idx;
};

struct Player {
    int fd;
    int score;
    char name[128];
};

int read_questions(struct Entry* Q, char* filename){
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    int Qno = 0;

     while (fgets(Q[Qno].prompt, sizeof(Q[Qno].prompt), file)) {

        char reading[1024];
        char ans[1024];
        char buffer[1024];

        fgets(reading, sizeof(reading), file);
        sscanf(reading, "%s %s %s", Q[Qno].options[0], Q[Qno].options[1], Q[Qno].options[2]);
        fgets(ans, sizeof(ans), file);
        for(int i = 0; i<3; i++){
             if(strcmp(ans, Q[Qno].options[i]) == 0){
                 Q[Qno].answer_idx = i;
                 break;
             }
        }

        fgets(buffer, sizeof(buffer), file);

     }
    fclose(file);
     return Qno+1;

}

int prep_fds( fd_set* active_fds,
    int server_fd,
    int* client_fds ) {

    FD_ZERO(active_fds);
    FD_SET(server_fd, active_fds);

    int max_fd = server_fd;

    for (int i = 0; i < MAX_CONN; i ++) {
    if (client_fds[i] > -1)
    FD_SET(client_fds[i], active_fds);
    if (client_fds[i] > max_fd)
    max_fd = client_fds[i];
    }
    return max_fd;
}

int main(int argc, char *argv[]){
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
    // //Setting up socket, binding,and listening
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htonl(p_arg);
    server_addr.sin_addr.s_addr = inet_addr(i_arg);
    bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
        perror("bind");
        //exit(EXIT_FAILURE);
    

    struct sockaddr_in in_addr;
    socklen_t addr_size = sizeof(in_addr);
    if(listen(server_fd, 2) == 0){
        printf("Welcome to 392 Trivia!\n");
    }
    else{
        perror("listen");
    }

    struct Entry Q[50];
    int numQ = read_questions(Q, f_arg);
    struct Player players[2];

    printf("%s\n", i_arg);
    printf("%d\n", p_arg);

    fd_set active_fds;
    int client_fds[MAX_CONN];
    int nconn = 0;
    int in_fd;
    char buffer[128];
    for (size_t i = 0; i < MAX_CONN; i ++) client_fds[i] = -1;

    printf("%d\n", server_fd);

    while(1){
        printf("here1\n");
        int max_fd = prep_fds(&active_fds, server_fd, client_fds);
        printf("%d\n", max_fd);
        int what = select(max_fd + 1, &active_fds, NULL, NULL, NULL);
                printf("%d\n", what);
        printf("here2\n");
        if (FD_ISSET(server_fd, &active_fds)) {
            in_fd = accept(server_fd,(struct sockaddr*) &in_addr,&addr_size);
            printf("here3\n");
            if (nconn == MAX_CONN) {
                close(in_fd);
                fprintf(stderr, "Max connection reached!\n");
            }
            else {
                printf("%d\n", in_fd);
                char *receipt = "Please type your name:\n";
                        int written = write(in_fd, receipt, strlen(receipt));
                        if(written < 0){ printf("faield\n");}
                        memset(buffer, 0, sizeof(buffer));
                        read(in_fd, buffer, sizeof(buffer) - 1);
                        printf("Hi %s!", buffer);
                // for (int i = 0; i < MAX_CONN; i ++) {
                //     if (client_fds[i] == -1) {
                //         client_fds[i] = in_fd;
                //         char *receipt = "Please type your name:\n";
                //         write(client_fds[i], receipt, strlen(receipt));
                //         memset(buffer, 0, sizeof(buffer));
                //         read(client_fds[i], buffer, sizeof(buffer) - 1);
                //         printf("Hi %s!", buffer);
                //         players[i].fd = client_fds[i];
                //         players[i].score = 0;
                //         strcpy(players[i].name, buffer);//memcopy or move
                //         printf("New connection detected!\n");
                //         nconn ++;
                //         if(i == 1){
                //             printf("The game starts now!");
                //         }
                //         break;
                //     }
                // }
            }
        }
        for(int i = 0; i<numQ; i++){
            int number = i+1;
            char question[1024];
            char buffer2[1024]; 
            snprintf(question, sizeof(question), "Question %d: %s\n"
            "1: %s\n"
            "2: %s\n"
            "3: %s\n", number, Q[i].prompt, Q[i].options[0], Q[i].options[1], Q[i].options[2]);
            for(int i = 0; i<MAX_CONN; i++){
                write(client_fds[i], question, strlen(question));
            }
            while(1){
                 select(max_fd + 1, &active_fds, NULL, NULL, NULL);
                 if(FD_ISSET(client_fds[0], &active_fds)){
                    read(client_fds[0], buffer2, sizeof(buffer2)-1);
                    if(buffer2 == Q[i].answer_idx){
                        players[0].score += 1;
                        continue;
                    }
                    else{
                        players[0].score -=1;
                    }
                 }
                 if(FD_ISSET(client_fds[1], &active_fds)){
                    read(client_fds[0], buffer2, sizeof(buffer2)-1);
                    if(buffer2 == Q[i].answer_idx){
                        players[1].score += 1;
                        continue;
                    }
                    else{
                        players[1].score -=1;
                    }
                 }
            }

        }
        char winner[1024];
        if(players[0].score > players[1].score){
            snprintf(winner, sizeof(winner), "Congrats, %s!", players[0].name);
        }else{
            snprintf(winner, sizeof(winner), "Congrats, %s!", players[1].name);
        }
        for(int i = 0; i<MAX_CONN; i++){
            write(client_fds[i], winner, strlen(winner));
            close(client_fds);
        }

    break;
    }


    // int connected = 0;
    // while(connected < MAX_CONNECTIONS){
    //     int client_fd = accept(server_fd, (struct sockaddr*)&in_addr, &addr_size);
    //     if(client_fd <0){
    //         perror("accepting");
    //         exit(EXIT_FAILURE); //do i exit or continue accpeting connections
    //     }
    //     else{
    //         printf("New connection detected!");
    //         char *receipt = "Please type your name:\n";
    //         send(client_fd, receipt, strlen(receipt), 0);
    //         //how does a player quit  their program and how can I know once a player has quit their program
    //     }
    //     //do you use stdin to read the cleints input such as name and number selected


    //}
    close(server_fd);
    return 0;
    
}