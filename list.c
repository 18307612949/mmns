#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "list.h"
#include "data.h"

void *trim_list(struct LL *head, long ts)
{
	struct LL *tmp;

	/* walk the list, then hand the sublist over to free_whole_list */
	tmp = head;
	while (head) {
		if (((struct comm_t *)(tmp->data))->header.ts < ts)
			break;
	}

	/* remove everything after tmp */
	tmp->prev->next = NULL;
	free_whole_list(tmp);

	return tmp;
}

void *add_node(struct LL *head, void *data)
{
	/*
	 * We always add to the front of the list because adding data needs
	 * to happen more than removing from the list.
	 */

	struct LL *tmp;
	tmp = head;

	if (tmp) {

		tmp->next = malloc(sizeof(struct LL));

		if (tmp->next) {
			// set up the new head
			tmp->data = data;
			tmp->next = head;
			tmp->prev = NULL;

			// set up tmp->next (old head)
			tmp->next->prev = tmp;
		}
	} else { // head was NULL
		tmp = malloc(sizeof(struct LL));

		tmp->data = data;
		tmp->next = NULL;
		tmp->prev = NULL;

		head = tmp;
	}

	return head; // always return the head
}

// void print_list_reverse(struct LL *head)
// {
// 	if (head != NULL) {
// 		print_list_reverse(head);
// 		printf("data: %d\n", *((int *)head->data));
// 	}
// }
// 
// void print_list(struct LL *head)
// {
// 	for (; head; head = head->next) {
// 		printf("data: %d\n", *((int *)head->data));
// 	}
// }

void free_whole_list(struct LL *head)
{
	struct LL *tmp;
	while (head) {
		tmp = head;

		if (head->next) {
			head = head->next;
			free(tmp->data);
			free(tmp);
		} else {
			free(tmp->data);
			free(tmp);
			break;
		}
	}
}
