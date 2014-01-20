/**
 * Definition d'une liste doublement chainee 
 **/


#define POSIX_SOURCE 1


typedef struct cell_type {
	struct cell_type *previous;
	struct cell_type *next;
	void *content;
} cell;


struct list_type {
	cell* head;
	cell* tail; 
	int size;
} ;

typedef struct list_type list;


/* Initialisation de la liste */
void init_list(list *l);

/* Insertion d'un element en tete de liste */
void insert_head(list *l, void* element);

/* Extraction (avec suppression) d'un element en tete de liste */
void* extract_head(list *l);

/* Extraction (avec suppression) d'un element en fin de liste */
void* extract_tail(list *l);

/* Calcul du nombre d'elements dans la liste */
int list_size(list *l);
