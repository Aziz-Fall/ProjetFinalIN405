#include "list_pages.h"

int main(void)
{
    
    Pages_list l = create_list();
    l            = add(l, 3); 
    l            = add(l, 6); 
    l            = add(l, 9); 
    l            = add(l, 2);
    print_content_pages_list(l);
    printf("\n"); 
    return EXIT_SUCCESS;
}