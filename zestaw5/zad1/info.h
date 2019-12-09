#ifndef INFO_H
#define INFO_H

#define MAX_CLIENTS  10
#define PROJECT_ID 0x033
#define MAX_CONT_SIZE 4096

typedef enum type {
    TIME = 1, END = 2, LOGIN = 3, STOP = 4
} mtype;

typedef struct Message {
    long msg_type;
    pid_t idSender;
    char msg_text[MAX_CONT_SIZE];
} Message;

const size_t MSG_SIZE = sizeof(Message);

#endif