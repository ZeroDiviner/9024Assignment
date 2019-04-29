//
//  main.c
//  Assignment2
//
//  Created by Jasper on 18/3/19.
//  Copyright © 2019 Jasper. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data type for avl tree nodes
typedef struct AVLTreeNode {
    int key; //key of this item
    int  value;  //value (int) of this item
    int height; //height of the subtree rooted at this node
    struct AVLTreeNode *parent; //pointer to parent
    struct AVLTreeNode *left; //pointer to left child
    struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
    int  size;      // count of items in avl tree
    AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
    AVLTreeNode *new;
    new = malloc(sizeof(AVLTreeNode));
    assert(new != NULL);
    new->key = k;
    new->value = v;
    new->height = 0; // height of this new node is set to 0
    new->left = NULL; // this node has no child
    new->right = NULL;
    new->parent = NULL; // no parent
    return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
    AVLTree *T;
    T = malloc(sizeof (AVLTree));
    assert (T != NULL);
    T->size = 0;
    T->root = NULL;
    return T;
}
void PrintAVLTree(AVLTree *T);
AVLTreeNode *Search(AVLTree *T, int k, int v);
// To create a AVL tree, for each node ,use insert function(which time complexity is O(log(n)) ), so time complexity for
// CreateAVLTree is O(nlog(n))
AVLTree *CreateAVLTree(const char *filename)
{
    
    if (strcmp(filename, "stdin")==0){
        AVLTree *Tree=newAVLTree();
        char buffer[100];
        int key,value,insertReturn;
        char left,right,up=0,down=0;
        printf("Get data from standard input!\n");
        int offset = 0;
        int scanned = 0;
        while ( fgets ( buffer, sizeof buffer, stdin) != NULL) {
            up=0;
            down=0;
            offset = 0;
            scanned = 0;
            while(1){
                if (sscanf ( buffer + offset, "%*[^0-9|-]%i%*[^0-9|-]%i%n", &key, &value, &scanned) ==2){
                    offset += scanned;
                    //                    printf("key=%d and value=%d\n",key,value);
                    insertReturn=InsertNode(Tree,key,value);
                    up=1;
                }else{
                    if (up==1){
                        down=0;break;
                    }
                    else{
                        down=1;break;
                    }
                    
                }
            }
            if (down==1){
                break;
            }
            //            while ( sscanf ( buffer + offset, "%*[^0-9|-]%i%*[^0-9|-]%i%n", &key, &value, &scanned) ==2) {
            //                offset += scanned;
            //                printf("key=%d and value=%d",key,value);
            //                insertReturn=InsertNode(Tree,key,value);
            //            }
        }
        return Tree;
    }
    else{
        AVLTree *Tree=newAVLTree();
        FILE *fp;
        int key,value,insertReturn;
        char left,comma,right;
        if ((fp=fopen(filename,"r"))==NULL){
            printf("file cannot open \n");
            exit(0);
        }
        while ((fscanf(fp, " %c%d%c%d%c ",&left,&key,&comma,&value,&right))!=EOF){
            //            printf("%d%c%d\n",key,comma,value);
            insertReturn=InsertNode(Tree,key,value);
        }
        //        printf("%d\n",Tree->size);
        fclose(fp);
        return Tree;
    }
    
}

// CloneAVLTree use a medium order to traverse and clone every node, so time complexity is O(n)
AVLTree *CloneAVLTree(AVLTree *T)
{
    if(T->root==NULL){
        AVLTree *tree=newAVLTree();
        return tree;
    }
    AVLTree *New_tree = newAVLTree();
    AVLTree *Temp_left=newAVLTree();
    AVLTree *Temp_right= newAVLTree();
    AVLTree *New_left =newAVLTree();
    AVLTree *New_right =newAVLTree();
    Temp_left->root = T->root->left;
    Temp_right->root = T->root->right;
    AVLTreeNode *newroot=newAVLTreeNode(T->root->key,T->root->value);
    New_tree->root=newroot;
    New_tree->size=T->size;
    newroot->height=T->root->height;
    New_right=CloneAVLTree(Temp_right);
    if (New_right->root !=NULL){
        New_right->root->parent=newroot;
    }
    newroot->right=New_right->root;
    New_left=CloneAVLTree(Temp_left);
    if (New_left->root !=NULL){
        New_left->root->parent=newroot;
    }
    newroot->left=New_left->root;
    return New_tree;
}

void Traversing_add(AVLTree *T1, AVLTree *T2){
    if(T2->root==NULL){
        return;
    }
    AVLTree *Temp_left=newAVLTree();
    AVLTree *Temp_right= newAVLTree();
    Temp_left->root = T2->root->left;
    Temp_right->root = T2->root->right;
    Traversing_add(T1,Temp_left);
    InsertNode(T1, T2->root->key, T2->root->value);
    Traversing_add(T1,Temp_right);
}

// first clone a tree with larger size, which time complexity is O(n) and then medium-order traverse the other tree add node to the first tree ,total time complexity is O(n + mlog(m))
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
    //put your code here
    AVLTree *newtree;
    
    if (T2->size >=T1->size){
        newtree=CloneAVLTree(T2);
        Traversing_add(newtree,T1);
    }
    else{
        newtree=CloneAVLTree(T1);
        Traversing_add(newtree,T2);
    }
    
    return newtree;
}

// traverse another tree and add element to another tree
void Traversing_compare_add(AVLTree *New,AVLTree *T1, AVLTree *T2){
    if(T2->root==NULL){
        return;
    }
    AVLTree *Temp_left=newAVLTree();
    AVLTree *Temp_right= newAVLTree();
    Temp_left->root = T2->root->left;
    Temp_right->root = T2->root->right;
    Traversing_compare_add(New,T1,Temp_left);
    if (Search(T1, T2->root->key, T2->root->value)!=NULL){
        InsertNode(New, T2->root->key, T2->root->value);
    }
    Traversing_compare_add(New,T1,Temp_right);
}

// first traverse one of the tree with size m,use search function(which time complexity is O(logn)) to decide if the node is exist in the other tree, if exist,insert into a new tree,so overall time complexity is mlog(n)+klog(k) k is the size of new tree
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
    //put your code here
    AVLTree *NewTree=newAVLTree();
    if (T1->size >=T2->size){
        Traversing_compare_add(NewTree,T1,T2);
    }
    else{
        Traversing_compare_add(NewTree,T2,T1);
    }
    return NewTree;
};

// To insert node,first find position(log(n)) and then adjust the height(log(n)) for the inserted node and rotate(which takes consant time), so time complexity for insert is O(log(n))

int InsertNode(AVLTree *T, int k, int v)
{
    AVLTreeNode *node=newAVLTreeNode(k,v);
    if (T->root==NULL){
        T->root=node;
        T->size+=1;
    }
    else{
        AVLTreeNode* current=T->root;
        while(current!=NULL){
            //            if node->key is greater than current->key
            if (node->key > current->key){
                //                if current has no right child,then insert node into current's right child
                if (current->right ==NULL){
                    current->right=node;
                    node->parent=current;
                    T->size+=1;
                    Height_And_rotation(T,node);
                    break;
                    //                if current has right child,then replace current with right child of current
                }else{
                    current=current->right;
                }
            }
            //            if the key of node and current are equivalent,then compare value
            else if (node->key == current->key){
                if (current->value==node->value){
                    return 0;
                }
                //            if value of node is greater than the current value,then replace current with right child
                if(node->value > current->value){
                    if (current->right ==NULL){
                        current->right=node;
                        node->parent=current;
                        T->size+=1;
                        Height_And_rotation(T,node);
                        break;
                    }else{
                        current=current->right;
                    }
                    //            if value of node is less than the current value,then replace current with left child
                }else{
                    if (current->left ==NULL){
                        current->left=node;
                        node->parent=current;
                        T->size+=1;
                        Height_And_rotation(T,node);
                        break;
                    }else{
                        current=current->left;
                    }
                }
                //            if key of node is less than the current key,then replace current with left child
            }else{
                if (current->left ==NULL){
                    current->left=node;
                    node->parent=current;
                    T->size+=1;
                    Height_And_rotation(T,node);
                    break;
                }else{
                    current=current->left;
                }
            }
        }
    }
    return 1;
}
// after add a node ,ajust height and rotstion and ajust height again, takes log(n) time,where n is the number of node.
int Height_And_rotation(AVLTree *T, AVLTreeNode *new_node){
    AVLTreeNode *temp_node;
    AVLTreeNode *current=new_node;
    int i=0,flag=0,left,right,first,second,i_left,i_right,i_final;
    //    do {
    //
    //    }while(current==T->root);
    while (current!=T->root){
        i+=1;
        current=current->parent;
        if (current->height <= i){
            current->height=i;
            //            printf("here current value ==%d\n",current->key);
            if (current->left!= NULL){
                left=current->left->height;
            }
            else{
                left=-1;
            }
            if (current->right!= NULL){
                right=current->right->height;
            }
            else{
                right=-1;
            }
            if (abs(left-right)>1 && flag==0){
                flag=1;
                temp_node = current;
                first=Compare(new_node, current);
                if (first==1){
                    second=Compare(new_node, current->right);
                }
                else{
                    second=Compare(new_node,current->left);
                }
            }
        }
        else{
            break;
            //            if (current->left == NULL){
            //                i_left=-1;
            //            }
            //            else{
            //                i_left=current->left->height;
            //            }
            //            if (current->right == NULL){
            //                i_right=-1;
            //            }
            //            else{
            //                i_right=current->right->height;
            //            }
            //            i_final= (i_left > i_right ? i_left : i_right) +1;
            //            printf("current->key ===%d,and i_final == %d,left height=%d,right height = %d\n",current->key,i_final,i_left,i_right);
            //            current->height=i_final;
        }
    }
    //    printf("first == %d,second== %d,current=%d\n",first,second,new_node->key);
    if (first==0 && second==0){
        //       printf("left\n");
        Left_rotation(temp_node,T);
        height_only(temp_node,T);
    }
    else if(first==0 && second ==1){
        //        printf("left&right\n");
        Left_And_Right_rotation(temp_node,T);
        height_only(temp_node,T);
    }
    else if (first==1 && second ==0){
        //        printf("right&left\n");
        Right_And_Left_rotation(temp_node,T);
        height_only(temp_node,T);
    }
    else if (first==1 && second ==1){
        //        printf("right\n");
        Right_rotation(temp_node,T);
        height_only(temp_node,T);
    }
    return 0;
}
int height_only(AVLTreeNode * node,AVLTree *T) {
    AVLTreeNode *current=node;
    while (current!=T->root){
        current=current->parent;
        int i_left,i_right,i_final;
        if (current->left == NULL){
            i_left=-1;
        }
        else{
            i_left=current->left->height;
        }
        if (current->right == NULL){
            i_right=-1;
        }
        else{
            i_right=current->right->height;
        }
        i_final= (i_left > i_right ? i_left : i_right) +1;
        //        printf("current->key ===%d,and i_final == %d,left height=%d,right height = %d\n",current->key,i_final,i_left,i_right);
        current->height=i_final;
    }
    return 0;
}
//rotation ,takes consant time
int Right_rotation(AVLTreeNode * node,AVLTree *T){
    int node_left_height;
    int node_right_height;
    int knode_right;
    AVLTreeNode *parent = node->parent;
    AVLTreeNode *k_node=node->right;
    if (node->left == NULL){
        node_left_height=-1;
    }
    else{
        node_left_height=node->left->height;
    }
    if (k_node->left !=NULL){
        node->right=k_node->left;
        k_node->left->parent=node;
        k_node->left=node;
        node->parent=k_node;
    }
    else{
        node->right=NULL;
        k_node->left=node;
        node->parent=k_node;
    }
    if (parent!=NULL &&parent->left == node){
        parent->left=k_node;
        k_node->parent=parent;
    }
    else if (parent!=NULL && parent->right == node){
        parent->right=k_node;
        k_node->parent=parent;
    }
    else if (parent==NULL){
        T->root=k_node;
        k_node->parent=NULL;
    }
    if (node->right==NULL){
        node_right_height = -1;
    }
    else{
        node_right_height = node->right->height;
    }
    node->height = (node_left_height > node_right_height ? node_left_height : node_right_height)+1;
    if (k_node->right == NULL){
        knode_right=-1;
    }
    else{
        knode_right=k_node->right->height;
    }
    k_node->height = (knode_right > node->height ? knode_right : node->height)+1;
    return 0;
}

int Left_rotation(AVLTreeNode * node,AVLTree *T){
    AVLTreeNode *parent = node->parent;
    AVLTreeNode *k_node=node->left;
    int node_left_height;
    int node_right_height;
    int knode_left;
    if(k_node->right !=NULL){
        node->left =k_node->right;
        k_node->right->parent=node;
        k_node->right=node;
        node->parent=k_node;
    }
    else{
        node->left=NULL;
        k_node->right=node;
        node->parent=k_node;
    }
    
    if (parent!=NULL && parent->left == node){
        parent->left=k_node;
        k_node->parent=parent;
    }
    else if (parent!=NULL && parent->right == node){
        parent->right=k_node;
        k_node->parent=parent;
    }
    else if (parent == NULL){
        T->root=k_node;
        k_node->parent=NULL;
    }
    if (node->left==NULL){
        node_left_height=-1;
    }
    else{
        node_left_height=node->left->height;
    }
    if (node->right == NULL){
        node_right_height=-1;
    }
    else{
        node_right_height=node->right->height;
    }
    node->height = (node_left_height > node_right_height ? node_left_height:node_right_height)+1;
    if (k_node->left == NULL){
        knode_left=-1;
    }
    else{
        knode_left=k_node->left->height;
    }
    k_node->height= (knode_left > node->height ? knode_left : node->height)+1;
    return 0;
}

int Left_And_Right_rotation(AVLTreeNode * node,AVLTree *T){
    Right_rotation(node->left,T);
    Left_rotation(node,T);
    return 0;
}

int Right_And_Left_rotation(AVLTreeNode * node,AVLTree *T){
    Left_rotation(node->right,T);
    Right_rotation(node,T);
    return 0;
}

int Compare(AVLTreeNode *node1,AVLTreeNode *node2){
    
    if ((node1->key>node2->key)||(node1->key==node2->key && node1->value >node2->value)){
        return 1;
    }
    else{
        return 0;
    }
}

void Node_height(AVLTreeNode* v){
    AVLTreeNode *left=v->left;
    int left_height,right_height,final;
    AVLTreeNode *right=v->right;
    if (left ==NULL){
        left_height=-1;
    }
    else{
        left_height=left->height;
    }
    if (right ==NULL){
        right_height=-1;
    }
    else{
        right_height=left->height;
    }
    final=(left_height > right_height ? left_height:right_height)+1;
    v->height=final;
}

// when delete a node ,ajust height and rotation,takes log(n) time
void Traceback_rotation(AVLTreeNode *node,int key,int value,AVLTree *T){
    int height_left,height_right;
    if (node->left!=NULL){
        height_left=node->left->height;
    }
    else{
        height_left=-1;
    }
    if (node->right!=NULL){
        height_right=node->right->height;
    }
    else{
        height_right=-1;
    }
    while (abs(height_right-height_left)<2){
        node=node->parent;
        if (node==NULL){
            break;
        }
        else{
            if (node->left!=NULL){
                height_left=node->left->height;
            }
            else{
                height_left=-1;
            }
            if (node->right!=NULL){
                height_right=node->left->height;
            }
            else{
                height_right=-1;
            }
        }
    }
    if (node==NULL){
        return;
    }
    else{
        if (key > node->key ||( key ==node->key && value > node->value)){
            AVLTreeNode *left = node->left;
            int left_height_left,left_height_right,balance;
            if (left->left !=NULL){
                left_height_left=left->left->height;
            }
            else{
                left_height_left=-1;
            }
            if (left->right !=NULL){
                left_height_right=left->right->height;
            }
            else{
                left_height_right=-1;
            }
            balance=left_height_left-left_height_right;
            if (balance == 0 || balance ==1){
                Left_rotation(node, T);
            }
            else if(balance==-1){
                Left_And_Right_rotation(left, T);
            }
        }
        //deleted a node in left subtree of node
        else{
            AVLTreeNode *right = node->right;
            int right_height_left,right_height_right,balance;
            if (right->left !=NULL){
                right_height_left=right->left->height;
            }
            else{
                right_height_left=-1;
            }
            if (right->right !=NULL){
                right_height_right=right->right->height;
            }
            else{
                right_height_right=-1;
            }
            balance=right_height_left-right_height_right;
            if (balance == 0 || balance ==1){
                Right_rotation(node, T);
            }
            else if (balance==-1){
                Right_And_Left_rotation(right, T);
            }
        }
    }
    //deleted a node in right subtree of node
    
}

int deleteflag=0;
AVLTree *delete_tree;
int size_change_flag=0;
// to delete a node is to find a node recursively(O(logn)) and remove this node according to subtree condition of this node(then rotate, cost consant time) and ajust height(O(logn)),so the overall time complexity is O(logn)
int DeleteNode(AVLTree *T, int k, int v)
{   int flag_right=0,flag_left=0;
    if (deleteflag==0){
        deleteflag+=1;
        //        extern AVLTree *delete_tree;
        delete_tree=T;
    }
    // put your code here
    if(T->root==NULL){
        return 0;
    }
    AVLTree *Temp_left=newAVLTree();
    AVLTree *Temp_right= newAVLTree();
    Temp_left->root = T->root->left;
    Temp_right->root = T->root->right;
    if (k> T->root->key || (k==T->root->key&&v>T->root->value)){
        flag_right=DeleteNode(Temp_right,k,v);
    }
    else if(k<T->root->key ||(k==T->root->key&&v<T->root->value)){
        flag_left=DeleteNode(Temp_left,k,v);
    }
    else if (k==T->root->key && v==T->root->value){
        // delete a leaf node
        if (T->root->left ==NULL && T->root->right ==NULL){
            AVLTreeNode *parent=T->root->parent;
            // it is  root node of Tree
            if (parent == NULL){
                T->root=NULL;
                T->size=0;
                free(T->root);
                delete_tree->size-=1;
                return 1;
            }else{
                if (parent->left == T->root){
                    parent->left=NULL;
                    Node_height(parent);
                    Traceback_rotation(parent,T->root->key,T->root->value,delete_tree);
                    free(T->root);
                    delete_tree->size-=1;
                    return 1;
                }
                else if (parent->right == T->root){
                    parent->right=NULL;
                    Node_height(parent);
                    Traceback_rotation(parent,T->root->key,T->root->value,delete_tree);
                    free(T->root);
                    delete_tree->size-=1;
                    return 1;
                }
            }
        }
        // delete a node with only right subtree
        else if (T->root->left ==NULL && T->root->right !=NULL){
            AVLTreeNode *parent=T->root->parent;
            // it is  root node of Tree
            if (parent == NULL){
                T->root->right->parent=NULL;
                AVLTreeNode *right=T->root->right;
                free(T->root);
                T->root=right;
                Node_height(T->root);
                delete_tree->size-=1;
                return 1;
            }
            else {
                AVLTreeNode *right=T->root->right;
                if (parent->right == T->root){
                    parent->right=right;
                    right->parent=parent;
                    Node_height(parent);
                    Node_height(right);
                    Traceback_rotation(parent,T->root->key,T->root->value,delete_tree);
                    free(T->root);
                    delete_tree->size-=1;
                    return 1;
                }
                else{
                    parent->left=right;
                    right->parent=parent;
                    Node_height(parent);
                    Traceback_rotation(parent,T->root->key,T->root->value,delete_tree);
                    Node_height(right);
                    free(T->root);
                    delete_tree->size-=1;
                    return 1;
                }
            }
        }
        // delete a node with only left subtree
        else if (T->root->right ==NULL && T->root->left !=NULL){
            AVLTreeNode *parent=T->root->parent;
            if (parent == NULL){
                T->root->left->parent=NULL;
                AVLTreeNode *left=T->root->left;
                free(T->root);
                T->root=left;
                Node_height(T->root);
                delete_tree->size-=1;
                return 1;
            }
            else {
                AVLTreeNode *left=T->root->left;
                if (parent->left == T->root){
                    parent->right=left;
                    left->parent=parent;
                    Node_height(parent);
                    Traceback_rotation(parent,T->root->key,T->root->value,delete_tree);
                    Node_height(left);
                    free(T->root);
                    delete_tree->size-=1;
                    return 1;
                }
                else{
                    parent->right=left;
                    left->parent=parent;
                    Node_height(parent);
                    Traceback_rotation(parent,T->root->key,T->root->value,delete_tree);
                    Node_height(left);
                    free(T->root);
                    delete_tree->size-=1;
                    return 1;
                }
            }
        }
        // delete a node with both left and right subtree
        else{
            AVLTreeNode* left_tree=T->root->left;
            AVLTreeNode* right_tree=T->root->right;
            
            if (left_tree->height>right_tree->height){
                AVLTreeNode* left_max;
                left_max=left_tree;
                while(left_max->right!=NULL){
                    left_max=left_max->right;
                }
                T->root->key=left_max->key;
                T->root->value=left_max->value;
                AVLTree *Tree_left=newAVLTree();
                Tree_left->root=left_tree;
                DeleteNode(Tree_left, T->root->key, T->root->value);
                delete_tree->size-=1;
                return 1;
            }
            else{
                AVLTreeNode* right_max;
                right_max = right_tree;
                while (right_tree ->left!=NULL){
                    right_max=right_max->left;
                }
                T->root->key=right_max->key;
                T->root->value=right_max->value;
                AVLTree *Tree_right=newAVLTree();
                Tree_right->root=right_tree;
                DeleteNode(Tree_right, T->root->key, T->root->value);
                delete_tree->size-=1;
                return 1;
            }
        }
    }
    if  ((flag_left||flag_right)==1){
        return 1;
    }
    else{
        return 0;
    }
    //    deleteflag=0;
}


// To search a node , the time complexity is O(logn), since for every node we need only choose a direction and then search
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
    // put your code here
    AVLTreeNode* current=T->root;
    while(current!=NULL){
        if(k> current->key){
            current=current->right;
        }
        else if(k == current->key){
            if(v==current->value){
                return current;
            }
            else{
                if (v>current->value){
                    current=current->right;
                }else{
                    current=current->left;
                }
            }
        }
        else{
            current=current->left;
        }
    }
    return NULL;
}


void FreeNode(AVLTreeNode *root){
    if (root == NULL){
        return;
    }
    AVLTreeNode *left = root->left;
    AVLTreeNode *right = root->right;
    FreeNode(left);
    FreeNode(right);
    free(root);
}

// To free a AVL tree ,i use post-order traversing to free every node in tree and then free tree,the time complexity for post-order traversing is O(n)
void FreeAVLTree(AVLTree *T)
{
    FreeNode(T->root);
    free(T);
}



// To print a AVL tree in order,i use medium-order traversing to print every node in tree ,the time complexity for medium-order traversing is O(n)
void PrintAVLTree(AVLTree *T)
{
    if(T->root==NULL){
        return;
    }
    AVLTree *Temp_left=newAVLTree();
    AVLTree *Temp_right= newAVLTree();
    Temp_left->root = T->root->left;
    Temp_right->root = T->root->right;
    PrintAVLTree(Temp_left);
    printf("(%d,%d),%d\n",T->root->key,T->root->value,T->root->height);
    PrintAVLTree(Temp_right);
}

int main() //sample main for testing
{ int i,j;
    AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
    AVLTreeNode *node1;
    
    tree1=CreateAVLTree("stdin");
    PrintAVLTree(tree1);
    FreeAVLTree(tree1);
    //you need to create the text file file1.txt
    // to store a set of items without duplicate items
    tree2=CreateAVLTree("file1.txt");
    PrintAVLTree(tree2);
    tree3=CloneAVLTree(tree2);
    PrintAVLTree(tree3);
    FreeAVLTree(tree2);
    FreeAVLTree(tree3);
    //Create tree4
    tree4=newAVLTree();
    j=InsertNode(tree4, 10, 10);
    for (i=0; i<15; i++)
    {
        j=InsertNode(tree4, i, i);
        if (j==0) printf("(%d, %d) already exists\n", i, i);
    }
    PrintAVLTree(tree4);
    node1=Search(tree4,20,20);
    if (node1!=NULL)
        printf("key= %d value= %d\n",node1->key,node1->value);
    else
        printf("Key 20 does not exist\n");
    
    for (i=17; i>0; i--)
    {
        j=DeleteNode(tree4, i, i);
        if (j==0)
            printf("Key %d does not exist\n",i);
        PrintAVLTree(tree4);
    }
    FreeAVLTree(tree4);
    //Create tree5
    tree5=newAVLTree();
    j=InsertNode(tree5, 6, 25);
    j=InsertNode(tree5, 6, 10);
    j=InsertNode(tree5, 6, 12);
    j=InsertNode(tree5, 6, 20);
    j=InsertNode(tree5, 9, 25);
    j=InsertNode(tree5, 10, 25);
    PrintAVLTree(tree5);
    //Create tree6
    tree6=newAVLTree();
    j=InsertNode(tree6, 6, 25);
    j=InsertNode(tree6, 5, 10);
    j=InsertNode(tree6, 6, 12);
    j=InsertNode(tree6, 6, 20);
    j=InsertNode(tree6, 8, 35);
    j=InsertNode(tree6, 10, 25);
    PrintAVLTree(tree6);
    tree7=AVLTreesIntersection(tree5, tree6);
    tree8=AVLTreesUnion(tree5,tree6);
    //    printf("-------\n");
    PrintAVLTree(tree7);
    //    printf("-------\n");
    PrintAVLTree(tree8);
    return 0;
}

