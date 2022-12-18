#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>



int main(){

   // reading the file 

    FILE *ptr;
    char file[2455920];
    ptr = fopen("test1.txt","r");
    fread(&file, 1, 2455920, ptr);
    fclose(ptr);

    // spliting the file to two files
    char f1[1227960]; 
    char f2[1227960];
    for(int i = 0; i < 1227960; i++){
        f1[i] = file[i];
        f2[i] = file[1227960 + i];
    }

    //-------------------------------//

   
    // creat a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Eror in creating socket");
        exit(1);
    }

   
    // initialize an address for the socket
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8002);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
    
    
    // create a connection 
    int connection_status =  connect(sockfd, (struct sockaddr *) &server_address, sizeof(server_address));
    if(connection_status == -1){
        printf("Eror in connecting\n");
        exit(1);
    }
    printf("connection succsseed\n");


    int user_decision;
    
    // Send the file

    while(1){

        // send the first half of the file
        printf("sending the first file...\n");
        int send_status = send(sockfd, f1, sizeof(f1), 0); 
        if(send_status == -1){
            printf("Eror in sending the file");
            exit(1);
        }
    
        
        // check the authentication from the receiver
        char authentication[17] = "0000010001010110";
        char receiver_response[17];
        int result = -1;
        
        while(result != 0){ 
            recv(sockfd, &receiver_response, sizeof(receiver_response), 0); 
            result = strcmp(authentication, receiver_response);
        }
        
        printf("Sending of the first file has Succsseed\n");

        

        // change the cc algorithm 
        char reno[6] = "reno";
        char algorithm[256];
        socklen_t len;
        len = sizeof(algorithm);


        int get_status = getsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, algorithm, &len);
        if(get_status != 0){
            printf("Eror in getting the current cc algorithm\n");
            exit(1);
        }

        int ans = strcmp(algorithm, reno);

        if(ans == 0){
            printf("Current algorithm: %s\n", algorithm);
            strcpy(algorithm, "cubic");
            int set_status = setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, algorithm, strlen(algorithm));
            if(set_status != 0){
                printf("Eror in changing the current cc algorithm\n");
                exit(1);
            }
            printf("The new algorithm is: cubic\n");

        }else{
            printf("Current algorithm: %s\n", algorithm);
            strcpy(algorithm, "reno");
            int set_status = setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, algorithm, strlen(algorithm));
            if(set_status != 0){
                printf("Eror in changing the current cc algorithm\n");
                exit(1);
            }
            printf("The new algorithm is: reno\n");
        }

        //send the second half of the file
        printf("sending the second file...\n");
        send_status = send(sockfd, f2, sizeof(f2), 0);
        if(send_status == -1){
            printf("Eror in sending the file");
            exit(1);
        }
        printf("Sending of the second file has Succsseed\n");

        //asking the user 
        printf("Send the file again?\n");
        printf("1 = Yes  ,  0 = No\n");
        scanf("%d", &user_decision);
        printf("the user choosed: %d\n", user_decision);

        //send an update message to the receiver
        
        if(user_decision == 1){
            char msg3[7] = "again.";
            send_status = send(sockfd, msg3, sizeof(msg3), 0);
            if(send_status == -1){
                printf("Eror in sending the file");
                exit(1);
            }
            printf("sending again...\n");

            //change the algorithm again
            get_status = getsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, algorithm, &len);
            if(get_status != 0){
                printf("Eror in getting the current cc algorithm\n");
                exit(1);
            }
            
            ans = strcmp(algorithm, reno);

            if(ans == 0){
                printf("Current algorithm: %s\n", algorithm);
                strcpy(algorithm, "cubic");
                int set_status = setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, algorithm, strlen(algorithm));
                if(set_status != 0){
                    printf("Eror in changing the current cc algorithm\n");
                    exit(1);
                }
                printf("The new algorithm is: cubic\n");

            }else{
                printf("Current algorithm: %s\n", algorithm);
                strcpy(algorithm, "reno");
                int set_status = setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, algorithm, strlen(algorithm));
                if(set_status != 0){
                    printf("Eror in changing the current cc algorithm\n");
                    exit(1);
                }
                printf("The new algorithm is: reno\n");
            }

        }else if (user_decision == 0){
            char msg4[7] = "byebye";
            send(sockfd, msg4, sizeof(msg4), 0);
            printf("bye bye!\n");
            break;
        }
    
    }

    //close the socket
    close(sockfd);

    return 0;
    
}
















