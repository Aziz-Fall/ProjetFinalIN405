#include "communication.h"
#include "time.h"

#define NB_DATA_FILE 5

/**
 * Manage the child thread communication
 * @param arg the data 
 */
void *child_thread(void *arg);

/**
 * Manage the parent thread communication
 * @param arg the data
 */
void *parent_thread(void *arg);

/**
 * Recover the data in the file config.
 * @param argc the number of argument of main function
 * @param argv list of arguments
 */
int *recover_data(int argc, char **argv);

static Communication com = {
                            .mutex_com   = PTHREAD_MUTEX_INITIALIZER,
                            .cond_parent = PTHREAD_COND_INITIALIZER,
                            .cond_child  = PTHREAD_COND_INITIALIZER, 
                           };

                           
int main(int argc, char **argv)
{
    srand(time(NULL));

    int *tab = NULL;
    tab                 = recover_data(argc, argv);
    com.nb_child_thread = tab[3];
    com.child           = malloc(sizeof(pthread_t) * tab[3]);

    create_pipes();

    Data_parent data_parent = init_data_parent(tab[4], tab[2], tab[3], tab[1]);
    Data_child data_child[com.nb_child_thread]; 

    open_pipes(&com);

    printf("Nb thread: %d\n", com.nb_child_thread);

    pthread_create(&com.parent, NULL, parent_thread, &data_parent);

    for( int i = 0; i < com.nb_child_thread; i++ )
    {
        data_child[i] = init_data_child(tab[4], tab[2], i);
        pthread_create(com.child + i, NULL, child_thread, &data_child[i]);
        printf("i,%d\n", i);
    }

    pthread_join(com.parent, NULL);

    for( int i = 0; i < com.nb_child_thread; i++ )
        pthread_join(com.child[i], NULL);
        
    delete_pipe();
    release_memory(com);
    return EXIT_SUCCESS;
}

void *child_thread(void *arg)
{  
    Data_child *d = (Data_child*) arg;

    for( int i = 0; i < d->nb_access; i++ )
    { 
        int id_page = (rand() % d->nb_pages);
        Address a = init_address(id_page, d->id_pthread);

        pthread_mutex_lock(&com.mutex_com);

        set_request(&a, com.fd1);

        pthread_mutex_unlock(&com.mutex_com);

        int response = get_response(com.fd2);
        printf("[Thread id: %d , response: %d] receive \n\n", d->id_pthread, response);
    }
    printf("Nb_access: %d\n", d->nb_access);
    pthread_exit(NULL);
}

void *parent_thread(void *arg)
{   
    Data_parent *d = (Data_parent*) arg;
    int nb_demande = d->nb_access * d->nb_pthread;
    int *t_hit     = malloc( sizeof(int) * d->nb_pthread );
    Page *t_page   = malloc( sizeof(Page) * d->nb_pages );
    
    //Intialize array page and array hit
    for( int i = 0; i < d->nb_pages; i++ )
    {   
        if( i < d->nb_pthread ) t_hit[i] = 0;

        t_page[i] = create_page(d->size_page, i);
    }
    for( int i = 0; i < nb_demande; i++ )
    {
        //printf("I: %d\n", i);
        Address *addr = get_request(com.fd1);
        set_response(addr->id_page, com.fd2);
    }

    free(t_hit);
    free(t_page);
    printf("fin\n");
    printf("nb page: %d, demande: %d\n", d->nb_pages, nb_demande);
    pthread_exit(NULL);
}

int *recover_data(int argc, char **argv)
{
    static int tab[5] = {0};

    if( argc > 2 )
    {
        fprintf(stderr, "Too much d'argument :|\n");
        exit(EXIT_FAILURE);
    }
    FILE *f = fopen("data", "r");
    if( !f )
    {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }
    if( fscanf(f, "%d %d %d %d %d", &tab[0], &tab[1], &tab[2], &tab[3], &tab[4]) < 0 )
    {
        fclose(f);
        perror("fscanf()");
        exit(EXIT_FAILURE);
    }
    fclose(f);

    return tab;
}

