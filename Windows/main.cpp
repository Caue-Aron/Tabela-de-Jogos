#include <iomanip>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include "Jogos.hpp"

void inputJogos(std::vector<Jogos>& jogos);
bool calculos(std::vector<Jogos>& jogos);
void printTabela(std::vector<Jogos>& jogos);
void reset(std::vector<Jogos>& jogos);
bool printMenu(std::vector<Jogos>& jogos);
void saveJogos(std::vector<Jogos>& jogos);
std::vector<Jogos> loadJogos();
void alteraJogo(std::vector<Jogos>& jogos);

int main(int argc, char const *argv[])
{
    CLEAN;
    std::vector<Jogos> jogos = loadJogos();

    bool run = true;
    while(run)
    {
        run = printMenu(jogos);
        calculos(jogos);
    }

    saveJogos(jogos);
    CLEAN;

    return 0;
}





std::vector<Jogos> loadJogos()
{
    std::ifstream load_file("Saves.txt");
    std::vector<Jogos> jogos;
    Jogos temp_jogo;

    if(!load_file)
    {
        std::ofstream temp("Saves.txt");
        temp.close();
    }

    while(load_file >> temp_jogo)
    {
        jogos.push_back(temp_jogo);
    }

    return jogos;
}

void saveJogos(std::vector<Jogos>& jogos)
{
    std::ofstream save_file("Saves.txt", std::ios::out);

    if(save_file.tellp() == EOF || jogos.empty())
    {
        return;
    }

    for(int i = 0; i < jogos.size(); ++i)
    {
        save_file << jogos[i];
    }
}

bool printMenu(std::vector<Jogos>& jogos)
{
    CLEAN;
    saveJogos(jogos);

    std::cout << "O que deseja fazer? " << std::endl <<
    "[0] Ver tabela de treinos" << std::endl <<
    "[1] Inserir jogo" << std::endl <<
    "[2] Sair" << std::endl <<
    "[3] Voltar a 'versao de fabrica'" << std::endl;

    int decisao;
    std::cin >> decisao;

    while(1)
    {           
        if( (decisao < 0 || decisao > 3) || !std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << std::endl << std::endl << "Opção invalida. Escolha novamente: ";
            std::cin >> decisao;
        }
        else break;
    }

    switch(decisao)
    {
        case 0:
            printTabela(jogos);
        break;

        case 1:
            inputJogos(jogos);
        break; 

        case 2:
            return false;
        break;        

        case 3:
            reset(jogos);
        break;  
    }

    return true;
}

void reset(std::vector<Jogos>& jogos)
{
    CLEAN;
    std::cout << "Retornar à versão de fabrica apagara todos os" << std::endl <<
    "registros do programa. Essa ação não poderá ser desfeita." << std::endl << std::endl;

    std::cout << "[1] Estou ciente disso e quero prosseguir" << std::endl <<
    "[0] Nao fazer qualquer alteração e retornar ao menu." << std::endl;

    int decisao;
    while(1)
    {
        std::cin >> decisao;
        
        if((decisao != 1 && decisao != 0) || !std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << std::endl << std::endl << "Opção invalida. Escolha novamente: ";
        }
        else break;
    }

    if(decisao)
    {
        if(jogos.size() == 1)
            jogos.erase(jogos.begin());

        else
        for(std::vector<Jogos>::iterator i = jogos.begin(); i != jogos.end(); i++)
        {
            jogos.erase(i);
        }

        if(jogos.size() == 1)
            jogos.erase(jogos.begin());
    }
}

void deleteJogo(std::vector<Jogos>& jogos)
{
    CLEAN;

    int precision = 10;
    std::cout << std::left << std::setw(precision) <<
    "Jogos" << std::setw(precision) << "Placar" << std::setw(precision + 15) <<
    "Minimo da Temporada" << std::setw(precision + 15) <<
    "Maximo da Temporada" << std::setw(precision + 15) <<
    "Quebra de recorde min." << std::setw(precision + 15) << "Quebra de recorde max." << std::endl;

    std::vector<Jogos>::iterator it = jogos.begin();
    for(int i = 0; it != jogos.end(); it++, i++)
    {
        std::cout << std::left << std::setw(10) << i + 1;
        it->getJogo();
    }

    std::cout << std::endl << "Escolha o numero do jogo que você quer excluir ou digite -1 para cancelar: ";

    int decisao;
    std::cin >> decisao;
    while(1)
    {   
        if(decisao == -1)
            return;

        if(decisao < 0 || decisao > jogos.size() || !std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << std::endl << std::endl << "Opção invalida. Escolha novamente: ";
            std::cin >> decisao;
        }
        else break;
    }

    it = jogos.begin();
    for(int i = 0; i < decisao - 1; i++, it++);

    if(jogos.size() >= 2 && decisao == 1)
    {
        jogos[1].setMin_temp(jogos[1].getPlacar());
        jogos[1].setMax_temp(jogos[1].getPlacar());

        jogos[1].setRec_max(0);
        jogos[1].setRec_min(0);
    }

    jogos.erase(it);
}

void printTabela(std::vector<Jogos>& jogos)
{
    CLEAN;  
    saveJogos(jogos);  
    
    if(!calculos(jogos))
    {
        std::cout << "Nenhum jogo registrado. Pressione Enter para voltar.";
        getchar(); getchar();
        return;
    }

    int precision = 10;

    std::cout << std::left << std::setw(precision) <<
    "Jogos" << std::setw(precision) << "Placar" << std::setw(precision + 15) <<
    "Minimo da Temporada" << std::setw(precision + 15) <<
    "Maximo da Temporada" << std::setw(precision + 15) <<
    "Quebra de recorde min." << std::setw(precision + 15) << "Quebra de recorde max." << std::endl;

    for(int i = 0; i < jogos.size(); i++)
    {
        std::cout << std::left << std::setw(10) << i + 1;
        jogos[i].getJogo();
    }

    std::cout << std::endl << std::endl << "[0] Voltar para o menu" << std::endl <<
    "[1] Excluir jogo" << std::endl <<
    "[2] Alterar jogo" << std::endl;

    int decisao;
    std::cin >> decisao;

    while(1)
    {   
        if(decisao < 0 || decisao > 2 || !std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << std::endl << std::endl << "Opção invalida. Escolha novamente: ";
            std::cin >> decisao;
        }
        else break;
    }

    switch(decisao)
    {
        case 0:
            return;
        break;

        case 1:
            deleteJogo(jogos);
        break;        

        case 2:
            alteraJogo(jogos);
        break;  
    }
}

bool calculos(std::vector<Jogos>& jogos)
{
    if(jogos.empty())
    {
        return false;
    }

    int menor_temporada = jogos[0].getMin_temp();
    int maior_temporada = jogos[0].getMax_temp();

    for(int i = 0; i < jogos.size(); i++)
    {
        if(jogos[i].getPlacar() < menor_temporada)
        {
            menor_temporada = jogos[i].getPlacar();
            jogos[i].setRec_min(1);
        }

        if(jogos[i].getPlacar() > maior_temporada)
        {
            maior_temporada = jogos[i].getPlacar();
            jogos[i].setRec_max(1);
        }

        jogos[i].setMax_temp(maior_temporada);
        jogos[i].setMin_temp(menor_temporada);
    }

    saveJogos(jogos);
    return true;
}

void inputJogos(std::vector<Jogos>& jogos)
{
    int decisao = 1;

    while(decisao)
    {
        CLEAN;

        Jogos temp; temp.setJogo();
        jogos.push_back(temp);

        while(1)
        {
            std::cout << std::endl << "Deseja inserir mais? [1]Sim [0]Nao: ";
            std::cin >> decisao;
            
            if((decisao != 1 && decisao != 0) || !std::cin)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << std::endl << std::endl << std::endl << "Opção invalida. Escolha novamente: ";
            }
            else break;
        }
    }

    saveJogos(jogos);
}

void alteraJogo(std::vector<Jogos>& jogos)
{
    CLEAN;

    int precision = 10;
    std::cout << std::left << std::setw(precision) <<
    "Jogos" << std::setw(precision) << "Placar" << std::setw(precision + 15) <<
    "Minimo da Temporada" << std::setw(precision + 15) <<
    "Maximo da Temporada" << std::setw(precision + 15) <<
    "Quebra de recorde min." << std::setw(precision + 15) << "Quebra de recorde max." << std::endl;

    for(int i = 0; i < jogos.size(); i++)
    {
        std::cout << std::left << std::setw(10) << i + 1;
        jogos[i].getJogo();
    }

    std::cout << std::endl << "Escolha o numero do jogo que você quer alterar ou digite -1 para cancelar: ";

    int decisao;
    std::cin >> decisao;
    while(1)
    {   
        if(decisao == -1)
            return;

        if(decisao < 0 || decisao > jogos.size() || !std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << std::endl << std::endl << "Opção invalida. Escolha novamente: ";
            std::cin >> decisao;
        }
        else break;
    }

    jogos[decisao - 1].setJogo();
}
