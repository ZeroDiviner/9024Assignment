//
//  main.c
//  ASS1
//
//  Created by Jasper on 4/3/19.
//  Copyright © 2019 Jasper. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data structures representing DLList

// data type for nodes
typedef struct DLListNode {
    int  value;  // value (int) of this list item
    struct DLListNode *prev;
    // pointer previous node in list
    struct DLListNode *next;
    // pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
    int  size;      // count of items in list
    DLListNode *first; // first node in list
    DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
    DLListNode *new;
    new = malloc(sizeof(DLListNode));
    assert(new != NULL);
    new->value = it;
    new->prev = new->next = NULL;
    return new;
}

// create a new empty DLList
DLList *newDLList()
{
    DLList *L;
    
    L = malloc(sizeof (struct DLList));
    assert (L != NULL);
    L->size = 0;
    L->first = NULL;
    L->last = NULL;
    return L;
}

// create a DLList from a text file
// put your time complexity analysis for CreateDLListFromFileDlist() here
//create  a Doubly linked list with file or stdin only use one while loop, the time complexity is O(n)
DLList *CreateDLListFromFileDlist(const char *filename)
{
    if (strcmp(filename,"stdin")==0){
        DLList *returnList=newDLList();
        char buffer[100];
        int number;
        int j=0;
        while(fgets(buffer, sizeof buffer, stdin) != NULL) {
            if(sscanf(buffer, "%d", &number) != 1) {    // or strtol perhaps
                break;
            }
            DLListNode *node=newDLListNode(number);
            if (j==0){
                returnList->first=node;
                returnList->last=node;
                returnList->size++;
                j++;
            }else{
                DLListNode *temp=returnList->last;
                node->prev=temp;
                temp->next=node;
                returnList->last=node;
                returnList->size++;
            }
        }
        return returnList;
    }
    // put your code here
    FILE *fp;
    int ch,i=0;
    if ((fp=fopen(filename,"r"))==NULL){
        printf("file cannot open \n");
        exit(0);
    }
    DLList *L=newDLList();
    while ((fscanf(fp, "%d", &ch))!=EOF){
        DLListNode *node=newDLListNode(ch);
        if (i==0){
            L->first=node;
            L->last=node;
            L->size++;
        }
        else {
            DLListNode *temp=L->last;
            node->prev=temp;
            temp->next=node;
            node->next=NULL;
            L->last=node;
            L->size++;
        }
        i++;
    }
    fclose(fp);
    return L;
    
}

// clone a DLList
// put your time complexity analysis for cloneList() here
// clone a list need a while loop to iterate each node of list,time complexity is O(n)
DLList *cloneList(DLList *u)
{
    // put your code here
    DLList *clone=newDLList();
    DLListNode *node = u->first;
    int i=0;
    while (node!=NULL){
        if (i==0){
            DLListNode *newnode=newDLListNode(u->first->value);
            clone->first=newnode;
            clone->last=newnode;
            clone->size++;
        }
        else{
            DLListNode *newnode=newDLListNode(node->value);
            DLListNode *temp=clone->last;
            temp->next=newnode;
            newnode->prev=temp;
            clone->last=newnode;
            newnode->next=NULL;
            clone->size++;
        }
        i++;
        node=node->next;
    }
    return clone;
}

// a new-added function to remove all the duplicate in a doubly linked list
// time complexity for this function is O(n^2)
//DLList *removeDup(DLList *u){
//    DLList *new_list=cloneList(u);
//    DLListNode *node_of_u_none_repeat=new_list->first;
//    while (node_of_u_none_repeat!=NULL){
//        if (node_of_u_none_repeat ==new_list->first){
//            node_of_u_none_repeat=node_of_u_none_repeat->next;
//            continue;
//        }
//        DLListNode *innner_node=node_of_u_none_repeat->prev;
//        while(innner_node!=NULL){
//            if (innner_node->value == node_of_u_none_repeat->value){
//                if(innner_node!=new_list->last){
//                    DLListNode *prev_node=node_of_u_none_repeat->prev;
//                    DLListNode *next_node=node_of_u_none_repeat->next;
//                    if (next_node==NULL){
//                        prev_node->next=NULL;
//                        new_list->last=prev_node;
//                        break;
//                    }
//                    else{
//                        prev_node->next=next_node;
//                        next_node->prev=prev_node;
//                        node_of_u_none_repeat=next_node;
//                    }
//                }else{
//                    DLListNode *prev_no=node_of_u_none_repeat->prev;
//                    prev_no->next=NULL;
//                    new_list->last=prev_no;
//                    break;
//                }
//            }
//            else{
//                innner_node=innner_node->prev;
//            }
//        }
//        node_of_u_none_repeat=node_of_u_none_repeat->next;
//    }
//    return new_list;
//}

// compute the union of two DLLists u and v
// this function first clone(O(n)) the argument u, which is also a doubly linked list, then add elements to the cloned-list(O(n^2)) with one loop in the other loop,so the time complexity is O(n^2)
DLList *setUnion(DLList *u, DLList *v)
{
    // put your code here
//    int flag=0;
    DLListNode *node_of_u=u->first;
    DLListNode *node_of_v=v->first;
    DLList *new_list=cloneList(u);
//    new_list=removeDup(u);
    while (node_of_v !=NULL){
        int flag=0;
        int value=node_of_v->value;
        while (node_of_u!=NULL){
            if (node_of_u->value==value){
                flag=1;
                break;
            }
            else{
                node_of_u=node_of_u->next;
            }
        }
        if (flag==0){
//            printf("flag==0,so add nodev=%d\n",node_of_v->value);
            DLListNode *temp=new_list->last;
            DLListNode *new_node=newDLListNode(value);
            temp->next=new_node;
            new_node->prev=temp;
            new_list->last=new_node;
            new_list->size++;
        }
        node_of_v=node_of_v->next;
        node_of_u=u->first;
    }
    return new_list;
}

// compute the insection of two DLLists u and v
// put your time complexity analysis for intersection() here
// this function  add common element to a new created list,so the time complexity is O(n^2) with a loop in the other one, so the time complexity is O(n^2)
DLList *setIntersection(DLList *u, DLList *v)
{
//    DLList *new_u=removeDup(u);
//    DLList *new_v=removeDup(v);
    DLList *new_u=u;
    DLList *new_v=v;
    DLList *new_return=newDLList();
    DLListNode *unode=new_u->first;
    DLListNode *vnode=new_v->first;
    while (unode!=NULL){
        int uvalue=unode->value;
        while(vnode!=NULL){
            int vvalue=vnode->value;
            if (vvalue==uvalue){
                if (new_return->first==NULL){
                    DLListNode *newfirst=newDLListNode(vvalue);
                    new_return->first=newfirst;
                    new_return->last=newfirst;
                    new_return->size++;
                }
                else{
                    DLListNode *newnode=newDLListNode(vvalue);
                    DLListNode *temp=new_return->last;
                    newnode->prev=temp;
                    temp->next=newnode;
                    new_return->last=newnode;
                    new_return->size++;
                }
            }
            vnode=vnode->next;
        }
        unode=unode->next;
        vnode=new_v->first;
    }
    return new_return;
}
//
// free up all space associated with list
// put your time complexity analysis for freeDLList() here
// the time complexity is O(n)(with only one loop)

void freeDLList(DLList *L)
{
    DLListNode * Lnode=L->first;
    while (Lnode!=NULL){
        DLListNode *temp=Lnode->next;
        if (temp!=NULL){
            
            free(Lnode);
            
            Lnode=temp;
        }
        else{
            
            free(Lnode);
            break;
        }
    }
}


// display items of a DLList
// put your time complexity analysis for printDDList() here
// the time complexity is O(n),cause there is only one loop in this function
void printDLList(DLList *u)
{
    // put your code here
    DLListNode *node = u->first;
    while (node!=NULL){
        printf("%d\n",node->value);
        node=node->next;
    }
}

int main()
{
    DLList *list1, *list2, *list3, *list4;
    
    list1=CreateDLListFromFileDlist("File1.txt");
    printDLList(list1);

    list2=CreateDLListFromFileDlist("File2.txt");
    printDLList(list2);

    list3=setUnion(list1, list2);
    printDLList(list3);

    list4=setIntersection(list1, list2);
    printDLList(list4);

    freeDLList(list1);
    freeDLList(list2);
    freeDLList(list3);
    freeDLList(list4);

    printf("please type all the integers of list1\n");
    list1=CreateDLListFromFileDlist("stdin");


    printf("please type all the integers of list2\n");
    list2=CreateDLListFromFileDlist("stdin");

    list3=cloneList(list1);
    printDLList(list3);
    list4=cloneList(list2);
    printDLList(list4);

    freeDLList(list1);
    freeDLList(list2);
    freeDLList(list3);
    freeDLList(list4);

    return 0;
}
