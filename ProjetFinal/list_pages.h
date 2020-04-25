#ifndef LIST_PAGES_H
#define LIST_PAGES_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct pages_list
{
    int page_number;
    struct pages_list *next;
}pages_list, *Pages_list;

/**
 * Create empty list
 */
Pages_list create_list();

/**
 * Add on the begin of the liste element passed in argument.
 * @param page_number the element
 * @return l
 */
Pages_list add(Pages_list l, int page_number);

/**
 * Check if the param page_number is in the list
 * @param l pages list
 * @param page_number page number
 * @return boolean
 */
bool is_in_pages_list(Pages_list l, int page_number);

/**
 * Delete the page passed in argument
 * @param l pages list
 * @param page_number page number
 * @return the page deleted
 */
int pop_page(Pages_list *l, int page_number);

/**
 * Delete the least recent page in the list.
 * @param l the list
 * @return the least recent page 
 */
int get_least_recent_page(Pages_list *l);

/**
 * Print the content of the pages list.
 */
void print_content_pages_list(Pages_list l);

#endif