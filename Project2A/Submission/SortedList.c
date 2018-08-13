//
// NAME: Zhengyuan Liu
// EMAIL: zhengyuanliu@ucla.edu
//

#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include "SortedList.h"

/**
 * SortedList_insert ... insert an element into a sorted list
 *
 *    The specified element will be inserted in to
 *    the specified list, which will be kept sorted
 *    in ascending order based on associated keys
 *
 * @param SortedList_t *list ... header for the list
 * @param SortedListElement_t *element ... element to be added to the list
 */
void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
    if (list == NULL || element == NULL)
        return;
    SortedListElement_t *p = list->next;  // the first (lowest valued) elment
    while (p != list) {
        if (strcmp(p->key, element->key) > 0)
            break;
        p = p->next;
    }
    if (opt_yield & INSERT_YIELD)
        sched_yield();
    SortedListElement_t *prev = p->prev;
    prev->next = element;
    element->prev = prev;
    element->next = p;
    p->prev = element;
    return;
}

/**
 * SortedList_delete ... remove an element from a sorted list
 *
 *    The specified element will be removed from whatever
 *    list it is currently in.
 *
 *    Before doing the deletion, we check to make sure that
 *    next->prev and prev->next both point to this node
 *
 * @param SortedListElement_t *element ... element to be removed
 *
 * @return 0: element deleted successfully, 1: corrtuped prev/next pointers
 *
 */
int SortedList_delete(SortedListElement_t *element) {
    if (element == NULL)
        return 1;
    SortedListElement_t * prevElement = element->prev;
    SortedListElement_t * nextElement = element->next;
    if (prevElement->next != element || nextElement->prev != element)  // corrtuped prev/next pointers
        return 1;
    prevElement->next = element->next;
    if (opt_yield & DELETE_YIELD)
        sched_yield();
    nextElement->prev = element->prev;
    return 0;
}

/**
 * SortedList_lookup ... search sorted list for a key
 *
 *    The specified list will be searched for an
 *    element with the specified key.
 *
 * @param SortedList_t *list ... header for the list
 * @param const char * key ... the desired key
 *
 * @return pointer to matching element, or NULL if none is found
 */
SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
    if (list == NULL || list->next == NULL)
        return NULL;
    SortedListElement_t *p = list->next;  // the first (lowest valued) elment
    while (p != list) {
        if (strcmp(p->key, key) > 0)
            return NULL;  // none is found
        if (strcmp(p->key, key) == 0)
            return p;
        if (opt_yield & LOOKUP_YIELD)
            sched_yield();
        p = p->next;
    }
    return NULL;  // none is found
}

/**
 * SortedList_length ... count elements in a sorted list
 *    While enumeratign list, it checks all prev/next pointers
 *
 * @param SortedList_t *list ... header for the list
 *
 * @return int number of elements in list (excluding head)
 *       -1 if the list is corrupted
 */
int SortedList_length(SortedList_t *list) {
    if (list == NULL)  // corrupted
        return -1;
    int length = 0;
    SortedListElement_t *p = list;
    while (p->next != list) {
        ++length;
        if (opt_yield & LOOKUP_YIELD)
            sched_yield();
        p = p->next;
        if(p->prev->next != p) //checks prev/next pointers
            return -1;
    }
    return length;
}
