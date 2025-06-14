/*
 usa sua classe para comprimir arquivos. Seu programa deverá receber tres argumentos. O primeiro deverá ser 
 “c” (ou “comprimir”) ou “d” (ou “descomprimir”). 
*/

#include "Huffman.h"
#include <fstream>
#include <cstring>


int main(int argc, char *argv[]){

    if(strcmp(argv[1],"c") ==0){         //se iremos comprimir o arquivo 
        /*Se for “c”, ele deverá ler um arquivo (o caminho será o segundo argumento) e gravar a 
        versão comprimida dele em outro arquivo (o caminho será o terceiro argumento). */
        std::ifstream in(argv[2]);
        std::ofstream out(argv[3],ios::binary); 
        if (!in.is_open()) {
            cerr << "Erro: não conseguiu abrir o arquivo de entrada: " << argv[2] << endl;
            return 1;
}
        if (!out.is_open()) {
            cerr << "Erro: não conseguiu abrir/criar o arquivo de saída: " << argv[3] << endl;
            return 1;
}

        int freqs[256]={0}; //inicializando
        MyVec<char> entrada;

        char c;
        while(in.get(c)){  //em quanto tiver caracteres no arquivo
            entrada.push_back(c);
            freqs[(unsigned char)c]++;
        }
        in.close();

        if(entrada.size() == 0) {
            cerr << "Arquivo vazio!" << endl;
            return 1;
        }
        // Escreve as frequências no arquivo de saída (em binário)
        for (int i = 0; i < 256; i++) {
            out.write(reinterpret_cast<const char*>(&freqs[i]), sizeof(int));
        }

        // Escreve o tamanho original (em binário)
        int tamanhoOriginal = entrada.size();
        out.write(reinterpret_cast<const char*>(&tamanhoOriginal), sizeof(int));

        HuffManTree arv(freqs);
        MyVec<bool> codificado;
        arv.comprimir(codificado,entrada);
        
        for(int i = 0; i < codificado.size(); i++)
            out << (codificado[i] ? '1' : '0');


    }
    else if(strcmp(argv[1],"d") ==0){     //se iremos descomprimir o arquivo
        /*Se o argumento for “d” ele deverá ler um arquivo comprimido (o caminho será o segundo 
        argumento) e gravar a versão descomprimida em outro arquivo (o caminho será o terceiro argumento).*/
        std::ifstream in(argv[2], std::ios::binary);
        std::ofstream out(argv[3]);

        if (!in.is_open()) {
            std::cerr << "Erro: não conseguiu abrir o arquivo comprimido: " << argv[2] << std::endl;
            return 1;
        }

        if (!out.is_open()) {
            std::cerr << "Erro: não conseguiu abrir/criar o arquivo de saída: " << argv[3] << std::endl;
            return 1;
        }

        // Lê as frequências
        int freqs[256];
        for (int i = 0; i < 256; ++i) {
            in.read(reinterpret_cast<char*>(&freqs[i]), sizeof(int));
        }

        // Lê o tamanho original do conteúdo descomprimido
        int tamanhoOriginal;
        in.read(reinterpret_cast<char*>(&tamanhoOriginal), sizeof(int));
        // Ignora o caractere de nova linha depois do número

        // Lê os bits comprimidos
        MyVec<bool> bits;
        char bitChar;
        while (in.get(bitChar)) {
            if (bitChar == '0')
                bits.push_back(false);
            else if (bitChar == '1')
                bits.push_back(true);
            // ignora qualquer outro caractere
        }

        in.close();

        // Reconstrói a árvore de Huffman e descomprime
        HuffManTree arv(freqs);
        MyVec<char> saida;
        arv.descomprimir(saida, bits);

        // Grava resultado
        for (int i = 0; i < saida.size(); ++i) {
            out.put(saida[i]);
        }

        out.close();
    }


    return 0;
}