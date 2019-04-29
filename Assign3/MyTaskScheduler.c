#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

// This template is only a guide
// You need to include additional fields, data structures and functions

// data type for heap nodes
typedef struct HeapNode {
    // each node stores the priority (key), name, execution time,
    //  release time and deadline of one task
    int key; //key of this task
    int TaskName;  //task name
    int Etime; //execution time of this task
    int Rtime; // release time of this task
    int Dline; // deadline of this task
    int degree;
    int Core;
    int StartTime;
    int MissFlag;
    struct HeapNode *left_child;
    struct HeapNode *parent;
    struct HeapNode *sibling;
} HeapNode;

//data type for a priority queue (heap)
typedef struct BinomialHeap{ //this is heap header
    int  size;
    HeapNode *head;
} BinomialHeap;

// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d, ...)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
    // ... you need to define other parameters yourself)
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->key = k;
    new->TaskName = n;
    new->Etime = c;
    new->Rtime = r;
    new->Dline = d;
    new->left_child = NULL;
    new->parent = NULL;
    new->sibling = NULL;
    new->degree = 0;
    //    new->Core = NULL;
    //    new->StartTime = NULL;
    //    new->MissFlag = NULL;
    // initialise other fields
    return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
    BinomialHeap *T;
    T = malloc(sizeof(BinomialHeap));
    assert (T != NULL);
    T->size = 0;
    T->head = NULL;
    return T;
}

//merge two heap in a single heap with all heap in increasing order(degree),time complexity O(logn)
BinomialHeap *mergeTwoHeap(BinomialHeap *h1, BinomialHeap *h2){
    BinomialHeap *new_heap = newHeap();
    HeapNode *h1_pointer = h1->head;
    HeapNode *h2_pointer = h2->head;
    HeapNode *new_pointer = NULL;
    while (h1_pointer != NULL ||  h2_pointer != NULL){
        if (h1_pointer == NULL){
            if (new_pointer == NULL){
                new_heap->head = h2_pointer;
                break;
            }
            new_pointer->sibling = h2_pointer;
            break;
        }
        else if (h2_pointer == NULL){
            if (new_pointer == NULL){
                new_heap->head = h1_pointer;
                break;
            }
            new_pointer->sibling = h1_pointer;
            break;
        }
        if (h1_pointer->degree >= h2_pointer->degree){
            if (new_heap->head == NULL){
                new_heap->head = h2_pointer;
                new_pointer = new_heap->head;
                h2_pointer = h2_pointer->sibling;
            }else{
                new_pointer->sibling = h2_pointer;
                new_pointer = new_pointer->sibling;
                h2_pointer = h2_pointer->sibling;
            }
        }
        else{
            if (new_heap->head == NULL){
                new_heap->head = h1_pointer;
                new_pointer = new_heap->head;
                h1_pointer = h1_pointer->sibling;
            }else{
                new_pointer->sibling = h1_pointer;
                new_pointer = new_pointer->sibling;
                h1_pointer = h1_pointer->sibling;
            }
        }
    }
    return new_heap;
}
void Link_heap(HeapNode *node_child, HeapNode *node_parent){
    node_child->parent = node_parent;
    node_child->sibling = node_parent->left_child;
    node_parent->left_child = node_child;
    node_parent->degree+=1;
    
}
// union two heap in a single heap, merge 2 heap first and check is it's necessary to union (same degree), time complexity of this function is O(logn)
BinomialHeap *UnionTwoHeap(BinomialHeap *h1, BinomialHeap *h2){
    BinomialHeap *merged_heap = mergeTwoHeap(h1,h2);
    //    printf("%d\n",merged_heap->size);
    HeapNode *merged_head = merged_heap->head;
    if (merged_head == NULL){
        return NULL;
    }
    HeapNode* prev_node = NULL;
    HeapNode* current_node = merged_head;
    HeapNode* next_node = current_node->sibling;
    while (next_node != NULL){
        if (current_node->degree != next_node->degree ||
            ((next_node->sibling != NULL) &&
             ((current_node->degree == next_node->degree) && (next_node->degree ==next_node->sibling->degree))))
        {
            prev_node = current_node;
            current_node = next_node;
        }
        else if(current_node->key <= next_node->key){
            //            current_node ->sibling = next_node->sibling;
            //            Link_heap(next_node,current_node);
            if (current_node->key < next_node->key){
                current_node ->sibling = next_node->sibling;
                Link_heap(next_node,current_node);
            }
            else{
                if (current_node->key == current_node->Dline && next_node->key == next_node->Dline){
                    if (current_node->Rtime <= next_node->Rtime){
                        current_node ->sibling = next_node->sibling;
                        Link_heap(next_node,current_node);
                    }
                    else{
                        if (prev_node == NULL){
                            merged_heap->head = next_node;
                        }
                        else{
                            prev_node ->sibling = next_node;
                        }
                        Link_heap(current_node, next_node);
                        current_node = next_node;
                    }
                }
                else{
                    if (current_node->Dline <=next_node->Dline){
                        current_node ->sibling = next_node->sibling;
                        Link_heap(next_node,current_node);
                    }
                    else{
                        if (prev_node == NULL){
                            merged_heap->head = next_node;
                        }
                        else{
                            prev_node ->sibling = next_node;
                        }
                        Link_heap(current_node, next_node);
                        current_node = next_node;
                    }
                    
                }
            }
        }
        else{
            
            if (prev_node == NULL){
                merged_heap->head = next_node;
            }
            else{
                prev_node ->sibling = next_node;
            }
            Link_heap(current_node, next_node);
            current_node = next_node;
        }
        next_node = current_node ->sibling;
    }
    return merged_heap;
}
BinomialHeap *delete_node(BinomialHeap *T, HeapNode *node_will_delete);

// put the time complexity analysis for Insert() here
// this function based on union function, create a new heapnode as head of a new heap ,and merge 2 heaps, time complexity is O(logn)
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline
    // You don't need to check if this task already exists in T
    //put your code here
    BinomialHeap *new_heap = newHeap();
    HeapNode *newhead = newHeapNode(k, n, c,  r, d);
    new_heap->head=newhead;
    BinomialHeap *Union_return=UnionTwoHeap(T,new_heap);
    T->head = Union_return->head;
    T->size+=1;
    
}

// this function search only head node and sibling of heap node, time complexity is O(logn)
HeapNode * get_Min_node(BinomialHeap *T) {
    
    HeapNode *current_node = T->head;
    HeapNode *returnNode = current_node;
    while (current_node){
        if (current_node->key <= returnNode->key){
            if (current_node->key < returnNode->key){
                returnNode = current_node;
            }
            else if (current_node->key == returnNode->key){
                if (current_node->key ==current_node->Dline && returnNode->key == returnNode->Dline){
                    if (current_node->Rtime <= returnNode->Rtime){
                        returnNode = current_node;
                    }
                }
                else{
                    if (current_node->Dline <= returnNode->Dline){
                        returnNode = current_node;
                    }
                }
            }
        }
        current_node = current_node->sibling;
    }
    return returnNode;
}

// put your time complexity for RemoveMin() here
//remove min function check only head node siblings of head, and delete function(time complexity is O(logn),so the time complexity of this function is O(logn))
HeapNode *RemoveMin(BinomialHeap *T)
{
    HeapNode *min_node =get_Min_node(T);
    T->head = delete_node(T, min_node)->head;
    return min_node;
}

// this function return the minimum value of heap, check only head and siblings of head.
int Min(BinomialHeap *T)
{
    HeapNode *current_pointer = T->head;
    int min_= INT_MAX;
    while (current_pointer){
        if(current_pointer->key < min_){
            min_ =current_pointer->key;
            current_pointer = current_pointer->sibling;
        }
        else{
            current_pointer = current_pointer->sibling;
        }
    }
    // put your code here
    return min_;
}

// recursive traverse heap and find a node, time complexity is O(n), this function is not used in this assignment.
HeapNode * search_node(HeapNode *head,int name){
    HeapNode *current_node = head;
    HeapNode *child;
    while (current_node !=NULL){
        if (current_node->TaskName == name){
            return current_node;
        }
        else{
            if ((child = search_node(current_node->left_child, name))!=NULL){
                return child;
            }
            current_node = current_node->sibling;
        }
    }
    return NULL;
}

// based on a given node, delete this node, time complexity is O(logn), exchange the value of this node and parent of this node untill the value of head of heap is equal to this node, and use removemin to delete this node.
BinomialHeap *delete_node(BinomialHeap *T, HeapNode *node_will_delete){
    if (T==NULL){
        return NULL;
    }
    
    int temp_d,temp_r,temp_n,temp_k,temp_e;
    //    HeapNode *node_will_delete = search_node(T->head,name);
    if (node_will_delete == NULL){
        return NULL;
    }
    int temp_size = T->size;
    HeapNode *current_node = node_will_delete;
    while (current_node->parent){
        temp_d = current_node->Dline;
        temp_e = current_node->Etime;
        temp_k = current_node->key;
        temp_n = current_node->TaskName;
        temp_r = current_node->Rtime;
        current_node->key = current_node->parent->key;
        current_node->Dline = current_node->parent->Dline;
        current_node->Etime = current_node->parent->Etime;
        current_node->TaskName = current_node->parent->TaskName;
        current_node->Rtime = current_node->parent->Rtime;
        current_node->parent->key = temp_k;
        current_node->parent->Dline = temp_d;
        current_node->parent->Etime = temp_e;
        current_node->parent->TaskName = temp_n;
        current_node->parent->Rtime = temp_r;
        current_node = current_node->parent;
    }
    BinomialHeap *next_heap = newHeap();
    BinomialHeap *prev_heap = newHeap();
    BinomialHeap *mid_heap = newHeap();
    BinomialHeap *unioned_first = newHeap();
    BinomialHeap *unioned_second = newHeap();
    next_heap ->head = current_node->sibling;
    current_node ->sibling = NULL;
    prev_heap ->head = T->head;
    HeapNode *previous_node = T->head;
    if (previous_node == current_node){
        prev_heap->head = NULL;
    }
    else{
        while (previous_node ->sibling != current_node){
            previous_node = previous_node->sibling;
        }
        previous_node ->sibling = NULL;
    }
    HeapNode *current_child = current_node ->left_child;
    
    if (current_child == NULL){
        mid_heap ->head = NULL;
    }
    else{
        current_child->parent=NULL;
        HeapNode *node_first = current_child;
        HeapNode *node_second = node_first->sibling;
        HeapNode *node_sibling;
        node_first->sibling = NULL;
        while (node_second){
            node_sibling = node_second->sibling;
            node_second->sibling = node_first;
            node_first = node_second;
            node_second->parent = NULL;
            node_second = node_sibling;
        }
        mid_heap ->head = node_first;
    }
    
    if (prev_heap->head !=NULL && mid_heap->head !=NULL){
        unioned_first = UnionTwoHeap(prev_heap, mid_heap);
    }
    else{
        if (prev_heap->head == NULL && mid_heap->head !=NULL){
            unioned_first->head =mid_heap->head;
        }
        else if (prev_heap->head != NULL && mid_heap->head ==NULL){
            unioned_first->head =prev_heap->head;
        }
        else{
            unioned_first->head = NULL;
        }
    }
    
    if (next_heap->head !=NULL){
        if (unioned_first == NULL){
            unioned_second->head = next_heap->head;
        }
        else{
            unioned_second = UnionTwoHeap (unioned_first, next_heap);
        }
    }
    else{
        if (unioned_first == NULL){
            unioned_second->head = next_heap->head;
        }
        else{
            unioned_second->head = unioned_first->head;
        }
    }
    unioned_second->size = temp_size-1;
    T->head = unioned_second->head;
    T->size = temp_size-1;
    return unioned_second;
}
// a useless function, don't mention it.
int get_min_index(int *list,int len){
    int min=INT_MAX;
    int index;
    for (int i=0;i<len;i++){
        if(list[i]==0){
            return i;
        }
        if (list[i] < min){
            min = list[i];
            index=i;
        }
    }
    return index;
}
// put your time complexity analysis for MyTaskScheduler here
// this function first use insert function(which time complexity is O(logn)) creating a binomial heap (release time as key), and then use three binomial tree to schedule tasks, the key of three heap is release time, deadline and scheduling point, every time get a node with smallest release time and find all the ready task, and find a free core, these operation based on heap, so time complexity no more than O(nlogn), so over all time complexity of this function is O(nlogn).
int TaskScheduler(char *f1, char *f2, int m )
{
    FILE *fp;
    BinomialHeap *release_heap = newHeap();
    
    int task_name,exe_time,release_time,deadline;
    if ((fp=fopen(f1,"r"))==NULL){
        printf("%s does not exist\n",f1);
        exit(0);
    }
    while (1){
        int flag=fscanf(fp, " %d %d %d %d ",&task_name,&exe_time,&release_time,&deadline);
        if (flag == EOF){
            break;
        }
        else if (flag != 4){
            printf("input error when reading the attribute of the task %d\n",task_name);
            //            printf("%d,%d,%d,%d\n",task_name,exe_time,release_time,deadline);
            break;
        }
        //        printf("%d,%d,%d,%d\n",task_name,exe_time,release_time,deadline);
        //        printf("%d %d %d %d %d\n",release_time,task_name,exe_time,release_time,deadline);
        HeapNode *node_release = newHeapNode(release_time, task_name, exe_time,  release_time, deadline);
        if (release_heap->head == NULL){
            release_heap->head = node_release;
            release_heap->size+=1;
            node_release->degree = 0;
        }
        else{
            Insert(release_heap, release_time, task_name, exe_time,  release_time, deadline );
        }
    }
    //    BinomialHeap *arrange_list =  newHeap();
    //    int core_free_list[m];
    //    memset(core_free_list, 0, sizeof(core_free_list));
    FILE *fpWrite=fopen(f2,"w");
    BinomialHeap *deadline_heap = newHeap();
    BinomialHeap *core_heap = newHeap();
    for (int i=1;i<=m;i++){
        HeapNode *core_node = newHeapNode(0, i, -1,  0, i);
        if(core_heap == NULL){
            core_heap->head = core_node;
        }
        else{
            Insert(core_heap, 0, i, -1, 0, i);
        }
    }
    int miss_flag=0;
    int release,dline,name,etime;
    while (release_heap->head!= NULL || deadline_heap->head!=NULL){
        while (Min(release_heap) <= Min(core_heap)){
            HeapNode *release_out_node=RemoveMin(release_heap);
            int release=release_out_node->Rtime;
            int dline = release_out_node->Dline;
            int name = release_out_node->TaskName;
            int etime = release_out_node->Etime;
            HeapNode *new_dline_node = newHeapNode(dline, name, etime, release, dline);
            if (deadline_heap->head ==NULL){
                deadline_heap->head = new_dline_node;
            }
            else{
                Insert(deadline_heap, dline,  name, etime, release, dline);
            }
        }
        if (deadline_heap->head != NULL){
            HeapNode *sche_task=RemoveMin(deadline_heap);
            
            HeapNode *apply_core = RemoveMin(core_heap);
            int keep_value = apply_core->Rtime;
            apply_core->key += sche_task->Etime;
            apply_core->Rtime += sche_task->Etime;
            //            printf("task %d sheduled on core %d,start time %d\n",sche_task->TaskName,apply_core->TaskName,keep_value);
            fprintf(fpWrite, "%d Core%d %d ", sche_task->TaskName,apply_core->TaskName,keep_value);
            
            if (apply_core->key > sche_task->Dline){
                //                printf("task %d missed deadline!\n",sche_task->TaskName);
                fprintf(fpWrite, "...\n\nwhere task %d misses its deadline.", sche_task->TaskName);
                miss_flag=1;
                return 0;
            }
            Insert(core_heap, apply_core->key, apply_core->TaskName, apply_core->Etime, apply_core->Rtime, apply_core->Dline);
        }else{
            HeapNode *out_core = RemoveMin(core_heap);
            Insert(core_heap, Min(release_heap), out_core->TaskName, out_core->Etime, Min(release_heap), out_core->Dline);
            //            printf("now core %d start time is %d",out_core->TaskName,Min(release_heap));
        }
        
    }
    fclose(fpWrite);
    // put your code here
    return 1;
}

int main() //sample main for testing
{ int i;
    i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
    if (i==0) printf("No feasible schedule!\n");
    //     There is a feasible schedule on 4 cores
    i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
    if (i==0) printf("No feasible schedule!\n");
    /* There is no feasible schedule on 3 cores */
    i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
    if (i==0) printf("No feasible schedule!\n");
    /* There is a feasible schedule on 5 cores */
    i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
    if (i==0) printf("No feasible schedule!\n");
    /* There is no feasible schedule on 4 cores */
    i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
    if (i==0) printf("No feasible schedule!\n");
    /* There is no feasible schedule on 2 cores */
    i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
    if (i==0) printf("No feasible schedule!\n");
    //     There is a feasible schedule on 2 cores
    return 0;
}
