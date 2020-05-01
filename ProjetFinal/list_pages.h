#ifndef LIST_PAGES_H
#define LIST_PAGES_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * this struct represent a memory page.
 * @param size the size of the page
 * @param id   the id/index page
 */
typedef struct page
{
    int size;
    int id;
}Page;

/**
 * This struct represent a linked list
 * @param page_number a page number
 * @param next the next page in the linked list
 */
typedef struct pages_list
{
    Page p;
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
Pages_list add(Pages_list l, Page p);

/**
 * Check if the param page_number is in the list
 * @param l pages list
 * @param page_number page number
 * @return boolean
 */
bool is_in_pages_list(Pages_list l, Page p);

/**
 * Delete the page passed in argument
 * @param l pages list
 * @param page_number page number
 * @return the page deleted
 */
void pop_page(Pages_list *l, Page p);

/**
 * Delete the least recent page in the list.
 * @param l the list
 * @return the least recent page 
 */
Page get_least_recent_page(Pages_list *l);

/**
 * Print the content of the pages list.
 */
void print_content_pages_list(Pages_list l);

/**
 * Print the description of the page
 * @param p page
 */
void print_page(Page p);

/**
 * Create an empty memory page 
 * @param size the size of the memory page
 * @param id   the page's id
 * @return Page
 */
Page create_page(int size, int id);


Page init_page();

/**
 * Free the memory allocated.
 */
void free_pages_list(Pages_list l);

#endif