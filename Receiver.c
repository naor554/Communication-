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
#include <time.h>

int main(){

    // creat a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Eror in creating socket\n");
        exit(1);
    }

    // initialize an address for the socket
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8002);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    // create connection 
    bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address));

    // Prepare to accept connections on socket FD
    int listen_status = listen(sockfd, 1);
    if(listen_status == -1){
        printf("Eror in getting connection");
        exit(1);
    }
    printf("listening on 127.0.0.0 : 8002 ...\n");


    // tools for receving and for authentication//
    char authentication[17] = "0000010001010110";
    char msg3[7] = "byebye";
    

    // tools for measuring time //
    struct timespec begin, end; 
    long seconds; 
    long nanoseconds;
    double elapsed;

    // tools for saving the times //
    double times[10];
    int i = 0;
    double sum1;
    double sum2;
    double average1;
    double average2;

    // tools for changing algorithm //
    char reno[6] = "reno";
    char algorithm[256];
    socklen_t len;
    len = sizeof(algorithm);
    int ans;
    

    // wait for an incoming connection
    int sender_socket;
    sender_socket = accept(sockfd, NULL, NULL);
    if(sender_socket == -1){
        printf("Eror in opennig new socket");
        exit(1);
    }




    while (1){

        
    // Buffers for saving the data that will be sent
        char buf1[1227960];
        char buf2[1227960];
        char buf3[7];

       // veriables for calculate the number of bytes that receivd
        int count1 = 0;
        int total1 = 0;
        
        
        // Start measuring time
        clock_gettime(CLOCK_REALTIME, &begin);

        while(total1 != sizeof(buf1)){
            count1 = recv(sender_socket, &buf1[total1], sizeof(buf1) - total1, 0);
            printf("Bytes received: %d\n", count1);
            total1 += count1;
            printf("Total Bytes received: %d\n", total1);
        // At this point the buffer is valid from 0..total-1, if that's enough then process it and break, otherwise continue
        }

        // Stop measuring time
        clock_gettime(CLOCK_REALTIME, &end);
        seconds = end.tv_sec - begin.tv_sec;
        nanoseconds = end.tv_nsec - begin.tv_nsec;
        elapsed = seconds + nanoseconds*1e-9;
        printf("Time measured for the first file: %.6f seconds.\n", elapsed);

        //save the time 
        times[i] = elapsed;
        i++;


        //send an authentication
        int send_status = send(sender_socket, authentication, sizeof(authentication), 0);
        if(send_status == -1){
            printf("Eror in sending the authentication");
            exit(1);
        }

        
        
        // change the cc algorithm 
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

        // veriables for calculate the number of bytes that receivd
        int count2 = 0;
        int total2 = 0;
        
        
        // Start measuring time
        clock_gettime(CLOCK_REALTIME, &begin);

        while(total2 != sizeof(buf2)){
            count2 = recv(sender_socket, &buf2[total2], sizeof(buf2) - total2, 0);
            printf("Bytes received: %d\n", count2);
            total2 += count2;
            printf("Total Bytes received: %d\n", total2);
        // At this point the buffer is valid from 0..total-1, if that's enough then process it and break, otherwise continue
        }

        // Stop measuring time
        clock_gettime(CLOCK_REALTIME, &end);
        seconds = end.tv_sec - begin.tv_sec;
        nanoseconds = end.tv_nsec - begin.tv_nsec;
        elapsed = seconds + nanoseconds*1e-9;
        printf("Time measured for the second file: %.6f seconds.\n", elapsed);

        //save the time 
        times[i] = elapsed;
        i++;

        printf("waiting for an update...\n");
        //receive an update message
        recv(sender_socket, &buf3, sizeof(buf3), 0);
        printf("I got an update\n");
        printf("%s\n", buf3);

        int result = strcmp(buf3, msg3);
        

        if(result == 0){
            for (int i = 0; i < 10; i++){
                printf("[%d]  %.6f seconds.\n", i, times[i]); 
            }
            for (int i = 0; i < 10; i += 2){
                sum1 += times[i];
                sum2 += times[i+1]; 
            }
            average1 = sum1 / 5;
            average2 = sum2 / 5;
            printf("The average time for sending the first file is: %.6f seconds.\n", average1);
            printf("The average time for sending the second file is: %.6f seconds.\n", average2);
            break;   
        }else{
            // change the cc algorithm 
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

        }
    
    }

    //close the socket
    close(sockfd);

    return 0;    

}