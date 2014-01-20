#include <stack.h>
#include <stdlib.h>

/* Taille max de la pile */
#define STACK_SIZE 12

/* Structure de la pile */
typedef struct astack{
	void* contents[STACK_SIZE];
	int top;
} stack;


stack s;

/* Initialisation de la pile */
int init_stack() {
	s.top = 0;
	return s.top;
}


/* Operation d'empilement */
int push(void* element) {
	if (s.top == STACK_SIZE)
		return -1;
	else {
		s.contents[s.top++] = element;
		return s.top;
	}
}


/*Operation de depilement */
void* pop() {
	void* result = NULL;
	if (s.top != 0) {
		--s.top;
		result = s.contents[s.top];
	}
	return result;
}


/* Calcul du nombre d'elements dans la pile */
int size() {
	return s.top;
}
