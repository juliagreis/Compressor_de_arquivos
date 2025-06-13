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
        //salva as frequências
        for(int i = 0; i < 256; i++) {
            out << freqs[i];
            if(i < 255) out << " ";
        }
        out << "\n";
        // salva o tamanho original
        out << entrada.size() << "\n";

        HuffManTree arv(freqs);
        MyVec<bool> codificado;
        arv.comprimir(codificado,entrada);
        
        for(int i = 0; i < codificado.size(); i++)
            out << (codificado[i] ? '1' : '0');


    }
    else if(strcmp(argv[1],"d") ==0){     //se iremos descomprimir o arquivo
        /*Se o argumento for “d” ele deverá ler um arquivo comprimido (o caminho será o segundo 
        argumento) e gravar a versão descomprimida em outro arquivo (o caminho será o terceiro argumento).*/

    }


    return 0;
}