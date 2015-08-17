#ifndef FIBHEAP_H
#define FIBHEAP_H
template <class T, class K>
class fibheap
{
    private:
    struct fibheapNode;

    fibheapNode *minNode;
    int n;

    void fibheapLink(fibheapNode *y, fibheapNode *x);
    void consolidate();
    void cut(fibheapNode *x);
    void cascadingCut(fibheapNode *y);
    void cascadingDelete(fibheapNode *x);

    public:
    fibheap();
    ~fibheap();
    int getSize();
    void *insertNode(T data, K key);
    T getMin();
    //unite
    T extractMin();
    void decreaseKey(void *nd, K key);
    //delete
    //can't implement delete as K is a general type so I cannot know it's minimum value
};

template <class T, class K>
struct fibheap<T,K>::fibheapNode
{
    T data;
    K key;
    fibheapNode *p;
    fibheapNode *child;
    fibheapNode *left;
    fibheapNode *right;
    int degree;
    bool mark;
};
#endif
