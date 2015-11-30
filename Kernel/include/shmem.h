void up();
void down();
void* get_mem();
void shm_init();
void switch_context();

typedef struct node {
	int pid;
	struct node *next;
}Nodo;
