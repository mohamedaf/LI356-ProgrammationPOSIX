#include <list.h>
#include <fifo.h>
#include <stdio.h>
#include <stdlib.h>

list l;

/* Initialisation de la file */
int init_queue(){
    init_list(&l);
    return 0;
}

/* Operation d'empilement */
int queue(void* element){
    insert_head(&l, element);
    return 0;
}

/* Operation de depilement ; l'element au sommet est supprime */
void* dequeue(){
    return extract_tail(&l);
}

/* Calcul du nombre d'elements dans la pile */
int size(){
    return list_size(&l);
}

