#include "communication.h"

void set_request(Address *address, int pipe[])
{
    close(pipe[0]);
    if( write(pipe[1], address, sizeof(struct address)) < 0 )
    {
        perror("In set_request() -> write()");
        return;
    }
    close(pipe[1]);
}

Address *get_request(int pipe[])
{
    static Address ad;
    close(pipe[1]);
    if( read(pipe[0], &ad, sizeof(struct address)) < 0 )
    {
        perror("In set_request() -> write()");
        return NULL;
    }
    close(pipe[0]);

    return &ad;
}

void set_response(int logic_address, int pipe[])
{
    close(pipe[0]);
    if( write(pipe[1], &logic_address, sizeof(int)) < 0 )
    {
        perror("In set_response() -> write()");
        return;
    }
    close(pipe[1]);
}

int get_response(int pipe[])
{
    int response = 0;
    close(pipe[1]);
    if( read(pipe[0], &response, sizeof(int)) < 0 )
    {
        perror("In set_reponse() -> write()");
        return -1;
    }
    close(pipe[0]);

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
