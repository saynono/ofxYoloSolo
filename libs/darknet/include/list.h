#ifndef LIST_H
#define LIST_H

typedef struct node{
    void *val;
    struct node *next;
    struct node *prev;
} node;

typedef struct listDN{
    int size;
    node *front;
    node *back;
} listDN;

#ifdef __cplusplus
extern "C" {
#endif
listDN *make_list();
int list_find(listDN *l, void *val);

void list_insert(listDN *, void *);

void **list_to_array(listDN *l);

void free_list(listDN *l);
void free_list_contents(listDN *l);
void free_list_contents_kvp(listDN *l);

#ifdef __cplusplus
}
#endif
#endif
