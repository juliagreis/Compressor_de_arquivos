#include "Huffman.h"
#include <fstream>
#include <cstring>


int main(int argc, char *argv[]){
    
    //-----------------------COMPRIMIR-------------------------//
    if(strcmp(argv[1],"c") ==0){         
        /*Se for “c”, ele deverá ler um arquivo (o caminho será o segundo argumento) e gravar a 
        versão comprimida dele em outro arquivo (o caminho será o terceiro argumento). */

        //abrindo os arquivos
        std::ifstream in(argv[2]);
        std::ofstream out(argv[3],ios::binary); 

        int freqs[256]={0}; //inicializando
        MyVec<char> entrada;

        char c;
        while(in.get(c)){           //em quanto tiver caracteres no arquivo
            entrada.push_back(c);
            freqs[(unsigned char)c]++;  //aumenta a frequencia q o caracter apareceu
        }
        in.close();


        //escreve as frequências no arquivo de saída (em binário)
        for (int i = 0; i < 256; i++) {
            out.write(reinterpret_cast<const char*>(&freqs[i]), sizeof(int));
        }

        //escreve o tamanho original em binario
        int tamanhoOriginal = entrada.size();
        out.write(reinterpret_cast<const char*>(&tamanhoOriginal), sizeof(int));

        HuffManTree arv(freqs);
        MyVec<bool> codificado;
        arv.comprimir(codificado,entrada);  //comprimir arquivo
        
        //explicacao:
        //calculando quantos  bits válidos no último byte, pq se a quantidade
        //for nao multipla de 8, teremos que completar o byte. mas na hora de decodificar
        //temos que ignorar esses bits adicionados

        //salva o total de bits
        int totalBits = codificado.size();
        out.write(reinterpret_cast<const char*>(&totalBits), sizeof(int));

        //calcula quantos bytes vou precisar
        int numBytes = (codificado.size() + 7) / 8;  //usei +7 pra arredondar pra cima ex: 1/8 = 0 mas precisa de 1 byte

        for(int i = 0; i< numBytes; i++) {  //em quanto eu ainda tiver que criar novos bytes
            string strBits = "";  //reinicia toda vez que comecamos a escrever um novo byte
            
            for(int j = 0; j < 8; j++) {  //pega 8 bits e converte pra string
                int bitIndex = i * 8 + j;
                
                if(bitIndex < codificado.size() && codificado[bitIndex]) { //se for true
                    strBits += "1";
                } else {  //se for false
                    strBits += "0";
                }
            }
            
            //convertendo a string
            int valorDecimal = 0;
            int base = 1;  // começa com 1 (2^0)
            
            //da direita para esquerda que é a leitura do binário
            for(int i = 7; i >= 0; i--) {
                if(strBits[i] == '1') {
                    valorDecimal += base;
                }
                base *= 2;  // prox potencia: 1 2 4 8 16 32 64 128
            }
            
            unsigned char byte = (unsigned char)valorDecimal;
            out.write(reinterpret_cast<const char*>(&byte), 1);
        }

    }
    //------------COMPRIMIR--------------//
    else if(strcmp(argv[1],"d")==0){     
        /*Se o argumento for “d” ele deverá ler um arquivo comprimido (o caminho será o segundo 
        argumento) e gravar a versão descomprimida em outro arquivo (o caminho será o terceiro argumento).*/
        std::ifstream in(argv[2], std::ios::binary);
        std::ofstream out(argv[3]);

        //le as frequências q estao salvas no arquivo comprimido
        int freqs[256];
        for (int i = 0; i < 256; ++i) {
            in.read(reinterpret_cast<char*>(&freqs[i]), sizeof(int));
        }

        // le o tamanho original do arquivo descomprimido
        int tamanhoOriginal;
        in.read(reinterpret_cast<char*>(&tamanhoOriginal), sizeof(int));
        
        //le o número de bits válidos no último byte  -> tenho que ignorar o resto pq foi
        //usado so pra completar o byte
        int qntd_bits_validos;
        in.read(reinterpret_cast<char*>(&qntd_bits_validos), sizeof(int));

        //le os bytes comprimidos e converte para bits
        MyVec<bool> bits;
        unsigned char byte;
        int cont_bits=0;
        
        //em quanto ainda tiver bits no arquivo comprimido e eles forem bits validos
        while(cont_bits < qntd_bits_validos && in.read(reinterpret_cast<char*>(&byte), 1)) {
    
            int numero = (int)byte;  //transforma o char pra tabela asc
            
            //converte o número de volta para 8 bits 
            string strBits  = "";
            
            //binario pra decimal
            int acumulador = numero;
            for(int i = 0; i < 8; i++) {
                int bit = acumulador % 2;  // 0 ou 1
                strBits  = (bit == 1 ? "1" : "0") + strBits ;  // adiciona na frente
                acumulador = acumulador/2;     // Divide por 2 para próximo bit
            }
            
            //string para bools 
            for(int i = 0; i < 8 && cont_bits < qntd_bits_validos; i++) {
                bool bitVal=(strBits[i] == '1');
                bits.push_back(bitVal);
                cont_bits++;
            }
        }

        in.close();

        //reconstrói a árvore e descomprime
        HuffManTree arv(freqs);
        MyVec<char> saida;
        arv.descomprimir(saida, bits);

        //grava resultado descomprimido
        for (int i = 0; i < saida.size(); ++i) {
            out.put(saida[i]);
        }

        out.close();
    }


    return 0;
}