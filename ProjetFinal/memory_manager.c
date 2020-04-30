#include "memory_manager.h"

void _do_LRU_algorithm(Frame *f)
{
    Page p = get_least_recent_page(&(f->l));
    printf("*****Page least recent: %d\n", p.id);

    for( int i = 0; i < f->size; i++ )
        if( f->t_frame[i].id == p.id )
           f->t_frame[i] = init_page();
    --f->number_pages_occupated;
}

int _get_free_page(Frame f)
{
    for( int i = 0; i < f.size; i++ )
        if( f.t_frame[i].id ==  FREE_PAGE )
            return i;
    return -1;
}

Frame create_frame(int size)
{
    Frame f;

    f.size                   = size;
    f.t_frame                = malloc(sizeof(Page) * f.size);
    f.number_pages_occupated = 0;

    if( !f.t_frame)
    {
        perror("In function create_frame() -> malloc()");
        exit(EXIT_FAILURE);
    }

    for( int i = 0; i < f.size; i++ )
        f.t_frame[i] = init_page();

    f.l = create_list();

    return f;
}

bool is_in_frame(Frame f, Page p)
{
    for( int i = 0; i < f.size; i++ )
        if( f.t_frame[i].size == p.size && f.t_frame[i].id == p.id )
            return true;
    return false;
}

Frame load_page(Frame f, Page p)
{
    int free_index = -1;
    //If memory is not full : recover the index of free page in the frame
    if( f.number_pages_occupated < f.size )
        free_index = _get_free_page(f);
    else // if frame is full : do LRU algorithm
    {   
        printf(".....Do algorithme....\n");
        _do_LRU_algorithm(&f);
        printf(".....Fin algorithme....\n");
        free_index = _get_free_page(f);
    }

    printf("**index : %d\n", free_index);
    // add page in the frame.
    f.number_pages_occupated += 1;
    f.t_frame[free_index] = p;
    f.l                   = add(f.l, p);
    printf("------>Nuber of page: %d\n", f.number_pages_occupated);
    return f;
}

void free_frame(Frame f)
{
    free(f.t_frame);
    free_pages_list(f.l);
}

Frame update_pages_list(Frame f, Page p)
{
    pop_page(&(f.l), p);
    f.l = add(f.l, p);
    return f;
}

void print_content_frame(Frame f)
{
    for( int i = 0; i < f.size; i++ )
        print_page(f.t_frame[i]);

}