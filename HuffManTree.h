/*Sua classe deverá ter um construtor que recebe como argumento um array automático com a 
frequência dos 256 possíveis símbolos em um byte (“HuffManTree(int freqs[256])”). 
Ao ser construída, o construtor deverá criar uma árvore de Huffman (que será armazenada 
na parte privada de HuffManTree) usando o algoritmo descrito anteriormente 
(e obrigatoriamente usando uma fila de prioridades).
*/
template<class T>
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
        

    private:

        T *array;  //armazena o vetor recebido no construtor
        MyTree *arvore; //construtor deverá criar uma árvore de Huffman (que será armazenada 
                        //na parte privada de HuffManTree


};