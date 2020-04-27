#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "list_pages.h"
#define FREE_PAGE -1
/**
 * This struct represent quick memory
 * @param size    number elements
 * @param t_frame pages array
 * @param l       linked list that manage the least recent page
 */
typedef struct frame
{
    int size; 
    int number_pages_occupated;
    Page *t_frame; 
    Pages_list l; 
}Frame;

/**
 * Create a frame 
 * @param size the size or number element in the frame.
 */
Frame create_frame(int size);

/**
 * Load the page passed in argument
 * @param f a frame
 * @param p a page 
 * @return  frame
 */
Frame load_page(Frame f, Page p);

/**
 * Check if the page passed in argment is in the frame
 *  and return boolean
 * @param f Frame
 * @param p page
 * @return  boolean
 */
bool is_in_frame(Frame f, Page p);

/**
 * Update the position of the argument page in the pages list
 * @param f frame
 * @param p page
 * @return  Frame
 */
Frame update_pages_list(Frame f, Page p);

/**
 * Free memory allocated in the Frame
 * @param f Frame
 */
void free_frame(Frame f);

/**
 * Print pages memory 
 */
void print_content_frame(Frame f);

#endif