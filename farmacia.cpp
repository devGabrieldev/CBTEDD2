#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>
#include <sstream> 

using namespace std;


string intParaString(int valor) {
    stringstream ss;
    ss << valor;
    return ss.str();
}

bool dataEValida(const string& dataStr) {
    if (dataStr.length() != 10 || dataStr[2] != '/' || dataStr[5] != '/') {
        return false;
    }

    int dia, mes, ano;
    char barra1, barra2;
    istringstream ss(dataStr);
    
    if (!(ss >> dia >> barra1 >> mes >> barra2 >> ano)) {
        return false;
    }

    if (mes < 1 || mes > 12 || dia < 1) {
        return false;
    }

    int diasPorMes[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (mes == 2) {
        bool ehBissexto = ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0));
        int maxDiasFevereiro = ehBissexto ? 29 : 28;
        
        if (dia > maxDiasFevereiro) {
            return false;
        }
    } 
    else {
        if (dia > diasPorMes[mes]) {
            return false;
        }
    }

    return true;
}

class Lote {
private:
    int id;
    int qtde;
    string venc; 

public:
    Lote() : id(0), qtde(0), venc("") {}
    
    Lote(int id, int qtde, string venc) {
        this->id = id;
        this->qtde = qtde;
        this->venc = venc;
    }

    int getId() const { return id; }
    int getQtde() const { return qtde; }
    void setQtde(int q) { this->qtde = q; }
    string getVenc() const { return venc; }

    string toString() const {
        return intParaString(id) + "-" + intParaString(qtde) + "-" + venc;
    }
};


class Medicamento {
private:
    int id;
    string nome;
    string laboratorio;
    queue<Lote> lotes;

public:
    Medicamento() : id(0), nome(""), laboratorio("") {}
    
    Medicamento(int id, string nome, string laboratorio) {
        this->id = id;
        this->nome = nome;
        this->laboratorio = laboratorio;
    }

    int getId() const { return id; }
    string getNome() const { return nome; }
    string getLaboratorio() const { return laboratorio; }
    
    queue<Lote> getLotes() const { return lotes; }

    int qtdeDisponivel() {
        int total = 0;
        queue<Lote> temp = lotes; 
        while (!temp.empty()) {
            total += temp.front().getQtde();
            temp.pop();
        }
        return total;
    }

    void comprar(Lote lote) {
        lotes.push(lote);
    }

    bool vender(int qtde) {
        if (qtdeDisponivel() < qtde) {
            return false; 
        }

        while (qtde > 0 && !lotes.empty()) {
            Lote& loteAtual = lotes.front(); 
            
            if (loteAtual.getQtde() > qtde) {
                loteAtual.setQtde(loteAtual.getQtde() - qtde);
                qtde = 0;
            } else {
                qtde -= loteAtual.getQtde();
                lotes.pop();
            }
        }
        return true;
    }

    string toString() {
        return intParaString(id) + "-" + nome + "-" + laboratorio + "-" + intParaString(qtdeDisponivel());
    }

    bool equals(const Medicamento& obj) const {
        return this->id == obj.id;
    }
};


class Medicamentos {
private:
    vector<Medicamento> listaMedicamentos;

public:
    Medicamentos() {}

    void adicionar(Medicamento medicamento) {
        listaMedicamentos.push_back(medicamento);
    }

    bool deletar(Medicamento medicamento) {
        for (vector<Medicamento>::iterator it = listaMedicamentos.begin(); it != listaMedicamentos.end(); ++it) {
            if (it->equals(medicamento)) {
                if (it->qtdeDisponivel() == 0) {
                    listaMedicamentos.erase(it);
                    return true;
                }
                return false; 
            }
        }
        return false;
    }

    Medicamento pesquisar(Medicamento medicamento) {
        for (size_t i = 0; i < listaMedicamentos.size(); i++) {
            if (listaMedicamentos[i].equals(medicamento)) {
                return listaMedicamentos[i];
            }
        }
        return Medicamento(); 
    }

    Medicamento* pesquisarRef(int id) {
        for (size_t i = 0; i < listaMedicamentos.size(); i++) {
            if (listaMedicamentos[i].getId() == id) {
                return &listaMedicamentos[i];
            }
        }
        return NULL; 
    }

    void listarTodos() {
        if (listaMedicamentos.empty()) {
            cout << "Nenhum medicamento cadastrado.\n";
            return;
        }
        for (size_t i = 0; i < listaMedicamentos.size(); i++) {
            cout << listaMedicamentos[i].toString() << "\n";
        }
    }
};

int main() {
    Medicamentos farmacia;
    int opcao = -1;

    while (opcao != 0) {
        cout << "\n=== CONTROLE DE ESTOQUE - FARMACIA ===\n";
        cout << "1. Cadastrar medicamento\n";
        cout << "2. Consultar medicamento (Sintetico)\n";
        cout << "3. Consultar medicamento (Analitico)\n";
        cout << "4. Comprar medicamento (Cadastrar Lote)\n";
        cout << "5. Vender medicamento\n";
        cout << "6. Listar medicamentos\n";
        cout << "0. Finalizar processo\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            int id;
            string nome, lab;
            cout << "ID do Medicamento: "; cin >> id;
            cout << "Nome: "; cin.ignore(); getline(cin, nome);
            cout << "Laboratorio: "; getline(cin, lab);

            if (farmacia.pesquisarRef(id) != NULL) {
                cout << "\n Erro: Medicamento com este ID ja existe.\n";
            } else {
                farmacia.adicionar(Medicamento(id, nome, lab));
                cout << "\n Medicamento cadastrado com sucesso!\n";
            }
        }
        else if (opcao == 2) {
            int id;
            cout << "Informe o ID do medicamento: "; cin >> id;
            Medicamento* m = farmacia.pesquisarRef(id);
            if (m != NULL) {
                cout << "\n[Dados Sinteticos]: " << m->toString() << "\n";
            } else {
                cout << "\n Medicamento nao encontrado.\n";
            }
        }
        else if (opcao == 3) {
            int id;
            cout << "Informe o ID do medicamento: "; cin >> id;
            Medicamento* m = farmacia.pesquisarRef(id);
            if (m != NULL) {
                cout << "\n[Dados Sinteticos]: " << m->toString() << "\n";
                cout << "--- DETALHE DOS LOTES ---\n";
                queue<Lote> tempLotes = m->getLotes();
                if (tempLotes.empty()) {
                    cout << "\n Nenhum lote cadastrado para este medicamento.\n";
                }
                while (!tempLotes.empty()) {
                    cout << "  Lote: " << tempLotes.front().toString() << "\n";
                    tempLotes.pop();
                }
            } else {
                cout << "\n Medicamento nao encontrado.\n";
            }
        }
        else if (opcao == 4) {
            int idMed;
            cout << "ID do Medicamento para abastecer: "; cin >> idMed;
            Medicamento* m = farmacia.pesquisarRef(idMed);
            if (m != NULL) {
                int idLote, qtde;
                string venc;
                cout << "ID do Lote: "; cin >> idLote;
                cout << "Quantidade: "; cin >> qtde;                
                
                do {
                    cout << "Vencimento (DD/MM/AAAA): "; 
                    cin >> venc;
                    
                    if (!dataEValida(venc)) {
                        cout << "[!] Erro: Data invalida! Verifique o formato DD/MM/AAAA ou os limites de dias (ex: Fevereiro).\n";
                    }
                } while (!dataEValida(venc));

                m->comprar(Lote(idLote, qtde, venc));
                cout << "\n Lote adicionado com sucesso!\n";
            } else {
                cout << "\n Medicamento nao encontrado.\n";
            }
        }
        else if (opcao == 5) {
            int idMed, qtde;
            cout << "ID do Medicamento para venda: "; cin >> idMed;
            Medicamento* m = farmacia.pesquisarRef(idMed);
            if (m != NULL) {
                cout << "Quantidade a vender: "; cin >> qtde;
                if (m->vender(qtde)) {
                    cout << "\n Venda realizada com sucesso (priorizando lote mais antigo).\n";
                } else {
                    cout << "\n Erro: Saldo insuficiente para realizar a venda. Estoque atual: " << m->qtdeDisponivel() << "\n";
                }
            } else {
                cout << "\n Medicamento nao encontrado.\n";
            }
        }
        else if (opcao == 6) {
            cout << "\n=== LISTAGEM GERAL DE MEDICAMENTOS ===\n";
            farmacia.listarTodos();
        }
    }

    cout << "\nProcesso finalizado.\n";
    return 0;
}
