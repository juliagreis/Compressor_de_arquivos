/*Sua classe deverá ter um construtor que recebe como argumento um array automático com a 
frequência dos 256 possíveis símbolos em um byte (“HuffManTree(int freqs[256])”). 
Ao ser construída, o construtor deverá criar uma árvore de Huffman (que será armazenada 
na parte privada de HuffManTree) usando o algoritmo descrito anteriormente 
(e obrigatoriamente usando uma fila de prioridades).
*/
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "MyPriorityQueue.h"
#include "MyVec.h"

class Node{  //classe auxiliar - encapsulamento nao necessario
    public:
        unsigned char simbolo;
        int freq;
        Node* left;
        Node* right;

        Node(unsigned char s, int f) : simbolo(s), freq(f), left(NULL), right(NULL) {}
    
        ~Node(){left = NULL; right = NULL;}

        Node(const Node &other) : simbolo(other.simbolo), freq(other.freq), left(NULL), right(NULL) {
            if(other.left) left = new Node(*other.left);
            if(other.right) right = new Node(*other.right);
        }
};
class NodoWrapper {
public:
    Node* ptr;

    NodoWrapper() : ptr(NULL) {}           
    NodoWrapper(Node* n) : ptr(n) {}

    //Ambos os operadores estao invertidos, para que esteja ordenado em ordem CRESCENTE 
    bool operator<(const NodoWrapper& other) const {
        if (!ptr) return true;          // considera null menor
        if (!other.ptr) return false;
        return ptr->freq > other.ptr->freq;
    }

    bool operator>(const NodoWrapper& other) const {
        if (!ptr) return false;
        if (!other.ptr) return true;
        return ptr->freq < other.ptr->freq;
    }
};

class HuffManTree{


    public:
        HuffManTree(int freqs[256]);    //construtor
        HuffManTree(const HuffManTree& other);    //construtor de copia
        ~HuffManTree();   //destrutor

        void comprimir(MyVec<bool> &out, const MyVec<char> &in) const; //ler o vetor de bytes (chars) “in”, comprimi-lo e 
        //gravar os bits representando o arquivo comprimido em “out” (cada bool de out representará um bit , 
        //sendo 1 representado por true e 0 por false)

        void descomprimir(MyVec<char> &out, const MyVec<bool> &in) const;//Essa função deverá fazer o 
        //inverso do que a função acima faz

        HuffManTree& operator=(const HuffManTree& other);
        

    private:

        int frequencia[256];  //armazena o vetor recebido no construtor
        Node * root; //construtor deverá criar uma árvore de Huffman (que será armazenada 
                        //na parte privada de HuffManTree


        //funcoes auxiliares
        Node* criaArvore();
        void destroyArvore();
        void destroyArvore(Node* root);

        void gerarCodigo(Node * root ,MyVec<bool> &caminho, MyVec<bool> codigo[256])const;
        Node* copiaArvore(Node* no);


};
#endif