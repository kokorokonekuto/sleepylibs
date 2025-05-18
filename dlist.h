/* Doubly linked list. */

#ifndef DLIST_H
# define DLIST_H

struct dlist {
	void *data;
	struct dlist *prev;
	struct dlist *next;
};

/* Function prototypes. */
extern struct dlist *dlist_do_push_back(
	struct dlist *head, const void *data);
extern struct dlist *dlist_do_push_front(
	struct dlist *head, const void *data);
extern struct dlist *dlist_do_push_at(
	struct dlist *head, const void *data, size_t pos);
extern struct dlist *dlist_do_delete_first(struct dlist *head);
extern struct dlist *dlist_do_delete_last(struct dlist *head);
extern struct dlist *dlist_do_delete_at(
	struct dlist *head, size_t pos);
extern struct dlist *dlist_do_reverse(struct dlist *head);
extern struct dlist *dlist_do_remove_from_beg(
	struct dlist *head, size_t times);
extern struct dlist *dlist_do_remove_from_end(
	struct dlist *head, size_t times);
extern void dlist_do_free(struct dlist *head);
extern void dlist_do_free_data(struct dlist *head);
extern size_t dlist_do_count_nodes(struct dlist *head);


/* Macros. */
#define DLIST_DO_INIT(head)			\
	head = NULL

/* Push operations. */
#define DLIST_DO_PUSH_FRONT(head, data)		\
	head = dlist_do_push_front(head, data)
#define DLIST_DO_PUSH_BACK(head, data)		\
	head = dlist_do_push_back(head, data)
#define DLIST_DO_PUSH_AT(head, data, pos)	\
	head = dlist_do_push_at(head, data, pos)

/* Foreach macros. */
#define DLIST_DO_FOREACH_FORWARD(head)		\
	while (head != NULL)
#define DLIST_DO_FOREACH_BACKWARD(head)		\
	while (head->next != NULL) {		\
		head = head->next;		\
	}					\
	while (head != NULL)

/* Delete operations. */
#define DLIST_DO_DELETE_FIRST(head)		\
	head = dlist_do_delete_first(head)
#define DLIST_DO_DELETE_LAST(head)		\
	head = dlist_do_delete_last(head)
#define DLIST_DO_DELETE_AT(head, pos)		\
	head = dlist_do_delete_at(head, pos)

/* Remove operations. */
#define DLIST_DO_REMOVE_FROM_BEG(head, times)	\
	head = dlist_do_remove_from_beg(head, times)
#define DLIST_DO_REMOVE_FROM_END(head, times)	\
	head = dlist_do_remove_from_end(head, times)

/* Reverse list. */
#define DLIST_DO_REVERSE(head)			\
	head = dlist_do_reverse(head)

/* Count the number of nodes. */
#define DLIST_DO_COUNT_NODES(head)		\
	dlist_do_count_nodes(head)

/* Cleanup. */
#define DLIST_DO_FREE(head)			\
	dlist_do_free(head)
#define DLIST_DO_FREE_DATA(head)		\
	dlist_do_free_data(head);

#ifdef DLIST_IMPL

#include <stdlib.h>

static struct dlist *dlist_create_node(const void *data)
{
	struct dlist *node;

	if ((node = malloc(sizeof(struct dlist))) == NULL)
		return (NULL);

	node->data = (void *)data;
	node->prev = NULL;
	node->next = NULL;

	return (node);
}

static struct dlist *dlist_swap_node(struct dlist **left,
				     struct dlist **right)
{
	struct dlist *t;

	t = *left;
	*left = *right;
	*right = t;
	return (t);
}

struct dlist *dlist_do_push_back(struct dlist *head, const void *data)
{
	struct dlist *nn, *t;

	nn = dlist_create_node(data);
	if (nn == NULL)
		return (NULL);

	if (head == NULL)
		return (nn);

	t = head;
	while (t->next != NULL)
		t = t->next;

	nn->prev = t;
	t->next = nn;
	return (head);
}

struct dlist *dlist_do_push_front(struct dlist *head, const void *data)
{
	struct dlist *nn;

	if ((nn = dlist_create_node(data)) == NULL)
		return (NULL);

	if (head == NULL)
		return (nn);

	nn->prev = NULL;
	nn->next = head;

	/* We're linking with the newly created node.
	   This is because head->prev is initially NULL, and so
	   iterating over prev would cause stop here, and we won't
	   be able to read values in front, because we never set
	   head->prev. By linking this to itself, this creates a
	   link with prev. It will iterate until we found NULL,
	   that's in nn->prev. */
	head->prev = nn;
	head = nn;
        return (head);
}

struct dlist *dlist_do_push_at(struct dlist *head, const void *data,
			       size_t pos)
{
	struct dlist *t, *nn;

	if (pos == 0)
		return (dlist_do_push_front(head, data));

	t = head;
	while (--pos > 0 && t->next != NULL)
		t = t->next;
	
	if ((nn = dlist_create_node(data)) == NULL)
		return (NULL);

	nn->prev = t;
	nn->next = t->next;
        t->next = nn;

	/* Add node previous of
	   fprintf(stdout, "nn->next: %s\n", nn->next->data); */
	if (nn->next != NULL)
		nn->next->prev = nn;
	return (head);
}

struct dlist *dlist_do_delete_first(struct dlist *head)
{
	struct dlist *t;

	t = head;
	head = head->next;

	free(t);
        head->prev = NULL;
	return (head);
}

struct dlist *dlist_do_delete_last(struct dlist *head)
{
	struct dlist *t, *tmp;

	if (head->next == NULL)
		return (dlist_do_delete_first(head));
	t = head;
	while (t->next->next != NULL)
		t = t->next;

	tmp = t->next;
	t->next = NULL;
	free(tmp);
        return (head);
}

struct dlist *dlist_do_delete_at(struct dlist *head, size_t pos)
{
	struct dlist *t, *tmp, *tt;

	if (pos == 0)
	        return (dlist_do_delete_first(head));

	t = head;
	while (--pos > 0 && t->next->next != NULL)
		t = t->next;

	tmp = t->next;
	t->next = t->next->next;

	/* Exact node. */
	t->next->prev = t;

	free(tmp);
        return (head);
}

/* TODO: Remove until. */
struct dlist *dlist_do_reverse(struct dlist *head)
{
        struct dlist *t, *nn, *next;

	t = head;
	nn = NULL;
	while (t != NULL) {
		next = t->next;
		t->next = nn;
		/* t->next contains the address of the previous pointer,
		   and since they aren't yet reversed, we can just assign
		   the previous pointer to the t->next, can call it later
		   with t->next (e.g. 2), and t->prev (t->prev->prev) (e.g. 1). */
		t->prev = next;
	        nn = t;
	        t = next;
	}

	return (nn);
}

struct dlist *dlist_do_reverse2(struct dlist *head)
{
	struct dlist *t, *tmp;

	t = head;
	while (t != NULL) {
	        tmp = dlist_swap_node(&t->prev, &t->next);
		t = t->prev;
	}

        return (tmp->prev);
}

struct dlist *dlist_do_remove_from_beg(struct dlist *head, size_t times)
{
	struct dlist *t, *tmp;

	t = head;
	while (times-- > 0) {
		tmp = t;
		t = t->next;
		free(tmp);
		t->prev = NULL;
	}
	head = t;
	return (head);
}

/* TODO: foreach backward.
 */

struct dlist *dlist_do_remove_from_end(struct dlist *head, size_t times)
{
	struct dlist *t, *tmp;

	while (times-- > 0) {
		t = head;
		while (t->next->next != NULL)
			t = t->next;

		tmp = t->next;
		t->next = t->next->next;
		free(tmp);
	}

	return (head);
}

void dlist_do_free(struct dlist *head)
{
	struct dlist *t, *free_node;

	t = head;
	while (t != NULL) {
	        free_node = t;
		t = t->next;
		free(free_node);
	}
}

void dlist_do_free_data(struct dlist *head)
{
        struct dlist *t, *free_node, *data;

	t = head;
	while (t != NULL) {
		free_node = t;
		data = t->data;
		t = t->next;
		free(data);
		free(free_node);
	}
}

size_t dlist_do_count_nodes(struct dlist *head)
{
	struct dlist *t;
	size_t count;

	t = head;
	count = 0;
	while (t != NULL) {
		count++;
		t = t->next;
	}

	return (count);
}

#endif /* DLIST_IMPL */

#endif /* DLIST */
