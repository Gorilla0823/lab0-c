#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (!q) {
        return NULL;
    }

    q->tail = NULL;
    q->head = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q) {
        return;
    }

    list_ele_t *tmp = NULL;
    while (q->head) {
        tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
    q->tail = NULL;
    q->head = NULL;
    q->size = 0;
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* TODO: What should you do if the q is NULL? */
    if (!q) {
        return false;
    }

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    size_t length = strlen(s);       // length of input s
    char *str = malloc(length + 1);  // \0
    /* What if either call to malloc returns NULL? */

    /*If there is no enough space to allocate ,
    malloc will return non-null pointer or null pointer*/
    if (newh && str) {
        strncpy(str, s, length + 1);
        newh->value = str;
        newh->next = q->head;
        q->head = newh;
        if (!q->tail) {
            q->tail = newh;
        }
        /*increment 1*/
        q->size++;
        return true;
    } else {
        free(newh);  // Malloc Before
        free(str);   // Malloc Before
        return false;
    }
}



/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q) {
        return false;
    }

    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));

    size_t length = strlen(s);       // length of input s
    char *str = malloc(length + 1);  // \0

    // check
    /*if (!newt || !str) {
        if (!newt)
            free(newt);  // Malloc Before
        if (!str)
            free(str);  // Malloc Before
        return false;
    }*/
    if (newt && str) {
        strncpy(str, s, length + 1);
        newt->value = str;
        // if queue is empty
        if (!q->head) {
            q->head = newt;
        } else {
            q->tail->next = newt;
        }
        q->tail = newt;
        newt->next = NULL;
        /*increment 1*/
        q->size++;
        return true;
    } else {
        free(newt);  // Malloc Before
        free(str);   // Malloc Before
        return false;
    }
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    // check
    if (!q || !q->head) {
        return false;
    }

    list_ele_t *tmp = q->head;

    size_t length = strlen(tmp->value);
    // Bufsize smaller than length-1
    if (bufsize < length) {
        return false;
    }
    strncpy(sp, tmp->value, length);
    sp[length] = '\0';
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */

int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */

    // If queue is NULL
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || !q->head->next) {
        return;
    }

    list_ele_t *curr = q->head, *prev = NULL, *next = NULL;
    q->tail = q->head;
    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    q->head = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
/*list_ele_t* merge (list_ele_t* l1, list_ele_t* l2) {
    // merge with recursive
        if(!l2)
            return l1;
        if(!l1)
            return l2;
        if(l1->value<l2->value) {
            l1->next = merge(l1->next, l2);
            return l1;
        }
        else {
            l2->next = merge(l1, l2->next);
            return l2;
        }
    }

list_ele_t* mergeSortList (list_ele_t* head) {
        // merge sort
        if(!head || !head->next) {
            return head;
        }
        list_ele_t* fast = head->next;
        list_ele_t* slow = head;
        // split list
        while(fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        fast = slow->next;
        slow->next = NULL;
        // sort each list
        list_ele_t* l1 = mergeSortList(head);
        list_ele_t* l2 = mergeSortList(fast);
        // merge sorted l1 and sorted l2
        return merge(l1, l2);
}*/
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || q->size == 1) {
        return;
    }
    /*list_ele_t *tmp =  malloc(sizeof(list_ele_t));
    list_ele_t *q = tmp;
    list_ele_t*/

    /*q->head = mergeSortList(q->head);
    while (q->tail->next) {
        q->tail = q->tail->next;
    }*/
}
