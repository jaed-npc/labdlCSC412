#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * In this file, you will find the partial implementation of common doubly
 * linked list functions.
 *
 * Your first task is to debug some of the functions!
 *
 * After you have found all of the bugs, you will be writing three doubly
 * linked list functions and test them.
 *
 */

/**
 * find and return the length of the list
 *
 * given a pointer to the head of list
 */
int length_list(node_t* head_list) {
  int len = 0;
  if (!head_list) {
    return len;
  }
  node_t* current = head_list;
  while (current) {
    len++;
    // Have to update to the next node

    current = current -> next;
  }
  return len;
}

/**
 * returns the value of the head of the list
 *
 * given pointer to the head of the list
 */
void* get_first(node_t* head_list) { 
  //Checks if list is empty, if empty returns null
  if(!head_list){
    return NULL;
  }
  
  return head_list->data; }

/** returns the value of the last element of the list
 *
 * given a pointer to the head of the list
 */
void* get_last(node_t* head_list) {
  if (!head_list) {
    return NULL;
  }
  node_t* curr = head_list;
  while (curr->next) {
    curr = curr->next;
  }
  return curr->data;
}

/** TODO: implement this!
 * inserts element at the beginning of the list
 *
 * given a pointer to the head of the list, a void pointer representing the
 * value to be added, and the size of the data pointed to
 *
 * returns nothing
 */
void insert_first(node_t** head_list, void* to_add, size_t size) { 
  // Check if to_add is a valid pointer
  if(!to_add){
    //If to_add = null, do nothing
    return;
  }

  //Allocate memory for the new node

  node_t* new_element = (node_t*)malloc(sizeof(node_t));
  // If the memory allocation fails, do nothing
  if(!new_element){
    return;
  }

  //Allocate mem for the data and copy the data into the node

  void* new_data = malloc(size);
  if(!new_data){
    //Free the node if data allocationfailed
    free(new_element);
    return;
  }
//Copy the data and add into the new memory block
  memcpy(new_data, to_add, size);

  //Set the new nodes data and next pointer
  new_element-> data = new_data;
  //Point the new nodes next to the current head
  new_element-> next = *head_list;
  //New node is at the beginning, so the prev is NULL
  new_element->prev = NULL;

  //If the list is not empty, update the previous head's previous pointer
  if(*head_list){
    (*head_list)->prev = new_element;
  }

  //Update the head to point to the new node

  *head_list = new_element;




  //return; 
  }

/**
 * inserts element at the end of the linked list
 *
 * given a pointer to the head of the list, a void pointer representing the
 * value to be added, and the size of the data pointed to
 *
 * returns nothing
 */
void insert_last(node_t** head_list, void* to_add, size_t size) {
  if (!to_add) {
    return;
  }
  node_t* new_element = (node_t*)malloc(sizeof(node_t));
  if(!new_element){
    return;
  }
//Fix by properly allocating size bytes
  void* new_data = malloc(size);
  if(!new_data){
    free(new_element);
    return;
  }
  //void* new_data = malloc(size - 1);
  // Fix also properly by allocating size byted for memcpy
  memcpy(new_data, to_add, size);

  //memcpy(new_data, to_add, size - 1);
  new_element->data = new_data;
  new_element->next= NULL;

  if (!(*head_list)) {  // means the list is empty
    *head_list = new_element;
    new_element->prev = NULL;
    //new_element->next = NULL;
    *head_list = new_element;
    return;
  }

  node_t* curr = *head_list;
  while (curr->next) {
    curr = curr->next;
  }

  curr->next = new_element;
  new_element->prev = curr;
}

/** TODO: implement this!
 * gets the element from the linked list
 *
 * given a pointer to the head of the list and an index into the linked list
 * you need to check to see if the index is out of bounds (negative or longer
 * than linked list)
 *
 * returns the string associated with an index into the linked list
 */
void* get(node_t* head_list, int index) {
  //If the index is less than zero, then return NULL
  if(index < 0){
    return NULL;
  }
//Start from the head of the list
  node_t* current = head_list;
//Counter to rack the inex as we traverse the listt
  int i = 0;

  //Traverse the list until we reach the index specified or the end of the list

  while(current){
    if(i == index){
      // Found the index specified and return its data
      return current-> data;
    }
    // Move to the next node
    current = current-> next;
    i++;
  }
// If we reach the end of the list wihtout finding the index, return NULL
  return NULL;
  }

/**
 * removes element from linked list
 *
 * given a pointer to the head of list, a void pointer of the node to remove
 * you need to account for if the void pointer doesn't exist in the linked list
 *
 * returns 1 on success and 0 on failure of removing an element from the linked
 * list
 */
int remove_element(node_t **head_list, void *to_remove, size_t size) {
  if (!(*head_list)) {
    return 0;  // element doesn't exist
  }

  node_t* curr = *head_list;

  while (curr) {
    if (!memcmp(curr->data, to_remove, size)) {  // found the element to remove
      if (curr->next) {
        curr->next->prev = curr->prev;
      }
      if (curr == *head_list) {
        *head_list = curr->next;
      } else {
        curr->prev->next = curr->next;
      }
      free(curr->data);
      free(curr);
      return 1;
    }
    curr = curr->next;
  }

  return 0;
}

/**
 * reverses the list given a double pointer to the first element
 *
 * returns nothing
 */
void reverse_helper(node_t** head_list) {
  if(!(*head_list)){
    return;// Conditional to check if list is empty
  }
  node_t* curr = *head_list;
  node_t* placeholder = NULL;
  if (curr->next) {
    while (curr) {
      if (!curr->next) {
        *head_list = curr;
      }
      curr->prev = curr->next;
      curr->next = placeholder;
      placeholder = curr;
      curr = curr->prev;
    }
  }
}

/**
 * calls a helper function that reverses the linked list
 *
 * given a pointer to the first element
 *
 * returns nothing
 */
void reverse(node_t** head_list) {
  if (head_list) {
    reverse_helper(head_list);
  }
}

/**
 * removes the first element of the linked list if it exists
 *
 * given a pointer to the head of the linked list
 *
 * returns the void pointer of the element removed
 *
 */
void* remove_first(node_t** head_list) {
  if (!(*head_list)) {
    //Empty list
    return NULL;
  }

  node_t* curr = *head_list;
  //Store the data before freeing
  void* data = curr->data;
  //Update the head to the nexxt node
  *head_list = (*head_list)->next;

  if (*head_list) {
    (*head_list)->prev = NULL;
  }

  //free(curr->data);
  //Free the node
  free(curr);

  //return curr->data;
  // Returned the data that was stored
  return data;
}

/** TODO: implement this!
 * removes the last element of the linked list if it exists
 *
 * given a pointer to the head of the linked list
 *
 * returns the void pointer of the element removed
 *
 */
void* remove_last(node_t** head_list) { 
  //Check if the list is empty, just to verify wether we can remove the last element
  if(!(*head_list)){
    return NULL;
  }
  //Start from the head of the list
  node_t* curr = *head_list;

  // Traverse to the last node

  while(curr->next){
    //Keep moving to the next node until we reach the last one
    curr = curr-> next;

  }

  //Store the data of the last node to return later
  void* data = curr-> data;

  // If the last node is not the node update the second-last node's next pointer
  if (curr->prev){
    //Remove the last node by setting the next of its previoous node to NULL
    curr->prev->next = NULL;

  }else{
    //If the list only had one elementm set head_list to NULL
    *head_list = NULL;
  }
// Free the memory used by the node and its data
free(curr);
// Return the data off the removed last node
return data;
  //return NULL;
 }
