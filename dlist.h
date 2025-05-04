#ifndef DLIST_H
# define DLIST_H

#include <stdlib.h>

/* TODO: Reverse and remove until. */
struct dlist {
	void *data;
	struct dlist *prev;
	struct dlist *next;
};

#define DLIST_IMPL
#ifdef DLIST_IMPL

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

struct dlist *dlist_do_delete_last(struct dlist *head)
{
	struct dlist *t, *tmp;

	t = head;
	while (t->next->next != NULL)
		t = t->next;

	tmp = t->next;
	t->next = NULL;
	free(tmp);
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
	tt = t->next->prev;
	t->next->prev = t;

	free(tmp);
        return (head);
}

void dlist_print_forward(struct dlist *head)
{
	struct dlist *t;

	t = head;
	while (t != NULL) {
		fprintf(stdout, "%s\n", t->data);
		t = t->next;
	}
}

void dlist_print_backward(struct dlist *head)
{
        struct dlist *t;

	t = head;
	while (t->next != NULL)
		t = t->next;

        while (t != NULL) {
		fprintf(stdout, "%s\n", t->data);
		t = t->prev;
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

#endif

#endif
