#include "Huffman.h"

Node::Node(const Node &other) : simbolo(other.simbolo), freq(other.freq), left(NULL), right(NULL) {
    if(other.left) left = new Node(*other.left);
    if(other.right) right = new Node(*other.right);
}

//os operadores estao invertidos para que esteja ordenado em ordem CRESCENTE
bool NodoWrapper::operator<(const NodoWrapper& other) const {
    if (!ptr) return true;          // considera null menor
    if (!other.ptr) return false;
    return ptr->freq > other.ptr->freq;
}

bool NodoWrapper::operator>(const NodoWrapper& other) const {
    if (!ptr) return false;
    if (!other.ptr) return true;
    return ptr->freq < other.ptr->freq;
}



//------------FUNCOES AUXILIARES---------------//

//cria arvore de codificacao dos caracters do arquivo
//com base na frwuencia de cada um deles no array 

Node* HuffManTree::criaArvore(){
    /*
            Ex: A: 1  B: 3  C: 5
            Passos:
            1)Fila inicial (ordenada por frequência crescente): A(1), B(3), C(5)
            2)Retire A(1) e B(3)
            3)Crie novo nodo com freq = 4 (A+B)
            4)Insira de volta: C(5), AB(4) → reordenar → AB(4), C(5)
            5)Retire AB(4) e C(5)
            6)Crie novo nodo com freq = 9 → esse será a raiz
*/
MyPriorityQueue<NodoWrapper> fila;
    //guarda todos os nodos folha na fila
    for (int i = 0; i < 256; i++) {
        if (frequencia[i] > 0) {
            fila.push(NodoWrapper(new Node((unsigned char)i, frequencia[i])));
        }
    }

    if (fila.size() == 0) {
        return NULL; 
    }

    //se tem só um caractere único
    if (fila.size() == 1) {
        NodoWrapper unicoWrapper = fila.top();
        fila.pop();  //fila esta limpa
        Node* unicoNode = unicoWrapper.ptr;
        
        //nó pai  para garantir que sempre há um caminho
        Node* especialRoot = new Node('\0', unicoNode->freq);
        especialRoot->left = unicoNode;
        especialRoot->right = nullptr;
        return especialRoot;
    }

    while(fila.size() > 1 ){  //quando o tamanho for 1, este será a raiz
        NodoWrapper firstWrapper = fila.top();
        fila.pop();
        Node* first = firstWrapper.ptr;

        NodoWrapper secondWrapper = fila.top();
        fila.pop();
        Node* second = secondWrapper.ptr;

        Node* aux=new Node('\0',first->freq + second->freq);  //nodo sem caracter
        aux->left=first;  //pois first é o menor, entao fica a esquerda
        aux->right=second;  //pois second é o maior, entao fica a direita
        fila.push(NodoWrapper(aux));  //reinserindo o nodo auxiliar
    }
   
    //sobrou um unico nodo, logo este é o maior de todos e será a raiz
    Node* raiz = fila.top().ptr;
    fila.pop();
    return raiz;
}
void HuffManTree::gerarCodigo(Node * root ,MyVec<bool> &caminho, MyVec<bool> codigo[256])const{
    if(root==NULL) return;
    //se chegamos em uma folha, registramos o codigo obtido para o caracter em questao
    if(!root->left&&!root->right){
        codigo[(unsigned char)root->simbolo]=caminho;  //esse caminho fois responsavel por obter o caracter  root->simbolo
        return;
    }
    //se nao, temos que ir pra esquerda e para a direita
    //para a esquerda
    if(root->left){
        caminho.push_back(false);  //segue pra esquerda
        gerarCodigo(root->left,caminho,codigo);  //gera o codigo da esquerda
        caminho.pop_back();  // voltamos pro node de origem
    }

    //para a direita
    if(root->right){
        caminho.push_back(true);  //segue pra direita
        gerarCodigo(root->right,caminho,codigo);  //gera o codigo da direita
        caminho.pop_back();    //// voltamos pro node de origem
    }
}
void HuffManTree::destroyArvore(){
    if(root==NULL) return;
    destroyArvore(root);
    root = NULL;
}
void HuffManTree::destroyArvore(Node* nodo){
    if(nodo==NULL){
        return;
    }
    destroyArvore(nodo->left);
    destroyArvore(nodo->right);
    delete nodo;
}

Node* HuffManTree::copiaArvore(Node* no) {
    if (!no) return NULL;
    Node* novo = new Node(no->simbolo, no->freq);
    novo->left = copiaArvore(no->left);
    novo->right = copiaArvore(no->right);
    return novo;
}


//--------------------------------------------//

HuffManTree::HuffManTree(int freqs[256]):root(NULL) {
    // copia as frequências para o atributo
    for (int i = 0; i < 256; i++) {
        frequencia[i] = freqs[i];
    }

    //cria a árvore usando a fila de NodoWrapper
    root = criaArvore();
}

HuffManTree::HuffManTree(const HuffManTree& other):root(NULL) {
    
    for (int i = 0; i < 256; ++i)
        frequencia[i] = other.frequencia[i];
    root = copiaArvore(other.root);
}
HuffManTree::~HuffManTree(){
    destroyArvore(); 
}
HuffManTree& HuffManTree::operator=(const HuffManTree& other) {
    if (this != &other) {
        //limpa a árvore atual
        destroyArvore();
        
        // copia os dados
        for (int i = 0; i < 256; ++i) {
            frequencia[i] = other.frequencia[i];
        }
        root = copiaArvore(other.root);
    }
    return *this;
}

void HuffManTree::comprimir(MyVec<bool> &out, const MyVec<char> &in) const{
    if (!root) return;

    MyVec<bool> codigo[256]; 
    for (int i = 0; i < 256; i++) {
        codigo[i].clear(); // Garante que está vazio
    }
    MyVec<bool> caminho;

    gerarCodigo(root,caminho,codigo);

    //comprime
    for(int i=0;i<in.size();i++){
        unsigned char caracter = (unsigned char) in[i];
        if (codigo[caracter].size() > 0) { // Verifica se há código para o caractere
            for(int j=0;j< codigo[caracter].size();j++){
            out.push_back(codigo[caracter][j]);
            }
        }
    }
}
   
void HuffManTree::descomprimir(MyVec<char> &out, const MyVec<bool> &in) const{
    if (!root || in.size() == 0) return;

    //se é arvore com um único nodo
    if (!root->left && !root->right) {
        for (int i = 0; i < in.size(); i++) {
            out.push_back(root->simbolo);
        }
        return;
    }

    Node* atual = root;
    for (int i = 0; i < in.size(); i++) {
        atual = in[i] ? atual->right : atual->left;

        if (!atual->left && !atual->right) {
            out.push_back(atual->simbolo);
            atual = root; // reinicia da raiz
        }
    }
}
