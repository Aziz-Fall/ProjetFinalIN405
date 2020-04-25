#include "list_pages.h"

bool is_null(void *arg)
{
    return ( !arg ) ? true : false; 
}

Pages_list create_list() { return NULL; }

Pages_list add(Pages_list l, int page_number)
{
    Pages_list new_page = malloc(sizeof(pages_list));

    if( !new_page )
    {
        perror("In function add() -> malloc()");
        if( !l ) free(l);
        exit(EXIT_FAILURE);
    }

    new_page->page_number = page_number;
    new_page->next        = l;

    return new_page;
}

void print_content_pages_list(Pages_list l)
{
    if( !l ) return;
    printf("[%3d]", l->page_number);
    print_content_pages_list(l->next);
}