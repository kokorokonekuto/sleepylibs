#ifndef SS_H
# define SS_H

/* Maximum stack size. */
#ifndef MAX_STACK_SIZE
# define MAX_STACK_SIZE    (10)
#endif

/* Single type of each element in the stack. */
#ifndef SS_ELEM_TYPE
# define SS_ELEM_TYPE     void
#endif

/* When we've pushed or popped a value onto/from the program stack. */
#define SS_ALL_OKAY        (0)

/* If the stack is full. */
#define SS_STACK_FULL      (-1)

/* If the stack is empty. */
#define SS_STACK_EMPTY     (-2)

/* Position is too large/high. */
#define SS_POS_TOO_HIGH    (-3)

struct ss {
        SS_ELEM_TYPE *p[MAX_STACK_SIZE];
	size_t elem_idx;
};

#define SS_IMPL
#ifdef SS_IMPL

#include <string.h>

void ss_do_init(struct ss *ss)
{
	ss->elem_idx = 0;
	memset(ss->p, '\0', MAX_STACK_SIZE);
}

int ss_do_push_back(struct ss *ss, const SS_ELEM_TYPE *elem)
{
	if (ss->elem_idx == MAX_STACK_SIZE)
		return (SS_STACK_FULL);

	++ss->elem_idx;
        ss->p[ss->elem_idx - 1] = (SS_ELEM_TYPE *)elem;
        return (SS_ALL_OKAY);
}

int ss_do_push_front(struct ss *ss, const SS_ELEM_TYPE *elem)
{
	size_t j;
	void *nelems[MAX_STACK_SIZE];
	int is_first;

	if (ss->elem_idx == MAX_STACK_SIZE)
		return (SS_STACK_FULL);
	
	nelems[0] = (SS_ELEM_TYPE *)elem;
	for (j = 0; j < ss->elem_idx; j++)
		nelems[j + 1] = ss->p[j];

	++ss->elem_idx;
	for (j = 0; j < ss->elem_idx; j++)
		ss->p[j] = nelems[j];

	return (SS_ALL_OKAY);
}

/* TODO: Comment. */
int ss_do_pop_back(struct ss *ss)
{
	if (ss->elem_idx == 0)
		return (SS_STACK_EMPTY);

        ss->p[--ss->elem_idx] = NULL;
        return (SS_ALL_OKAY);
}

int ss_do_pop_front(struct ss *ss)
{
	size_t j;

	if (ss->elem_idx == 0)
		return (SS_STACK_EMPTY);

	ss->p[0] = NULL;
	for (j = 0; ss->p[j + 1]; j++)
		ss->p[j] = ss->p[j + 1];

	--ss->elem_idx;
	return (SS_ALL_OKAY);
}

/* Reverse the order of stack elements. */
int ss_do_stack_rev(struct ss *ss)
{
	size_t i, j, mid;
        SS_ELEM_TYPE *t;

	if (ss->elem_idx == 0)
		return (SS_STACK_EMPTY);

	/* Swap until we found the middle position. */
	mid = ss->elem_idx >> 1;
	for (i = 0, j = 1; i < mid; i++, j++) {
		t = ss->p[ss->elem_idx - j];
		ss->p[ss->elem_idx - j] = ss->p[i];
		ss->p[i] = t;
	}

	return (SS_ALL_OKAY);
}

/* TODO: clear to n-th position. */
int ss_do_stack_clear(struct ss *ss)
{
	if (ss->elem_idx == 0)
		return (SS_STACK_EMPTY);
        while (ss->elem_idx > 0)
		ss->p[--ss->elem_idx] = NULL;

	return (SS_ALL_OKAY);
}

int ss_do_stack_clean_nth(struct ss *ss, size_t pos)
{
        size_t i, j, k;

	if (ss->elem_idx == 0)
		return (SS_STACK_EMPTY);
	if (pos > ss->elem_idx)
		return (SS_POS_TOO_HIGH);

	for (i = 0; i < ss->elem_idx; i++) {
		if (i == pos) {
			for (j = i, k = 1; j < MAX_STACK_SIZE; k++, j++)
				ss->p[j] = ss->p[i + k];
			--ss->elem_idx;
		        return (SS_ALL_OKAY);
		}
	}
}

SS_ELEM_TYPE *ss_do_get_elem(struct ss *ss, size_t idx)
{
	return (ss->p[idx]);
}

SS_ELEM_TYPE *ss_do_get_elem_chkd(struct ss *ss, size_t idx)
{
	if (idx >= ss->elem_idx)
		return (NULL);
	return (ss->p[idx]);
}

#endif /* SS_IMPL */

#endif /* SS_H */
