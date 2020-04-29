#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "memory_manager.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * This struct define the communication between
 * parent thread and thread son.
 * @param nb_child_thread nb child thread
 * @param pipe_parent the named pipe for communication
 * @param pipe_child the named pipe for communication
 * @param mutex_com       mutex of communication
 * @param cond_parent     condition parent
 * @param cond_son        condition son
 */
typedef struct communication
{
    int nb_child_thread;
    int fd1;
    int fd2;
    pthread_t parent;
    pthread_t *child;
    pthread_mutex_t mutex_com;
    pthread_cond_t cond_parent;
    pthread_cond_t cond_child;

}Communication;

typedef struct address
{
    int id_page;
    int id_pthread;
}Address;


Address init_address(int id_page, int id_pthread);

/**
 * This struct define the data recoved in the file config for child thread
 * @param nb_access  the number of access of each child thread
 * @param nb_pages   the number of pages 
 * @param id_pthread id thread
 */
typedef struct data_child_thread
{
    int nb_access;
    int nb_pages;
    int id_pthread;
}Data_child;

Data_child init_data_child(int nb_ac, int nb_p, int id_pthread);
/**
 * This struct define the data recoved in the file config for parent thread
 * @param nb_access   the number of access of each child thread
 * @param nb_pages    the number of pages 
 * @param nb_pthreads the number of threads
 * @param size_page   the page size
 */
typedef struct data_parent_thread
{
    int nb_access;
    int nb_pages;
    int nb_pthread;
    int size_page;
}Data_parent;

Data_parent init_data_parent(int nb_ac, int nb_p, int nb_pthread, int size);

/**
 * Set a request to the parent thread
 * @param addresse      physical address 
 * @param pipes[]     the pipe communication    
 */
void set_request(Address *address, int pipes[]);

/**
 * Recover the request 
 * @param pipes[] the pipe communication
 * @return          the phisical address
 */
Address *get_request(int pipes[]);

/**
 * Set a response to the child thread
 * @param logic_address logic address
 * @param pipes[]     the pipe communication
 */
void set_response(int logic_address, int pipes[]);

/**
 * Recover the response
 * @param pipes[] the pipe communication
 * @return          the logic address
 */
int get_response(int pipes[]);

#endif