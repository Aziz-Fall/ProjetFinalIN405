#include "communication.h"
#include "time.h"
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

static Communication com = {
                            .mutex_com   = PTHREAD_MUTEX_INITIALIZER,
                            .cond_parent = PTHREAD_COND_INITIALIZER,
                            .cond_child  = PTHREAD_COND_INITIALIZER, 
                           };

int main(int argc, char **argv)
{
    srand(time(NULL));
    if( argc > 2 )
    {
        fprintf(stderr, "Too much d'argument :|\n");
        exit(EXIT_FAILURE);
    }
    FILE *f = fopen(argv[1], "r");
    if( !f )
    {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }
    int tab[5] = {0};
    if( fscanf(f, "%d %d %d %d %d", &tab[0], &tab[1], &tab[2], &tab[3], &tab[4]) < 0 )
    {
        perror("fscanf()");
        exit(EXIT_FAILURE);
    }
    if( pipe(com.pipe_child) < 0 || pipe(com.pipe_parent) < 0 )
    {
        perror("pipe()");
        exit(EXIT_FAILURE);
    }

    com.nb_child_thread = tab[3];

    pthread_t thread_p, thread_c[tab[3]];

    Data_parent data_parent = init_data_parent(tab[4], tab[2], tab[3], tab[1]);
    pthread_create(&thread_p, NULL, parent_thread, &data_parent);
    
    Data_child data_child[com.nb_child_thread]; 
    printf("Nb thread: %d\n", com.nb_child_thread);

    for( int i = 0; i < com.nb_child_thread; i++ )
    {
        data_child[i] = init_data_child(tab[4], tab[2], i);
        pthread_create(thread_c + i, NULL, child_thread, &data_child[i]);
        printf("i,%d\n", i);
    }
    
    for( int i = 0; i < com.nb_child_thread; i++ )
        pthread_join(thread_c[i], NULL);
    
    pthread_join(thread_p, NULL);
    fclose(f);
    return EXIT_SUCCESS;
}

void *child_thread(void *arg)
{
    Data_child *d = (Data_child*) arg;

    printf("thread: %d\n", d->id_pthread);

    pthread_exit(NULL);
}

void *parent_thread(void *arg)
{
    Data_parent *d = (Data_parent*) arg;
    printf("nb page: %d\n", d->nb_pages);
    int *t_hit   = malloc( sizeof(int) * d->nb_pthread );
    Page *t_page = malloc( sizeof(Page) * d->nb_pages );
    
    //Intialize array page and array hit
    for( int i = 0; i < d->nb_pages; i++ )
    {   
        if( i < d->nb_pthread ) t_hit[i] = 0;

        t_page[i] = create_page(d->size_page, i);
    }

    int nb_demandes = d->nb_pthread * d->nb_access;
    printf("Demande: %d\n", nb_demandes);
    free(t_hit);
    free(t_page);
    pthread_exit(NULL);
}
