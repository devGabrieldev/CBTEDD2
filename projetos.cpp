#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

// ==========================================
// FUNÇÕES AUXILIARES (Padrão C++98)
// ==========================================
string intParaString(int valor) {
    stringstream ss;
    ss << valor;
    return ss.str();
}

string obterDataHoraAtual() {
    time_t agora = time(0);
    tm* tempoLocal = localtime(&agora);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", tempoLocal);
    return string(buffer);
}

// ==========================================
// CLASSE: Tarefa
// ==========================================
class Tarefa {
private:
    int id;
    string titulo;
    string descricao;
    int prioridade; // 1=alta, 2=média, 3=baixa
    string status;  // "Aberta", "Fechada", "Cancelada"
    string dataCriacao;
    string dataConclusao;

public:
    Tarefa() : id(0), prioridade(3), status("Aberta"), dataCriacao(""), dataConclusao("") {}

    Tarefa(int id, string titulo, string descricao, int prioridade) {
        this->id = id;
        this->titulo = titulo;
        this->descricao = descricao;
        this->prioridade = prioridade;
        this->status = "Aberta";
        this->dataCriacao = obterDataHoraAtual();
        this->dataConclusao = "";
    }

    int getId() const { return id; }
    string getStatus() const { return status; }
    int getPrioridade() const { return prioridade; }
    string getTitulo() const { return titulo; }

    void concluir() {
        this->status = "Fechada";
        this->dataConclusao = obterDataHoraAtual();
    }

    void cancelar() {
        this->status = "Cancelada";
        this->dataConclusao = "";
    }

    void reabrir() {
        this->status = "Aberta";
        this->dataConclusao = "";
    }

    bool equals(const Tarefa& outra) const {
        return this->id == outra.id;
    }

    string toString() const {
        string pStr = (prioridade == 1) ? "Alta" : (prioridade == 2 ? "Media" : "Baixa");
        return "ID: " + intParaString(id) + " | " + titulo + " [" + pStr + "] - Status: " + status + " (Criada: " + dataCriacao + (dataConclusao.empty() ? "" : " | Concluida: " + dataConclusao) + ")";
    }
};

// ==========================================
// CLASSE: Projeto
// ==========================================
class Projeto {
private:
    int id;
    string nome;
    vector<Tarefa> tarefas;

public:
    Projeto() : id(0), nome("") {}

    Projeto(int id, string nome) {
        this->id = id;
        this->nome = nome;
    }

    int getId() const { return id; }
    string getNome() const { return nome; }
    vector<Tarefa> getTarefas() const { return tarefas; }

    void adicionarTarefa(Tarefa t) {
        tarefas.push_back(t);
    }

    bool removerTarefa(Tarefa t) {
        for (vector<Tarefa>::iterator it = tarefas.begin(); it != tarefas.end(); ++it) {
            if (it->equals(t)) {
                tarefas.erase(it);
                return true;
            }
        }
        return false;
    }

    Tarefa buscarTarefa(Tarefa t) {
        for (size_t i = 0; i < tarefas.size(); i++) {
            if (tarefas[i].equals(t)) {
                return tarefas[i];
            }
        }
        return Tarefa();
    }

    Tarefa* buscarTarefaRef(int idTarefa) {
        for (size_t i = 0; i < tarefas.size(); i++) {
            if (tarefas[i].getId() == idTarefa) {
                return &tarefas[i];
            }
        }
        return NULL;
    }

    vector<Tarefa> tarefasPorStatus(string s) {
        vector<Tarefa> filtradas;
        for (size_t i = 0; i < tarefas.size(); i++) {
            if (tarefas[i].getStatus() == s) {
                filtradas.push_back(tarefas[i]);
            }
        }
        return filtradas;
    }

    vector<Tarefa> tarefasPorPrioridade(int p) {
        vector<Tarefa> filtradas;
        for (size_t i = 0; i < tarefas.size(); i++) {
            if (tarefas[i].getPrioridade() == p) {
                filtradas.push_back(tarefas[i]);
            }
        }
        return filtradas;
    }

    int totalAbertas() {
        int cont = 0;
        for (size_t i = 0; i < tarefas.size(); i++) {
            if (tarefas[i].getStatus() == "Aberta") cont++;
        }
        return cont;
    }

    int totalFechadas() {
        int cont = 0;
        for (size_t i = 0; i < tarefas.size(); i++) {
            if (tarefas[i].getStatus() == "Fechada") cont++;
        }
        return cont;
    }

    int totalCanceladas() {
        int cont = 0;
        for (size_t i = 0; i < tarefas.size(); i++) {
            if (tarefas[i].getStatus() == "Cancelada") cont++;
        }
        return cont;
    }

    bool equals(const Projeto& outro) const {
        return this->id == outro.id;
    }
};

// ==========================================
// CLASSE: Projetos
// ==========================================
class Projetos {
private:
    vector<Projeto> itens;

public:
    Projetos() {}

    bool adicionar(Projeto p) {
        if (buscarRef(p.getId()) != NULL) {
            return false;
        }
        itens.push_back(p);
        return true;
    }

    bool remover(Projeto p) {
        for (vector<Projeto>::iterator it = itens.begin(); it != itens.end(); ++it) {
            if (it->equals(p)) {
                if (it->getTarefas().empty()) {
                    itens.erase(it);
                    return true;
                }
                return false; // Bloqueia remoção se possuir tarefas cadastradas
            }
        }
        return false;
    }

    Projeto buscar(Projeto p) {
        for (size_t i = 0; i < itens.size(); i++) {
            if (itens[i].equals(p)) {
                return itens[i];
            }
        }
        return Projeto();
    }

    Projeto* buscarRef(int idProjeto) {
        for (size_t i = 0; i < itens.size(); i++) {
            if (itens[i].getId() == idProjeto) {
                return &itens[i];
            }
        }
        return NULL;
    }

    vector<Projeto> listar() {
        return itens;
    }
};

// ==========================================
// PROGRAMA PRINCIPAL
// ==========================================
int main() {
    Projetos sistemaProjetos;
    int opcao = -1;

    while (opcao != 0) {
        cout << "\n================ MENU DE GERENCIAMENTO ================\n";
        cout << "1. Adicionar projeto\n";
        cout << "2. Pesquisar projeto (Tarefas por status e totais)\n";
        cout << "3. Remover projeto (Apenas sem tarefas)\n";
        cout << "4. Adicionar tarefa em projeto\n";
        cout << "5. Concluir tarefa\n";
        cout << "6. Cancelar tarefa\n";
        cout << "7. Reabrir tarefa\n";
        cout << "8. Listar tarefas de um projeto\n";
        cout << "9. Filtrar tarefas por status ou prioridade em um projeto\n";
        cout << "10. Filtrar tarefas por status ou prioridade em todos os projetos\n";
        cout << "11. Resumo geral da empresa\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            int id; string nome;
            cout << "ID do Projeto: "; cin >> id;
            cout << "Nome do Projeto: "; cin.ignore(); getline(cin, nome);
            if (sistemaProjetos.adicionar(Projeto(id, nome))) {
                cout << "[Sucesso] Projeto cadastrado com sucesso.\n";
            } else {
                cout << "[Erro] Ja existe um projeto com este ID.\n";
            }
        }
        else if (opcao == 2) {
            int id; cout << "ID do Projeto: "; cin >> id;
            Projeto* p = sistemaProjetos.buscarRef(id);
            if (p != NULL) {
                cout << "\n=== PROJETO: " << p->getNome() << " ===\n";
                cout << "Total Abertas: " << p->totalAbertas() << "\n";
                cout << "Total Fechadas: " << p->totalFechadas() << "\n";
                cout << "Total Canceladas: " << p->totalCanceladas() << "\n";
                
                cout << "\n-- TAREFAS ABERTAS --\n";
                vector<Tarefa> abertas = p->tarefasPorStatus("Aberta");
                for (size_t i = 0; i < abertas.size(); i++) cout << "  " << abertas[i].toString() << "\n";
                
                cout << "\n-- TAREFAS FECHADAS --\n";
                vector<Tarefa> fechadas = p->tarefasPorStatus("Fechada");
                for (size_t i = 0; i < fechadas.size(); i++) cout << "  " << fechadas[i].toString() << "\n";
            } else {
                cout << "[Erro] Projeto nao encontrado.\n";
            }
        }
        else if (opcao == 3) {
            int id; cout << "ID do Projeto para remover: "; cin >> id;
            Projeto* p = sistemaProjetos.buscarRef(id);
            if (p != NULL) {
                if (sistemaProjetos.remover(*p)) {
                    cout << "[Sucesso] Projeto removido com sucesso.\n";
                } else {
                    cout << "[Erro] Nao foi possivel remover. O projeto possui tarefas vinculadas.\n";
                }
            } else {
                cout << "[Erro] Projeto nao encontrado.\n";
            }
        }
        else if (opcao == 4) {
            int idProj; cout << "ID do Projeto destino: "; cin >> idProj;
            Projeto* p = sistemaProjetos.buscarRef(idProj);
            if (p != NULL) {
                int idT, prio; string tit, desc;
                cout << "ID da Tarefa: "; cin >> idT;
                if (p->buscarTarefaRef(idT) != NULL) {
                    cout << "[Erro] Uma tarefa com este ID ja existe neste projeto.\n";
                } else {
                    cout << "Titulo: "; cin.ignore(); getline(cin, tit);
                    cout << "Descricao: "; getline(cin, desc);
                    cout << "Prioridade (1=alta, 2=media, 3=baixa): "; cin >> prio;
                    if (prio < 1 || prio > 3) prio = 3;
                    p->adicionarTarefa(Tarefa(idT, tit, desc, prio));
                    cout << "[Sucesso] Tarefa vinculada ao projeto.\n";
                }
            } else {
                cout << "[Erro] Projeto nao encontrado.\n";
            }
        }
        else if (opcao == 5) {
            int idProj, idT; cout << "ID do Projeto: "; cin >> idProj;
            Projeto* p = sistemaProjetos.buscarRef(idProj);
            if (p != NULL) {
                cout << "ID da Tarefa para Concluir: "; cin >> idT;
                Tarefa* t = p->buscarTarefaRef(idT);
                if (t != NULL) { 
                    t->concluir(); 
                    cout << "[Sucesso] Tarefa alterada para Fechada.\n"; 
                } else {
                    cout << "[Erro] Tarefa nao encontrada.\n";
                }
            } else {
                cout << "[Erro] Projeto nao encontrado.\n";
            }
        }
        else if (opcao == 6) {
            int idProj, idT; cout << "ID do Projeto: "; cin >> idProj;
            Projeto* p = sistemaProjetos.buscarRef(idProj);
            if (p != NULL) {
                cout << "ID da Tarefa para Cancelar: "; cin >> idT;
                Tarefa* t = p->buscarTarefaRef(idT);
                if (t != NULL) { 
                    t->cancelar(); 
                    cout << "[Sucesso] Tarefa alterada para Cancelada.\n"; 
                } else {
                    cout << "[Erro] Tarefa nao encontrada.\n";
                }
            } else {
                cout << "[Erro] Projeto nao encontrado.\n";
            }
        }
        else if (opcao == 7) {
            int idProj, idT; cout << "ID do Projeto: "; cin >> idProj;
            Projeto* p = sistemaProjetos.buscarRef(idProj);
            if (p != NULL) {
                cout << "ID da Tarefa para Reabrir: "; cin >> idT;
                Tarefa* t = p->buscarTarefaRef(idT);
                if (t != NULL) { 
                    t->reabrir(); 
                    cout << "[Sucesso] Tarefa reaberta para Aberta.\n"; 
                } else {
                    cout << "[Erro] Tarefa nao encontrada.\n";
                }
            } else {
                cout << "[Erro] Projeto nao encontrado.\n";
            }
        }
        else if (opcao == 8) {
            int idProj; cout << "ID do Projeto: "; cin >> idProj;
            Projeto* p = sistemaProjetos.buscarRef(idProj);
            if (p != NULL) {
                cout << "\n--- LISTA DE TAREFAS DO PROJETO: " << p->getNome() << " ---\n";
                vector<Tarefa> tfs = p->getTarefas();
                if (tfs.empty()) {
                    cout << "\n Nenhuma tarefa cadastrada.\n";
                }
                for (size_t i = 0; i < tfs.size(); i++) {
                    cout << tfs[i].toString() << "\n";
                }
            } else {
                cout << "[Erro] Projeto nao encontrado.\n";
            }
        }
        else if (opcao == 9) {
            int idProj; cout << "ID do Projeto: "; cin >> idProj;
            Projeto* p = sistemaProjetos.buscarRef(idProj);
            if (p != NULL) {
                int tipoFiltro; cout << "Filtrar por: (1) Status ou (2) Prioridade? "; cin >> tipoFiltro;
                if (tipoFiltro == 1) {
                    string st; cout << "Status (Aberta, Fechada, Cancelada): "; cin >> st;
                    vector<Tarefa> res = p->tarefasPorStatus(st);
                    for (size_t i = 0; i < res.size(); i++) cout << res[i].toString() << "\n";
                } else {
                    int pr; cout << "Prioridade (1, 2, 3): "; cin >> pr;
                    vector<Tarefa> res = p->tarefasPorPrioridade(pr);
                    for (size_t i = 0; i < res.size(); i++) cout << res[i].toString() << "\n";
                }
            } else {
                cout << "[Erro] Projeto nao encontrado.\n";
            }
        }
        else if (opcao == 10) {
            int tipoFiltro; cout << "Filtrar todos por: (1) Status ou (2) Prioridade? "; cin >> tipoFiltro;
            vector<Projeto> projs = sistemaProjetos.listar();
            if (tipoFiltro == 1) {
                string st; cout << "Status (Aberta, Fechada, Cancelada): "; cin >> st;
                for (size_t i = 0; i < projs.size(); i++) {
                    vector<Tarefa> res = projs[i].tarefasPorStatus(st);
                    for (size_t j = 0; j < res.size(); j++) cout << "[" << projs[i].getNome() << "] " << res[j].toString() << "\n";
                }
            } else {
                int pr; cout << "\n Prioridade (1, 2, 3): "; cin >> pr;
                for (size_t i = 0; i < projs.size(); i++) {
                    vector<Tarefa> res = projs[i].tarefasPorPrioridade(pr);
                    for (size_t j = 0; j < res.size(); j++) cout << "[" << projs[i].getNome() << "] " << res[j].toString() << "\n";
                }
            }
        }
        else if (opcao == 11) {
            vector<Projeto> projs = sistemaProjetos.listar();
            int totalProjs = projs.size();
            int tAbertas = 0, tFechadas = 0, tCanceladas = 0;
            for (size_t i = 0; i < projs.size(); i++) {
                tAbertas += projs[i].totalAbertas();
                tFechadas += projs[i].totalFechadas();
                tCanceladas += projs[i].totalCanceladas();
            }
            int totalGeral = tAbertas + tFechadas + tCanceladas;
            double porcentagem = 0.0;
            if (totalGeral > 0) {
                porcentagem = ((double)tFechadas / totalGeral) * 100.0;
            }
            cout << "\n================ RESUMO GERAL ================\n";
            cout << "Quantidade de Projetos: " << totalProjs << "\n";
            cout << "Tarefas Abertas: " << tAbertas << "\n";
            cout << "Tarefas Fechadas (Concluidas): " << tFechadas << "\n";
            cout << "Tarefas Canceladas: " << tCanceladas << "\n";
            cout << "Rendimento: " << porcentagem << "% concluidas em relacao ao total.\n";
        }
    }
    cout << "\nProcesso finalizado.\n";
    return 0;
}