/*********************************************************************
 * INCLUDES
 ********************************************************************/
#include <iomanip>
#include <fstream>
#include <exception>
#include <limits>
#include <iostream>
#include <iterator>
#include "Jogos.hpp"

/*********************************************************************
 * IMPORTANTES
 ********************************************************************/
/**
 * codigos semelhantes a esse:

    int decisao;
    std::cin >> decisao;

    while(1)
    {           
        if( (decisao < 0 || decisao > 3) || !std::cin) -> caso o input esteja errado
        {
            std::cin.clear(); -> limpa o as flags de erro de cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); -> ignora o input indevido

            std::cout << std::endl << std::endl << std::endl << "Opção inválida. Escolha novamente: ";
            -> alerta o usuario que o input foi erroneo

            std::cin >> decisao; -> permite ao usuario inserir novo input
        }
        else break;
    }

 * servem para pegar e validar o input do usuario
 * 
 ****************************************************************
 * Todas as funcoes desse arquivo, exceto main e loadJogos,
 * recebem o endereço de memoria de um vector da classe
 * Jogos. Os vectors passados de funcao em funcao representam
 * o mesmo vector criado em main, retornado por loadJogos.
 */



/**********************************************************************
 * PROTOTIPOS
 *********************************************************************/
void inputJogos(std::vector<Jogos>& jogos);
bool calculos(std::vector<Jogos>& jogos);
void printTabela(std::vector<Jogos>& jogos);
void reset(std::vector<Jogos>& jogos);
bool printMenu(std::vector<Jogos>& jogos);
void saveJogos(std::vector<Jogos>& jogos);
std::vector<Jogos> loadJogos();
void alteraJogo(std::vector<Jogos>& jogos);

/**********************************************************************
 * FUNCAO MAIN
 *********************************************************************/
/**
 * Aqui ocorre a criacao do vector da
 * classe Jogos que vai ser passada para
 * todas as funcoes, excluindo a 
 * loadJogos() (pois essa cria o vector e
 * o retorna para o vector de main).
 * Main nao precisa de argumentos para inicializar.
 */
int main(int argc, char const *argv[])
{
    CLEAN;
    std::vector<Jogos> jogos = loadJogos();

    /*essa variavel define quando o 
    programa deve sair do loop principal*/
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



/**********************************************************************
 * IMPLEMENTACOES
 *********************************************************************/
/**
 * Carrega os jogos ja salvos.
 * Caso nao exista, a funcao
 * cria esse arquivo, que vai
 * estar vazio.
 * Retorna um vetor da classe
 * Jogos, gerado apartir do
 * arquivo externo.
 */
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



/**
 * Salva os jogos existentes no programa.
 */
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



/**
 * Mostra na tela o menu principal
 * e fornece ao usuario a possibilidade de:
 * ver a tabela de jogos, registrar novos jogos,
 * sair e deletar todos os jogos existentes.
 * 
 * A funcao retorna um bool que define se o 
 * programa sai do loop principal (caso o usu-
 * ario queira).
 */
bool printMenu(std::vector<Jogos>& jogos)
{
    CLEAN;
    saveJogos(jogos);

    std::cout << "O que deseja fazer? " << std::endl <<
    "[0] Ver tabela de jogos" << std::endl <<
    "[1] Inserir jogo" << std::endl <<
    "[2] Sair" << std::endl <<
    "[3] Voltar a 'versão de fábrica'" << std::endl;

    int decisao;
    std::cin >> decisao;

    while(1)
    {           
        if( (decisao < 0 || decisao > 3) || !std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << std::endl << std::endl << "Opção inválida. Escolha novamente: ";
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


/**
 * A funcao exclui todos os jogos ja registrados.
 * 
 * Por seguranca, a funcao pede que o usuario
 * confirme duas vezes sua decisao.
 */
void reset(std::vector<Jogos>& jogos)
{
    CLEAN;
    std::cout << "Retornar à versão de fábrica apagará todos os" << std::endl <<
    "registros do programa. Essa ação não poderá ser desfeita." << std::endl << std::endl;

    std::cout << "[1] Estou ciente disso e quero prosseguir" << std::endl <<
    "[0] Não fazer qualquer alteração e retornar ao menu." << std::endl;

    int decisao;
    while(1)
    {
        std::cin >> decisao;
        
        if((decisao != 1 && decisao != 0) || !std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << std::endl << std::endl << "Opção inválida. Escolha novamente: ";
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


/**
 * Esta funcao mostra todos os jogos ja registrados
 * e pede que o usuario informe qual deles deve ser
 * excluido, ou se deseja cancelar a acao
 */
void deleteJogo(std::vector<Jogos>& jogos)
{
    CLEAN;

    int precision = 10;
    std::cout << std::left << std::setw(precision) <<
    "Jogos" << std::setw(precision) << "Placar" << std::setw(precision + 15) <<
    "Mínimo da Temporada" << std::setw(precision + 15) <<
    "Máximo da Temporada" << std::setw(precision + 15) <<
    "Quebra de recorde min." << std::setw(precision + 15) << "Quebra de recorde max." << std::endl;

    std::vector<Jogos>::iterator it = jogos.begin();
    for(int i = 0; it != jogos.end(); it++, i++)
    {
        std::cout << std::left << std::setw(10) << i + 1;
        it->printJogo();
    }

    std::cout << std::endl << "Escolha o número do jogo que você quer excluir ou digite -1 para cancelar: ";

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
            std::cout << std::endl << std::endl << std::endl << "Opção inválida. Escolha novamente: ";
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


/**
 * Imprime a tabela com todos os jogos e suas
 * informacoes. Recebe input do usuario, este
 * informa se quer voltar ao menu principal,
 * alterar ou excluir quaisquer jogos existentes
 */
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
    "Mínimo da Temporada" << std::setw(precision + 15) <<
    "Máximo da Temporada" << std::setw(precision + 15) <<
    "Quebra de recorde min." << std::setw(precision + 15) << "Quebra de recorde max." << std::endl;

    for(int i = 0; i < jogos.size(); i++)
    {
        std::cout << std::left << std::setw(10) << i + 1;
        jogos[i].printJogo();
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
            std::cout << std::endl << std::endl << std::endl << "Opção inválida. Escolha novamente: ";
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


/**
 * A funcao ira determinar todas as informacoes como
 * recordes e maximos da temporada apartir do placar
 */
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


/**
 * A funcao requisita ao usuario para inserir um ou
 * mais placares de jogos.
 */
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
            std::cout << std::endl << "Deseja inserir mais? [1]Sim [0]Não: ";
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

    saveJogos(jogos);
}


/**
 * Esta funcao exibe a tabela e pergunta ao usuario
 * qual dos jogos ele deseja alterar ou se deseja
 * abortar a acao
 */
void alteraJogo(std::vector<Jogos>& jogos)
{
    CLEAN;

    int precision = 10;
    std::cout << std::left << std::setw(precision) <<
    "Jogos" << std::setw(precision) << "Placar" << std::setw(precision + 15) <<
    "Mínimo da Temporada" << std::setw(precision + 15) <<
    "Máximo da Temporada" << std::setw(precision + 15) <<
    "Quebra de recorde min." << std::setw(precision + 15) << "Quebra de recorde max." << std::endl;

    for(int i = 0; i < jogos.size(); i++)
    {
        std::cout << std::left << std::setw(10) << i + 1;
        jogos[i].printJogo();
    }

    std::cout << std::endl << "Escolha o número do jogo que você quer alterar ou digite -1 para cancelar: ";

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
            std::cout << std::endl << std::endl << std::endl << "Opção inválida. Escolha novamente: ";
            std::cin >> decisao;
        }
        else break;
    }

    jogos[decisao - 1].setJogo();
}
