/* Singly linked list. */

#ifndef SLL_H
# define SLL_H

/* Default data type. */
#ifndef SLL_DATA_TYPE
# define SLL_DATA_TYPE    void
#endif

struct sll_node {
	SLL_DATA_TYPE *data;
	struct sll_node *next;
};

/* Push an element to the end of the list. */
extern struct sll_node *sll_do_push_back(
	struct sll_node *head, const SLL_DATA_TYPE *data);
/* Push an element to the front of the list. */
extern struct sll_node *sll_do_push_front(
	struct sll_node *head, const SLL_DATA_TYPE *);
/* Push an element at a specific position in the list. */
extern struct sll_node *sll_do_push_at(
	struct sll_node *head, const SLL_DATA_TYPE *data,
	size_t pos);
/* Remove the first element from the list. */
extern struct sll_node *sll_do_remove_first(struct sll_node *head);
/* Remove the last element from the list. */
extern struct sll_node *sll_do_remove_last(struct sll_node *head);
/* Remove an element at a specific position from the list. */
extern struct sll_node *sll_do_remove_at(
	struct sll_node *head, size_t pos);
/* Remove the number of elements. */
extern struct sll_node *sll_do_remove_until(
	struct sll_node *head, size_t till);
/* Reverse the linked-list. */
extern struct sll_node *sll_do_reverse_list(
	struct sll_node *head);
/* Test whether the list is empty or not. */
extern int sll_do_is_empty(struct sll_node *head);
/* Count the number of nodes in the list. */
extern size_t sll_do_count_lists(struct sll_node *head);
/* Free all allocated nodes. */
extern void sll_do_free(struct sll_node *head);
/* Free all allocated nodes including the data pointer. */
extern void sll_do_free_data_node(struct sll_node *head);

#define SLL_DO_INIT(head)    do { head = NULL; } while (0)
#define SLL_DO_PUSH_BACK(head, data)				\
	do { head = sll_do_push_back(head, data); } while (0)
#define SLL_DO_PUSH_FRONT(head, data)				\
	do { head = sll_do_push_front(head, data); } while (0)
#define SLL_DO_PUSH_AT(head, data, pos)					\
	do { head = sll_do_push_at(head, data, pos); } while (0)
#define SLL_DO_REMOVE_FIRST(head)				\
	do { head = sll_do_remove_first(head); } while (0)
#define SLL_DO_REMOVE_LAST(head)				\
	do { head = sll_do_remove_last(head); } while (0)
#define SLL_DO_REMOVE_ATPOS(head, pos)				\
	do { head = sll_do_remove_at(head, pos); } while (0)
#define SLL_DO_REMOVE_UNTIL(head, till)					\
	do { head = sll_do_remove_until(head, till); } while (0)
#define SLL_DO_REVERSE_LIST(head)		\
	do { head = sll_do_reverse_list(head); } while (0)
#define SLL_DO_IS_EMPTY(head)			\
        sll_do_is_empty(head)
#define SLL_DO_FOREACH(head, temp)		\
        temp = head;				\
	while (temp != NULL)
#define SLL_DO_COUNT_LISTS(head)		\
	sll_do_count_lists(head)
#define SLL_DO_FOREACH_NODE(head, fn, xarg)	\
	sll_do_foreach_node(head, fn, xarg)
#define SLL_DO_FOREACH_FN(head, fn)		\
	sll_do_foreach_fn(head, fn)
#define SLL_DO_FREE(head)			\
	sll_do_free(head)
#define SLL_DO_FREE_DATA_NODE(head)		\
	sll_do_free_data_node(head)

#ifdef SLL_IMPL
#include <stdlib.h>

/* TODO: Maybe make it an object file? */
static struct sll_node *sll_create_node(const SLL_DATA_TYPE *data)
{
	struct sll_node *node;

	node = malloc(sizeof(struct sll_node));
	if (node == NULL)
	        return (NULL);

	node->data = (void *)data;
	node->next = NULL;
	return (node);
}

struct sll_node *sll_do_push_back(struct sll_node *head,
				  const SLL_DATA_TYPE *data)
{
	struct sll_node *t, *new_node;

	if ((new_node = sll_create_node(data)) == NULL)
		return (NULL);
	if (head == NULL)
		return (new_node);

	t = head;
	/* t here shouldn't be checked for NULL, as we'll need t
	   later for t->next. */
	while (t->next != NULL)
		t = t->next;

	/* We're dereferencing the t pointer that points to head
	   and setting the next value to head (we've dereferenced). */
	t->next = new_node;
	return (head);
}

struct sll_node *sll_do_push_front(struct sll_node *head,
				   const SLL_DATA_TYPE *data)
{
        struct sll_node *nn;

	nn = sll_create_node(data);
        nn->next = head;
	head = nn;
	return (head);
}

struct sll_node *sll_do_push_at(struct sll_node *head,
				const SLL_DATA_TYPE *data, size_t pos)
{
	struct sll_node *t, *nn;

	if (pos == 0)
		return (sll_do_push_front(head, data));

	t = head;
        while (--pos > 0 && t->next != NULL)
		t = t->next;

	if (t == NULL)
		return (sll_do_push_back(head, data));

	if ((nn = sll_create_node(data)) == NULL)
		return (NULL);
	nn->next = t->next;
	t->next = nn;
	return (head);
}

struct sll_node *sll_do_remove_first(struct sll_node *head)
{
	struct sll_node *t;

	if (head == NULL)
		return (NULL);
	t = head;
        head = head->next;
	free(t);
        return (head);
}

struct sll_node *sll_do_remove_last(struct sll_node *head)
{
        struct sll_node *t;

	if (head == NULL)
		return (NULL);
	if (head->next == NULL) {
		free(head);
		return (NULL);
	}

	t = head;
	/* Find the last node, from 2 nodes ahead. */
	while (t->next->next != NULL)
		t = t->next;

	/* t->next is the last node. */
        free(t->next);
	t->next = NULL;
        return (head);
}

struct sll_node *sll_do_remove_at(struct sll_node *head, size_t pos)
{
	struct sll_node *t, *new_next;

	if (head == NULL)
		return (NULL);
	if (pos == 0)
		return (sll_do_remove_first(head));

	t = head;
	while (--pos && t->next != NULL)
		t = t->next;

        new_next = t->next;
        t->next = t->next->next;
	free(new_next);
	return (head);
}

struct sll_node *sll_do_remove_until(struct sll_node *head, size_t till)
{
	struct sll_node *t;

	if (head == NULL)
		return (NULL);

        while (till > 0 && head->next != NULL) {
		t = head;
		--till;
		head = head->next;
		free(t);
	}
	return (head);
}

struct sll_node *sll_do_reverse_list(struct sll_node *head)
{
	struct sll_node *t, *prev, *nn;

	t = head;
	prev = NULL;
	while (t != NULL) {
		nn = t->next;
		t->next = prev;
		prev = t;
		t = nn;
	}

	return (prev);
}

int sll_do_is_empty(struct sll_node *head)
{
	if (head == NULL)
		return (1);
	return (0);
}

size_t sll_do_count_lists(struct sll_node *head)
{
	struct sll_node *t;
	size_t ncount;

	if (head == NULL)
		return (0);

	t = head;
	ncount = 0;
	while (t != NULL) {
		ncount++;
		t = t->next;
	}

	return (ncount);
}

void sll_do_free(struct sll_node *head)
{
	struct sll_node *t, *x;

	t = head;
	while (t->next != NULL) {
		x = t;
	        t = t->next;
		free(x);
	}

	free(t);
}

void sll_do_free_data_node(struct sll_node *head)
{
        struct sll_node *t, *node;

	t = head;
	while (t->next != NULL) {
		node = t;
		free(t->data);
		t = t->next;
		free(node);
	}

	free(t);
}

#endif /* SLL_IMPL */

#endif /* SLL_H */
