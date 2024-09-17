
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>


void test(){
    // Temporarily simulate argv for testing purposes
    // char* tmp[] = {"programName", "parent", "child1", "child2", "3"};
}

char** getMessages(char* argv[]) {
    char **messages = malloc(3 * sizeof(char*));
    if (messages == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }
    // Copy each argument into the messages array and append a newline
    for (int i = 0; i < 3; i++) {
        int len = strlen(argv[i + 1]);
        messages[i] = malloc(len + 2); // +2 for newline and null terminator
        if (messages[i] == NULL) {
            perror("Failed to allocate memory for message");
            // Cleanup previously allocated memory before exiting
            for (int j = 0; j < i; j++) {
                free(messages[j]);
            }
            free(messages);
            return NULL;
        }
        sprintf(messages[i], "%s\n", argv[i + 1]); // Copy and append a newline
    }

    return messages;

}



void freeMessages(char** messages){
    // Free allocated memory
    for (int i = 0; i < 3; i++) {
        free(messages[i]);
    }
    free(messages);
}
int getTimesToWrite(int argc,char* argv[]){
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <parent_message> <child1_message> <child2_message> <count>\n",
                argv[0]);

        return -1;
    }
    else return atoi(argv[4]);

}
int synchronizeWriting(char** messages,int timesToWrite){
    int status1 =0;
    int status2 =0;
    int fdOut =0;

    pid_t pid1,pid2;
    pid1 = fork();
    if (pid1 < 0){
        perror("fork has failed!\n");
        exit(1);
    }
    if(pid1 == 0){// in child process
        printf("child 1 before sleep \n");
        sleep(2);
        printf("child 1 after sleep \n");
        fdOut = open("output.txt", O_CREAT | O_RDWR| O_APPEND, 0666);
        if (fdOut == -1) {
            perror("Error opening file");
            return 1;
        }
        for(int i = 0; i < timesToWrite; i ++){
            ssize_t bytes_written = write(fdOut, messages[1], strlen(messages[1]));
            if (bytes_written == -1) {
                perror("Error writing to file");
                close(fdOut);
                exit(1); // Exit if writing failed
            }
        }
        printf("child 1 after loop\n");
        return 1;
    }else{
        printf("parent before sec fork \n");
        pid2 = fork();
        if (pid2 < 0){
            perror("fork has failed!\n");
            exit(1);
        }else if(pid2 == 0){
            printf("child 2 before sleep \n");
            sleep(4);
            printf("child 2 after sleep \n");
            fdOut = open("output.txt", O_CREAT | O_RDWR| O_APPEND, 0666);
            if (fdOut == -1) {
                perror("Error opening file");
                return 1;
            }
            for(int i = 0; i < timesToWrite; i ++){
                ssize_t bytes_written = write(fdOut, messages[2], strlen(messages[2]));
                if (bytes_written == -1) {
                    perror("Error writing to file");

                    exit(1); // Exit if writing failed
                }
            }
            close(fdOut);
            printf("child 2 after write \n");
            return 1;
        }
        waitpid(pid1,&status1,0);
        printf("after wait status 1 is : %d \n",status1);
        waitpid(pid2,&status2,0);
        printf("after wait status 2 is : %d \n",status2);

        fdOut = open("output.txt", O_CREAT | O_RDWR| O_APPEND, 0666);
        if (fdOut == -1) {
            perror("Error opening file");
            return 1;
        }
        for(int i = 0; i < timesToWrite; i ++){
            ssize_t bytes_written = write(fdOut, messages[0], strlen(messages[0]));
            if (bytes_written == -1) {
                perror("Error writing to file");
                close(fdOut);
                exit(1); // Exit if writing failed
            }
        }

    }
    return 0;
}

int main(int argc, char* argv[]) {
    char* tmp[] = {"programName", "parent", "child1", "child2", "3"};
    char** messages = getMessages(argv);
    int timesToWrite = 0;
    if (messages == NULL) {
        fprintf(stderr, "Failed to get messages\n");
        return 1;
    }
    timesToWrite = getTimesToWrite(argc,argv);
    if(timesToWrite < 0){
        return 1;
    }
    synchronizeWriting(messages,timesToWrite);
    freeMessages(messages);


    return 0;
}
