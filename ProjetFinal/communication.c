#include "communication.h"


void set_request(Address *address, int fd)
{
    if( write(fd, address, sizeof(Address)) < 0 )
    {
        perror("In set_request() -> write()");
        return;
    }
}

Address *get_request(int fd)
{
    static Address ad;
    
    if( read(fd, &ad, sizeof(struct address)) < 0 )
    {
        perror("In set_request() -> write()");
        return NULL;
    }
    
    return &ad;
}

void set_response(int logic_address, int fd)
{
    if( write(fd, &logic_address, sizeof(int)) < 0 )
    {
        perror("In set_response() -> write()");
        return;
    }
}

int get_response(int fd)
{
    int response = 0;
    if( read(fd, &response, sizeof(int)) < 0 )
    {
        perror("In set_reponse() -> write()");
        return -1;
    }

    return response;
}

Address init_address(int id_page, int id_pthread)
{
    Address a;
    a.id_page    = id_page;
    a.id_pthread = id_pthread;

    return a;
}

Data_child init_data_child(int nb_ac, int nb_p, int id_pthread)
{
    Data_child d;
    d.nb_access  = nb_ac;
    d.nb_pages   = nb_p;
    d.id_pthread = id_pthread;

    return d;
}

Data_parent init_data_parent(int nb_ac, int nb_p, int nb_pthread, int size)
{
    Data_parent d;
    d.nb_access  = nb_ac;
    d.nb_pages   = nb_p;
    d.nb_pthread = nb_pthread;
    d.size_page  = size;

    return d;
}

void create_pipes()
{
    if( mkfifo("tube_in", 0666) < 0 || mkfifo("tube_out", 0666) < 0 )
    {
        perror("mkfifo()");
        exit(EXIT_FAILURE);
    }
}

void open_pipes(Communication *com)
{
    com->fd1 = open("tube_in", O_RDWR);
    com->fd2 = open("tube_out", O_RDWR);
}
void delete_pipe()
{
    unlink("tube_in");
    unlink("tube_out");
}

void release_memory(Communication com)
{   
    if( com.child )
        free(com.child);
}