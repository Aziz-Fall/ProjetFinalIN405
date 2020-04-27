#include "list_pages.h"


Pages_list _delete_end_list(Pages_list l, pages_list **page_deleted)
{   
    if( !l->next )
    {   *page_deleted = l;
        return NULL;
    }
    l->next = _delete_end_list(l->next, page_deleted);
    return l;
}

void print_page(Page p)
{
    printf("[id: %3d, size: %4d]\n", p.id, p.size);
}

Pages_list create_list() { return NULL; }

Page init_page()
{
    Page p;
    p.size =  0;
    p.id   = -1;

    return p;
}

Page create_page(int size, int id)
{
    Page p;

    p.size = size;
    p.id   = id;

    return p;
}

Pages_list add(Pages_list l, Page p)
{
    Pages_list new_page = malloc(sizeof(pages_list));

    if( !new_page )
    {
        perror("In function add() -> malloc()");
        if( !l ) free(l);
        exit(EXIT_FAILURE);
    }

    new_page->p    = p;
    new_page->next = l;

    return new_page;
}

bool is_in_pages_list(Pages_list l, Page p)
{   
    if( !l ) return false;
    if( l->p.id == p.id && l->p.size == p.size) return true;

    return is_in_pages_list(l->next, p);
}
void print_content_pages_list(Pages_list l)
{
    if( !l ) return;
    print_page(l->p);
    print_content_pages_list(l->next);
}

Page get_least_recent_page(Pages_list *l)
{
    pages_list *element = NULL;
    if( !*l ) return init_page();
    *l     = _delete_end_list(*l, &element);
    Page p;
    p.id   = element->p.id;
    p.size = element->p.size;
    free(element);
    return p;
}

void pop_page(Pages_list *l, Page p)
{
    Pages_list tmp = *l, prev_element = NULL;

    while((*l) && (*l)->p.id != p.id)
    {
       prev_element = (*l);
       (*l)       = (*l)->next;
    }

    if( !(*l)->next ) //fin de list
    {
        printf(" 1 \n");

        if( prev_element )
            prev_element->next = NULL;
        else 
            tmp = NULL;

        free((*l));
        *l = tmp;
    }
    else if( !prev_element ) //début de list
    {
        (*l)   = (*l)->next;
        free(tmp);
    }
    else//milieu de list
    {
        prev_element->next = (*l)->next;
        free((*l));
        *l = tmp;
    }
}

void free_pages_list(Pages_list l)
{
    if( l )
        free_pages_list(l->next);
    
    free(l);
}