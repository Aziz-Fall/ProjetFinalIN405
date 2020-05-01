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
 * @param fd1             file descriptor
 * @param fd2             file descriptor
 * @param child           thread child
 * @param mutex_com       mutex of communication
 * @param cond_parent     condition parent
 * @param cond_son        condition son
 */
typedef struct communication
{
    int nb_child_thread;
    int fd1;
    int fd2;
    pthread_t *child;
    pthread_mutex_t mutex_com;
}Communication;

/**
 * This struct define an Address 
 * @param id_page   id page
 * @param id_thread id thread that send a demande
 */
typedef struct address
{
    int id_page;
    int id_pthread;
}Address;

/**
 * Create and Init an address
 * @param id_page    id page
 * @param id_pthread id thread
 */
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

/**
 * Create and init data child thread recover in the file config 
 * @param nb_access  the number of access
 * @param nb_pages   the number of page
 * @param id_pthread id thread
 */
Data_child init_data_child(int nb_ac, int nb_p, int id_pthread);

/**
 * This struct define the data recoved in the file config for parent thread
 * @param nb_access   the number of access of each child thread
 * @param nb_pages    the number of pages 
 * @param nb_pthreads the number of threads
 * @param size_page   the page size
 * @param nb_frames  the number of frames. 
 */
typedef struct data_parent_thread
{
    int nb_access;
    int nb_pages;
    int nb_pthread;
    int size_page;
    int nb_frames;
}Data_parent;

/**
 * Create and init data parent 
 * @param nb_ac      number of access for each child thread
 * @param nb_p       number of pages
 * @param nb_pthread number of thread
 * @param size       size page
 * @param nb_f       number of frame
 */
Data_parent init_data_parent(int nb_ac, int nb_p, int nb_pthread, int size, int nb_f);

/**
 * Set a request to the parent thread
 * @param addresse physical address 
 * @param fd       file descriptor    
 */
void set_request(Address *address, int fd);

/**
 * Recover the request 
 * @param fd the pipe communication
 * @return   the phisical address
 */
Address *get_request(int fd);

/**
 * Set a response to the child thread
 * @param logic_address logic address
 * @param fd            file descriptor
 */
void set_response(int logic_address, int fd);

/**
 * Recover the response
 * @param fd the pipe communication
 * @return   the logic address
 */
int get_response(int fd);

/**
 * Create all pipes for the comminucation.
 */
void create_pipes();

/**
 * Open pipe's commnunication.
 * @param com Communication
 */
void open_pipes(Communication *com);
/**
 * Delete pipe communication
 */
void delete_pipe();

/**
 * Release the memory allocated
 * @param com Communication
 */
void release_memory(Communication com);

#endif