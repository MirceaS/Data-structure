#include <cmath>
#include "fibheap.h"

#define LOGPHI 0.481211825

template <class T, class K>
void fibheap<T,K>::fibheapLink(fibheapNode *y, fibheapNode *x)
{
    if(minNode == y)minNode = x;
    y->left->right = y->right;
    y->right->left = y->left;
    if(x->child != 0)
    {
        x->child->right->left = y;
        y->right = x->child->right;
        x->child->right = y;
        y->left = x->child;
    }
    else
    {
        x->child = y;
    }
    y->p = x;
    x->degree++;
    y->mark = false;
}

template <class T, class K>
void fibheap<T,K>::consolidate()
{
    int siz = ceil(log(n)/LOGPHI);
    fibheapNode **A;
    A = new fibheapNode *[siz+1];
    for(int i=0;i<=siz;i++)A[i]=0;
    fibheapNode *w = minNode;
    do
    {
        fibheapNode *x = w;
        int d = x->degree;
        while(A[d] != 0)
        {
            fibheapNode *y = A[d];
            if(x->key > y->key)
            {
                fibheapNode *z = x;
                x = y;
                y = z;
            }
            fibheapLink(y,x);
            A[d] = 0;
            d++;
        }
        A[d] = x;
        w = w->right;
    }while(w!=minNode);
    minNode = 0;
    for(int i=0;i<=siz;i++)
    {
        if(A[i] != 0)
        {
            if(minNode == 0)
            {
                minNode = A[i];
                minNode->right = minNode->left = minNode;
            }
            else
            {
                minNode->right->left = A[i];
                A[i]->right = minNode->right;
                minNode->right = A[i];
                A[i]->left = minNode;
                if(A[i]->key < minNode->key)minNode = A[i];
            }
        }
    }
    delete[] A;
}

template <class T, class K>
void fibheap<T,K>::cut(fibheapNode *x)
{
    if(x->right == x)
    {
        x->p->child = 0;
    }
    else
    {
        if(x->p->child == x)x->p->child = x->right;
        x->left->right = x->right;
        x->right->left = x->left;
    }
    minNode->right->left = x;
    x->right = minNode->right;
    minNode->right = x;
    x->left = minNode;
    x->p->degree--;
    x->p = 0;
    x->mark = false;
}

template <class T, class K>
void fibheap<T,K>::cascadingCut(fibheapNode *y)
{
    fibheapNode *z = y->p;
    if(z != 0)
    {
        if(!y->mark)y->mark = true;
        else
        {
            cut(y);
            cascadingCut(z);
        }
    }
}

template<class T, class K>
fibheap<T,K>::fibheap()
{
    minNode = 0;
    n = 0;
}

template<class T, class K>
void fibheap<T,K>::cascadingDelete(fibheapNode *x)
{
    if(x != 0)
    {
        if(x->child != 0)
        {
            x->child->left->right = 0;
            cascadingDelete(x->child);
        }
        cascadingDelete(x->right);
        delete x;
    }
}

template<class T, class K>
fibheap<T,K>::~fibheap()
{
    if(minNode != 0)
    {
        minNode->left->right = 0;
        cascadingDelete(minNode);
    }
    minNode = 0;
    n = 0;
}

template<class T, class K>
int fibheap<T,K>::getSize()
{
    return n;
}

template <class T, class K>
void *fibheap<T,K>::insertNode(T data, K key)
{
    fibheapNode *nd = new fibheapNode;
    nd->data = data;
    nd->key = key;
    nd->p = 0;
    nd->child = 0;
    nd->mark = false;
    nd->degree = 0;
    if(minNode == 0)
    {
        minNode = nd;
        nd->left = nd->right = nd;
    }
    else
    {
        minNode->right->left = nd;
        nd->right = minNode->right;
        minNode->right = nd;
        nd->left = minNode;
        if(key<minNode->key)minNode = nd;
    }
    n++;
    return nd;
}

template <class T, class K>
T fibheap<T,K>::getMin()
{
    return minNode != 0 ? minNode->data : 0;
}

template <class T, class K>
T fibheap<T,K>::extractMin()
{
    fibheapNode *z = minNode;
    if(z!=0)
    {
        if(z->child != 0)
        {
            for(fibheapNode *i = z->child->right; i != z->child; i = i->right) i->p = 0;
            z->child->left->right = z->right;
            z->left->right = z->child;
            z->right->left = z->child->left;
            z->child->left = z->left;
            z->child->p = 0;
        }
        else
        {
            z->left->right = z->right;
            z->right->left = z->left;
        }

        if(z == z->right)minNode = 0;
        else
        {
            minNode = z->right;
            consolidate();
        }

        n--;
        return z->data;
    }
    return 0;
}

template <class T, class K>
void fibheap<T,K>::decreaseKey(void *x, K key)
{
    fibheapNode *nd = (fibheapNode *)x;
    if(key>nd->key)return;//error
    if(key==nd->key)return;
    nd->key = key;
    fibheapNode *y = nd->p;
    if(y!=0&&nd->key<y->key)
    {
        cut(nd);
        cascadingCut(y);
    }
    if(nd->key<minNode->key)minNode = nd;
}
