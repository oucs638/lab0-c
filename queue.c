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
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Free queue structure */
    if (!q)
        return;
    while (q->head)
        q_remove_head(q, NULL, 0);
    q->tail = NULL;
    // free(q->size);
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
    if (!q)
        return false;
    // make sure allocate the memory success
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    newh->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    // copy the string
    memset(newh->value, 0, (strlen(s) + 1) * sizeof(char));
    strncpy(newh->value, s, strlen(s));
    newh->value[strlen(s)] = '\0';
    // link newh
    newh->next = q->head;
    q->head = newh;
    if (!q->tail)
        q->tail = newh;
    // refresh the size of queue
    q->size++;
    return true;
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
    if (!q)
        return false;
    // make sure allocate the memory success
    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    newt->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!newt->value) {
        free(newt);
        return false;
    }
    // copy the string
    memset(newt->value, 0, (strlen(s) + 1) * sizeof(char));
    strncpy(newt->value, s, strlen(s));
    newt->value[strlen(s)] = '\0';
    // link newt
    newt->next = NULL;
    if (!q->head)
        q->head = newt;
    else
        q->tail->next = newt;
    q->tail = newt;
    // refresh the size of queue
    q->size++;
    return true;
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
    if (!q || !q->head || !q->size)
        return false;
    if (sp && q->head->value)
        strncpy(sp, q->head->value, bufsize - 1);
    list_ele_t *tmp = q->head;
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
    if (q && q->size)
        return q->size;
    return 0;
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
    if (!q || !q->head || q->size < 2)
        return;
    list_ele_t *prv = NULL, *now = q->head;
    q->tail = q->head;
    while (now) {
        list_ele_t *tmp = now;
        now = now->next;
        tmp->next = prv;
        prv = tmp;
    }
    q->head = prv;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head || q->size < 2)
        return;
    q->head = mergeSortList(q->head);
    list_ele_t *tmp = q->head;
    // find the new tail
    while (tmp->next)
        tmp = tmp->next;
    q->tail = tmp;
}

list_ele_t *mergeSortList(list_ele_t *head)
{
    if (!head || !head->next)
        return head;
    list_ele_t *fast = head->next, *slow = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *ls1 = mergeSortList(head);
    list_ele_t *ls2 = mergeSortList(fast);
    return merge(ls1, ls2);
}

list_ele_t *merge(list_ele_t *ls1, list_ele_t *ls2)
{
    list_ele_t *res = NULL, *lst = NULL;
    while (ls1 || ls2) {
        int tmp = 0;
        if (ls1 && ls2)
            tmp = (strnatcmp(ls1->value, ls2->value) < 0) ? 1 : -1;
        if (!ls1 || tmp == -1) {
            if (!res)
                res = ls2;
            else
                lst->next = ls2;
            lst = ls2;
            ls2 = ls2->next;
        } else if (!ls2 || tmp == 1) {
            if (!res)
                res = ls1;
            else
                lst->next = ls1;
            lst = ls1;
            ls1 = ls1->next;
        }
    }
    return res;
}



static int compare_r(char const *st1, char const *st2)
{
    int bis = 0;
    for (;; st1++, st2++)
        if (!isdigit((unsigned char) *st1))
            return (!isdigit((unsigned char) *st2)) ? bis : -1;
        else if (!isdigit((unsigned char) *st2))
            return 1;
        else if (!*st1 && !*st2)
            return bis;
        else if (!bis)
            bis = (*st1 < *st2) ? -1 : (*st1 > *st2) ? 1 : bis;
    return 0;
}

static int compare_l(char const *st1, char const *st2)
{
    for (;; st1++, st2++)
        if (!isdigit((unsigned char) *st1))
            return (!isdigit((unsigned char) *st2)) ? 0 : -1;
        else if (!isdigit((unsigned char) *st2))
            return 1;
        else if (*st1 < *st2)
            return -1;
        else if (*st1 > *st2)
            return 1;
    return 0;
}

static int compare_a(char const *st1, char const *st2)
{
    int id1 = 0, id2 = 0;
    while (1) {
        char ch1 = st1[id1], ch2 = st2[id2];
        while (isspace((unsigned char) ch1))
            ch1 = st1[++id1];
        while (isspace((unsigned char) ch2))
            ch2 = st2[++id2];
        if (isdigit((unsigned char) ch1) && isdigit((unsigned char) ch2)) {
            int tmp = 0;
            if (ch1 == '0' || ch2 == '0')
                tmp = compare_l(st1 + id1, st2 + id2);
            else
                tmp = compare_r(st1 + id1, st2 + id2);
            if (tmp)
                return tmp;
        }
        if (!ch1 && !ch2)
            return 0;
        if (ch1 < ch2)
            return -1;
        if (ch1 > ch2)
            return 1;
        ++id1;
        ++id2;
    }
}

int strnatcmp(char const *st1, char const *st2)
{
    return compare_a(st1, st2);
}