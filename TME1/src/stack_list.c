#include <list.h>
#include <stack.h>
#include <stdio.h>
#include <stdlib.h>

list l;

/* Initialisation de la pile */
int init_stack(){
	init_list(&l);
	return 0;
}

/* Operation d'empilement */
int push(void* element){
	insert_head(&l, element);
	return 0;
}

/* Operation de depilement ; l'element au sommet est supprime */
void* pop(){
	return extract_head(&l);
}

/* Calcul du nombre d'elements dans la pile */
int size(){
	return list_size(&l);
}
