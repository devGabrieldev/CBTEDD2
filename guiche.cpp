#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <sstream>
#include <ctime> 

using namespace std;


string intParaString(int valor) {
    stringstream ss;
    ss << valor;
    return ss.str();
}


string obterDataAtual() {
    time_t agora = time(0);
    tm* tempoLocal = localtime(&agora);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", tempoLocal);
    return string(buffer);
}


string obterHoraAtual() {
    time_t agora = time(0);
    tm* tempoLocal = localtime(&agora);
    char buffer[9];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", tempoLocal);
    return string(buffer);
}


class Senha {
private:
    int id;
    string dataGerac;
    string horaGerac;
    string dataAtend;
    string horaAtend;

public:
    Senha() : id(0), dataGerac(""), horaGerac(""), dataAtend(""), horaAtend("") {}
    
    Senha(int id) {
        this->id = id;
        this->dataGerac = obterDataAtual(); 
        this->horaGerac = obterHoraAtual(); 
        this->dataAtend = "";
        this->horaAtend = "";
    }

    int getId() const { return id; }

    void registrarAtendimento() {
        this->dataAtend = obterDataAtual();
        this->horaAtend = obterHoraAtual();
    }
    string dadosParciais() const {
        return intParaString(id) + " - " + dataGerac + " - " + horaGerac;
    }
    string dadosCompletos() const {
        return intParaString(id) + " - " + dataGerac + " - " + horaGerac + " - " + dataAtend + " - " + horaAtend;
    }
};

class Senhas {
private:
    int proximoAtendimento;
    queue<Senha> filaSenhas;

public:
    Senhas() {
        proximoAtendimento = 1;
    }

    void gerar() {
        Senha novaSenha(proximoAtendimento);
        filaSenhas.push(novaSenha);
        cout << "[Sucesso] Senha gerada: " << novaSenha.dadosParciais() << "\n";
        proximoAtendimento++;
    }

    queue<Senha>& getFilaSenhas() {
        return filaSenhas;
    }

    void listarAguardando() {
        if (filaSenhas.empty()) {
            cout << "\n Nao ha senhas aguardando atendimento.\n";
            return;
        }
        queue<Senha> copiaFila = filaSenhas;
        cout << "--- Senhas na Fila de Espera ---\n";
        while (!copiaFila.empty()) {
            cout << "  " << copiaFila.front().dadosParciais() << "\n";
            copiaFila.pop();
        }
    }
};


class Guiche {
private:
    int id;
    queue<Senha> atendimentos;

public:
    Guiche() {
        this->id = 0;
    }

    Guiche(int id) {
        this->id = id;
    }

    int getId() const { return id; }

    bool chamar(queue<Senha>& filaSenhasGeral) {
        if (filaSenhasGeral.empty()) {
            return false;
        }
        
        Senha senhaAtual = filaSenhasGeral.front();
        filaSenhasGeral.pop();
        
        senhaAtual.registrarAtendimento();

        atendimentos.push(senhaAtual);
        
        cout << "\n>>> Guiche " << id << " chamando Senha ID: " << senhaAtual.getId() << " <<<\n";
        return true;
    }

    void listarHistorico() {
        cout << "Historico do Guiche [" << id << "]:\n";
        if (atendimentos.empty()) {
            cout << "  Nenhum atendimento realizado.\n";
            return;
        }
        queue<Senha> copiaAtendimentos = atendimentos;
        while (!copiaAtendimentos.empty()) {
            cout << "  " << copiaAtendimentos.front().dadosCompletos() << "\n";
            copiaAtendimentos.pop();
        }
    }
};


class Guiches {
private:
    vector<Guiche> listaGuiches;

public:
    Guiches() {}

    void adicionar(Guiche guiche) {
        listaGuiches.push_back(guiche);
    }

    Guiche* obterGuiche(int id) {
        for (size_t i = 0; i < listaGuiches.size(); i++) {
            if (listaGuiches[i].getId() == id) {
                return &listaGuiches[i];
            }
        }
        return NULL;
    }

    void listarTodosAtendimentos() {
        if (listaGuiches.empty()) {
            cout << "\n Nenhum guiche cadastrado no sistema.\n";
            return;
        }
        for (size_t i = 0; i < listaGuiches.size(); i++) {
            listaGuiches[i].listarHistorico();
            cout << "------------------------------------\n";
        }
    }
};


int main() {
    Senhas controleSenhas;
    Guiches controleGuiches;
    int opcao = -1;

    while (opcao != 0) {
        cout << "\n===== SISTEMA DE ATENDIMENTO E SENHAS =====\n";
        cout << "1. Gerar Senha\n";
        cout << "2. Cadastrar Novo Guiche\n";
        cout << "3. Chamar Proxima Senha por Guiche\n";
        cout << "4. Listar Senhas Aguardando (Fila)\n";
        cout << "5. Listar Atendimentos Realizados (Por Guiche)\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            controleSenhas.gerar();
        }
        else if (opcao == 2) {
            int idGuiche;
            cout << "Digite o ID ou Numero do novo Guiche: ";
            cin >> idGuiche;

            if (controleGuiches.obterGuiche(idGuiche) != NULL) {
                cout << "[!] Erro: Um guiche com esse numero ja existe.\n";
            } else {
                controleGuiches.adicionar(Guiche(idGuiche));
                cout << "[Sucesso] Guiche " << idGuiche << " cadastrado.\n";
            }
        }
        else if (opcao == 3) {
            int idGuiche;
            cout << "Informe o numero do Guiche que ira chamar: ";
            cin >> idGuiche;

            Guiche* g = controleGuiches.obterGuiche(idGuiche);
            if (g != NULL) {
                if (!g->chamar(controleSenhas.getFilaSenhas())) {
                    cout << "[Aviso] Nao ha nenhuma senha na fila para ser chamada.\n";
                }
            } else {
                cout << "[!] Erro: Guiche nao encontrado.\n";
            }
        }
        else if (opcao == 4) {
            controleSenhas.listarAguardando();
        }
        else if (opcao == 5) {
            cout << "\n=== HISTORICO GERAL DE ATENDIMENTOS ===\n";
            controleGuiches.listarTodosAtendimentos();
        }
    }

    cout << "\nSistema encerrado.\n";
    return 0;
}
