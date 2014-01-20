#include <list.h>
#include <stdlib.h>
#include <stdio.h>


void init_list(list *l) {
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
}
	
	
void insert_head(list *l, void* element) {
	cell* new_cell = malloc(sizeof(cell));
	new_cell->content = element;
	new_cell->previous = NULL;
	new_cell->next = l->head;
	if (l->head != NULL)
		l->head->previous = new_cell;
	l->head = new_cell;
	if (l->tail == NULL)
			l->tail = l->head;

	(l -> size)++;
}


void* extract_head(list *l) {
	cell* tete = l -> head;

	if(l -> head -> next == NULL) free(l -> head);
	else {
		l -> head = l -> head -> next;
		free(l -> head -> previous);
	}
   
	return tete -> content;                            
}


void* extract_tail(list *l) {
	cell* queue = l -> tail;

	if(l -> tail -> previous == NULL) free(l -> tail);
	else {
		l -> tail = l -> tail -> previous;
		free(l -> tail -> next);
	}   

	return queue -> content; 
}


int list_size(struct list_type *l) {
	/*if(l == NULL) return 0;

	cell *tete = l -> head;
	int taille = 0;

	while(tete){
		taille++;
		tete = tete -> next;
	}

	return taille;*/

	return l -> size;
}
	

