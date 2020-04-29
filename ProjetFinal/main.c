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
    FILE *f = fopen("data", "r");
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

    com.nb_child_thread = tab[3];
    com.child           = malloc(sizeof(pthread_t) * tab[3]);

    mkfifo("tube_in", 0666);
    mkfifo("tube_out", 0666);
    com.fd1 = open("tube_in", O_RDWR);
    com.fd2 = open("tube_out", O_RDWR);

    Data_parent data_parent = init_data_parent(tab[4], tab[2], tab[3], tab[1]);
    pthread_create(&com.parent, NULL, parent_thread, &data_parent);
    
    Data_child data_child[com.nb_child_thread]; 
    printf("Nb thread: %d\n", com.nb_child_thread);

    for( int i = 0; i < com.nb_child_thread; i++ )
    {
        data_child[i] = init_data_child(tab[4], tab[2], i);
        pthread_create(com.child + i, NULL, child_thread, &data_child[i]);
        printf("i,%d\n", i);
    }
    
    pthread_join(com.parent, NULL);

    for( int i = 0; i < com.nb_child_thread; i++ )
        pthread_join(com.child[i], NULL);

    fclose(f);
    free(com.child);
    unlink("tube_in");
    unlink("tube_out");
    return EXIT_SUCCESS;
}

void *child_thread(void *arg)
{   
    Data_child *d = (Data_child*) arg;
    for( int i = 0; i < 10; i++ )
    { 
        sleep(1);
        pthread_mutex_lock(&com.mutex_com);

        int id_page = (rand() % d->nb_pages);

        Address a = init_address(id_page, d->id_pthread);
        
        if( write(com.fd1, &a, sizeof(Address)) < 0 )
        {
            perror("In child write()");
            return NULL;
        }
        printf("[Child id: %d , p: %d] message sended...\n", d->id_pthread, id_page);
        
        pthread_cond_signal(&com.cond_parent); 
        pthread_cond_wait(&com.cond_child, &com.mutex_com);

        int response = 0;
        
        if ( read(com.fd2, &response, sizeof(int)) < 0 ) 
        {
            perror("In child read()");
            return NULL;
        }
        pthread_mutex_unlock(&com.mutex_com);
        printf("[Child id: %d , p: %d] receive \n\n", d->id_pthread, response);
    }
    return NULL;
}

void *parent_thread(void *arg)
{   
    Data_parent *d = (Data_parent*) arg;
    //printf("nb page: %d\n", d->nb_pages);
    int *t_hit     = malloc( sizeof(int) * d->nb_pthread );
    Page *t_page   = malloc( sizeof(Page) * d->nb_pages );
    
    //Intialize array page and array hit
    for( int i = 0; i < d->nb_pages; i++ )
    {   
        if( i < d->nb_pthread ) t_hit[i] = 0;

        t_page[i] = create_page(d->size_page, i);
    }
    for(int i = 0; i < 100; i++ )
    {
        pthread_mutex_lock(&com.mutex_com);

        //printf("Parent wait for message....\n");

        pthread_cond_wait(&com.cond_parent, &com.mutex_com);

        //printf("In parent after wait....\n");
        Address addr ;
        if( read(com.fd1, &addr, sizeof(Address)) < 0)
        {
            perror("In parent read()");
            goto release;
        }
        //printf("child id: %d\n", addr.id_pthread);
        //printf("In Parent -> child: [id: %d - page: %d]\n", addr.id_pthread, addr.id_page);

        if( write(com.fd2, &(addr.id_page), sizeof(int)) < 0 )
        {
            perror("In parent write()");
            goto release;
        }   
        //int nb_demandes = d->nb_pthread * d->nb_access;
        printf("I: %d\n", i);

        pthread_cond_signal(&com.cond_child);
        pthread_mutex_unlock(&com.mutex_com);

       // printf("Demande: %d\n", nb_demandes);
    }
    
    release:
        free(t_hit);
        free(t_page);
    free(t_hit);
    free(t_page);
    return NULL;
}
