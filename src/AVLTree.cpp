#include <iostream>
#include <queue>
#include <vector>
#include <stack> 

template <class T>
struct Node
{
    Node(T v)
    {
        value = v;
        nodes[0] = nodes[1] = 0;
    }
    T value;
    Node<T>* nodes[2]; //0 left, 1 right
};

template <class T>
class CBinTree
{
public:
    CBinTree();
    ~CBinTree();
    Node<T>* Root();
    bool Find(T x, Node<T>**& p);
    bool Ins(T x);
    bool Rem(T x);

    void RR(Node<T>**& p);
    void LL(Node<T>**& p);
    void AVL(Node<T>**& p);

    int MaxDepth(Node<T>* n);
    void Step(std::stack<Node<T>*>& n, std::stack<T>& s);

    Node<T>** Rep(Node<T>** p);


    void PrintWidth();


private:
    int espaciosEntre(std::vector<T> numeros, T a, T b);
    Node<T>* root;
    bool brep;
    std::stack<Node<T>**> recorrido;
};

template <class T>
CBinTree<T>::CBinTree()
{
    root = 0;
    brep = 0;
}

template <class T>
CBinTree<T>::~CBinTree()
{
    //...
}

template <class T>
Node<T>* CBinTree<T>::Root()
{
    return root;
}

template <class T>
bool CBinTree<T>::Find(T x, Node<T>**& p)
{
    p = &root;
    while (*p && (*p)->value != x)
    {
        recorrido.push(p);
        if ((*p)->value < x)
        {
            p = &((*p)->nodes[1]);
        }
        else
        {
            p = &((*p)->nodes[0]);
        }

    }
    return *p != 0;
}

template <class T>
bool CBinTree<T>::Ins(T x)
{
    Node<T>** p;
    if (Find(x, p)) return 0;
    *p = new Node<T>(x);

    PrintWidth();
    for (; !recorrido.empty(); recorrido.pop())
    {
        AVL(recorrido.top());
    }

    return 1;
}

template <class T>
bool CBinTree<T>::Rem(T x)
{
    Node<T>** p;
    if (!Find(x, p)) return 0;

    if ((*p)->nodes[0] && (*p)->nodes[1])
    {
        Node<T>** q = Rep(p);
        (*p)->value = (*q)->value;
        p = q;
    }

    Node<T>* t = *p;
    *p = (*p)->nodes[(*p)->nodes[1] != 0];
    delete t;

    PrintWidth();
    for (; !recorrido.empty(); recorrido.pop())
    {
        AVL(recorrido.top());
    }
    return 1;
}

template <class T>
void CBinTree<T>::RR(Node<T>**& p)
{
    Node<T>* t1, * t2, * t3;
    t1 = *p; t2 = (*p)->nodes[1]; t3 = ((*p)->nodes[1])->nodes[0];

    *p = t2;
    t1->nodes[1] = t3;
    t2->nodes[0] = t1;
}

template <class T>
void CBinTree<T>::LL(Node<T>**& p)
{
    Node<T>* t1, * t2, * t3;
    t1 = *p; t2 = (*p)->nodes[0]; t3 = ((*p)->nodes[0])->nodes[1];

    *p = t2;
    t1->nodes[0] = t3;
    t2->nodes[1] = t1;

}

template <class T>
void CBinTree<T>::AVL(Node<T>**& p)
{
    int izqH = MaxDepth((*p)->nodes[0]), derH = MaxDepth((*p)->nodes[1]);
    int balance = derH - izqH;

    if (balance != -2 && balance != 2) return;
    std::cout << "\n\nAVL en: " << (*p)->value << "\n" << std::endl;
    //std::cout << (*p)->value << std::endl;
    //std::cout << balance << std::endl;

    if (balance == -2)
    {
        Node<T>** izq = &((*p)->nodes[0]);
        int balanceIzq = MaxDepth((*izq)->nodes[1]) - MaxDepth((*izq)->nodes[0]);
        if (balanceIzq == 1)
        {
            std::cout << "RL: " << std::endl;
            RR(izq);
            LL(p);
        }
        else if (balanceIzq == -1)
        {
            std::cout << "LL: " << std::endl;
            LL(p);
        }
    }
    else if (balance == 2)
    {
        Node<T>** der = &((*p)->nodes[1]);
        int balanceDer = MaxDepth((*der)->nodes[1]) - MaxDepth((*der)->nodes[0]);
        if (balanceDer == -1)
        {
            std::cout << "LR: " << std::endl;
            LL(der);
            RR(p);
        }
        else if (balanceDer == 1)
        {
            std::cout << "RR: " << std::endl;
            RR(p);
        }
    }
    PrintWidth();
}

template <class T>
Node<T>** CBinTree<T>::Rep(Node<T>** p)
{
    Node<T>** q;
    for (q = &(*p)->nodes[!brep]; (*q)->nodes[brep]; q = &(*q)->nodes[brep]);
    brep = !brep;
    return q;
}

template <class T>
void CBinTree<T>::Step(std::stack<Node<T>*>& n, std::stack<T>& s)
{
    while (true) {
        while (!(n.top()) || s.top() == 3) {
            n.pop();
            s.pop();
            if (n.empty()) return;
        }

        switch (s.top()) {
        case 0:
            n.push((n.top())->nodes[0]);
            s.top()++;
            s.push(0);
            break;
        case 1:
            s.top()++;
            return;
        case 2:
            n.push((n.top())->nodes[1]);
            s.top()++;
            s.push(0);
            break;
        }
    }
}

template <class T>
int CBinTree<T>::espaciosEntre(std::vector<T> numeros, T a, T b)
{
    int espacios = 0;


    typename std::vector<T>::iterator it = numeros.begin();

    while (*it != a)
    {
        it++;
    }

    while (*it != b)
    {
        espacios++;
        it++;
    }
    return espacios;
}

template <class T>
void CBinTree<T>::PrintWidth()
{
    if (!root) return;

    std::vector<T> numerosinorder;

    std::stack<Node<T>*> SNodes;
    std::stack<int> SStates;

    SNodes.push(root);
    SStates.push(0);

    Step(SNodes, SStates);

    while (!(SNodes.empty())) {
        numerosinorder.push_back((SNodes.top())->value);
        Step(SNodes, SStates);
    }

    std::queue<Node<T>*> q;
    int antes;
    q.push(root);
    int espacio = 0;
    int espaciodondevengo;
    for (int level = 0; !q.empty(); level++)
    {
        int size = q.size();
        for (int i = 0; i < size; i++)
        {
            Node<T>* n = q.front();
            q.pop();
            if (i == 0)
            {
                espacio = espaciosEntre(numerosinorder, *(numerosinorder.begin()), n->value);
            }
            else
            {
                espacio = espaciosEntre(numerosinorder, antes, n->value);
                espacio -= 1;
            }
            espaciodondevengo = espacio;
            while (espacio) { std::cout << "    "; espacio--; }//cambiar de " " a dos espacios "  " si hay muchos numeros de dos digitos
            std::cout << n->value;//<< "(" << level << ") ";
            antes = n->value;
            if (n->nodes[0])
            {
                q.push(n->nodes[0]);
            }
            if (n->nodes[1])
            {
                q.push(n->nodes[1]);
            }
        }
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }
    std::cout << "\n\n";
}

template <class T>
int CBinTree<T>::MaxDepth(Node<T>* n)
{
    if (!n) return 0;
    return std::max(MaxDepth(n->nodes[0]), MaxDepth(n->nodes[1])) + 1;
}

CBinTree<int> t;


void Comenzar(int n)
{
    int num;
    std::cout << "\n\nVALORES DE LOS NODOS:\n\n" << std::endl;

    for (int i = 0; i < n; i++)
    {
        std::cout << i + 1 << ") ingrese un numero: "; std::cin >> num; std::cout << "\n\n";
        t.Ins(num); //t.PrintWidth();
    }
}

void Terminar(int n)
{
    int num;
    std::cout << "\n\nVALORES DE LOS NODOS:\n\n" << std::endl;

    for (int i = 0; i < n; i++)
    {
        std::cout << i + 1 << ") ingrese un numero: "; std::cin >> num; std::cout << "\n\n";
        t.Rem(num); //t.PrintWidth();
    }
}

int main()
{
    int n;
    std::cout << "cuantos nodos quiere ingresar: ";
    std::cin >> n;

    Comenzar(n);

    std::cout << "cuantos nodos quiere eliminar: ";
    std::cin >> n;

    Terminar(n);


    return 0;
}