/*********************************************************************
 * DEFINES PARA PREVENIR MULTIPLAS DEFINICOES
 ********************************************************************/
#ifndef JOGOS
#define JOGOS

/*********************************************************************
 * DEFINES
 ********************************************************************/
/**
 * comando rapido para limpar a tela.
 * Cada versao de sistema possui sua propria versao
 */
#define CLEAN system("cls")

/*********************************************************************
 * INCLUDES
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <typeinfo>

/*********************************************************************
 * CLASSES
 ********************************************************************/  
/**
 * Esta classe contem a informacao de um jogos especifico,
 * como placar, recordes, etc.
 */
class Jogos
{

/*********************************************************************
 * SOBRECARGAS
 ********************************************************************/
/**
 * Todas as sobrecargas permitem chamada em cascata.
 */

/**
 * Sobrecarga do operador '<<' para que objetos Jogos possam
 * ser gravados externamente com objetos ofstream.
 */
friend std::ofstream& operator<<(std::ofstream& out, Jogos jogo)
{
    out << jogo.placar << " " << jogo.min_temp << " " << jogo.max_temp
    << " " << jogo.rec_min << " " << jogo.rec_max << std::endl;

    return out;
}


/**
 * Sobrecarga do operador '>>' para que objetos Jogos possam
 * ser carregados apartir de arquivos externos com objetos ifstream.
 */
friend std::ifstream& operator>>(std::ifstream& in, Jogos& jogo)
{
    in >> jogo.placar >> jogo.min_temp >> jogo.max_temp
    >> jogo.rec_min >> jogo.rec_max;

    return in;
}



/********************************************************************************/
private:
    // informacoes de cada jogo
    int placar, max_temp, min_temp, rec_min, rec_max;


/********************************************************************************/
public:
    /**
     * Imprime as informcoes do jogo
     */
    void printJogo()
    {
        int precision = 10;

        std::cout << std::left << std::setw(precision) <<
        placar <<  std::setw(precision + 14) << 
        min_temp <<  std::setw(precision + 14) <<
        max_temp << std::setw(precision + 15) <<
        rec_min << std::setw(precision + 15) <<
        rec_max << std::endl;
    }


    /**
     * Pede para que o usuario insira um placar
     * para que o jogo e suas informacoes possam ser
     * formulados.
     */
    void setJogo()
    {
        int decisao = 1;

        while(decisao)
        {
            CLEAN;

            std::cout << "Insira o placar: "; std::cin >> placar; std::cout << std::endl;

            while(typeid(placar).name() != "i")
            {                
                if((decisao != 1 && decisao != 0) || !std::cin)
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << std::endl << std::endl << std::endl << "Opção inválida. Escolha novamente: ";
                }
                else break;

                std::cin >> placar;
            }

            while(1)
            {
                std::cout << std::endl << std::endl << "Deseja alterar? [1]Sim [0]Não: ";
                std::cin >> decisao;
                
                if((decisao != 1 && decisao != 0) || !std::cin)
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << std::endl << std::endl << std::endl << "Opção inválida. Escolha novamente: ";
                }
                else break;
            }
        }

        max_temp = min_temp = placar;
        rec_min = rec_max = 0;
    }


    /**
     * Getters e setters de cada variavel private
     */
    int getPlacar() { return placar; }
    void setPlacar(int val) { placar = val; }

    int getMax_temp() { return max_temp; }
    void setMax_temp(int val) { max_temp = val; }

    int getMin_temp() { return min_temp; }
    void setMin_temp(int val) { min_temp = val; }

    int getRec_min() { return rec_min; }
    void setRec_min(int val) { rec_min = val; }

    int getRec_max() { return rec_max; }
    void setRec_max(int val) { rec_max = val; }
};

#endif