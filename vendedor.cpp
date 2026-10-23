#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;
class Venda {
private:
    int qtde;
    double valor;

public:
    Venda() : qtde(0), valor(0.0) {}
    Venda(int qtde, double valor) {
        this->qtde = qtde;
        this->valor = valor;
    }

    int getQtde() const { return qtde; }
    double getValor() const { return valor; }

    double valorMedio() const {
        if (qtde == 0) return 0.0;
        return valor / qtde;
    }
};


class Vendedor {
private:
    int id;
    string nome;
    double percComissao;
    Venda* asVendas[31]; 

public:
    Vendedor() : id(0), nome(""), percComissao(0.0) {
        for (int i = 0; i < 31; i++) {
            asVendas[i] = NULL;
        }
    }

    Vendedor(int id, string nome, double percComissao) {
        this->id = id;
        this->nome = nome;
        this->percComissao = percComissao;
        for (int i = 0; i < 31; i++) {
            asVendas[i] = NULL;
        }
    }

    ~Vendedor() {
        for (int i = 0; i < 31; i++) {
            if (asVendas[i] != NULL) {
                delete asVendas[i];
            }
        }
    }

    int getId() const { return id; }
    string getNome() const { return nome; }
    double getPercComissao() const { return percComissao; }

    void registrarVenda(int dia, Venda venda) {
        if (dia >= 1 && dia <= 31) {
            if (asVendas[dia - 1] != NULL) {
                delete asVendas[dia - 1];
            }
            asVendas[dia - 1] = new Venda(venda);
        }
    }

    double valorVendas() const {
        double total = 0.0;
        for (int i = 0; i < 31; i++) {
            if (asVendas[i] != NULL) {
                total += asVendas[i]->getValor();
            }
        }
        return total;
    }

    double valorComissao() const {
        return valorVendas() * (percComissao / 100.0);
    }

    bool possuiVendas() const {
        for (int i = 0; i < 31; i++) {
            if (asVendas[i] != NULL) return true;
        }
        return false;
    }

    Venda* getVendaDia(int dia) const {
        if (dia >= 1 && dia <= 31) {
            return asVendas[dia - 1];
        }
        return NULL;
    }
};

class Vendedores {
private:
    Vendedor* osVendedores[10]; 
    int max;
    int qtde;

public:
    Vendedores(int max = 10) {
        this->max = max;
        this->qtde = 0;
        for (int i = 0; i < max; i++) {
            osVendedores[i] = NULL;
        }
    }

    ~Vendedores() {
        for (int i = 0; i < qtde; i++) {
            if (osVendedores[i] != NULL) {
                delete osVendedores[i];
            }
        }
    }

    int getQtde() const { return qtde; }
    int getMax() const { return max; }

    bool addVendedor(Vendedor* v) {
        if (qtde < max) {
            osVendedores[qtde] = v;
            qtde++;
            return true;
        }
        return false;
    }

    bool delVendedor(Vendedor* v) {
        for (int i = 0; i < qtde; i++) {
            if (osVendedores[i]->getId() == v->getId()) {
                delete osVendedores[i];
                for (int j = i; j < qtde - 1; j++) {
                    osVendedores[j] = osVendedores[j + 1];
                }
                osVendedores[qtde - 1] = NULL;
                qtde--;
                return true;
            }
        }
        return false;
    }

    Vendedor* searchVendedor(Vendedor* v) {
        for (int i = 0; i < qtde; i++) {
            if (osVendedores[i]->getId() == v->getId()) {
                return osVendedores[i];
            }
        }
        return NULL;
    }

    Vendedor* searchVendedorPorId(int id) {
        for (int i = 0; i < qtde; i++) {
            if (osVendedores[i]->getId() == id) {
                return osVendedores[i];
            }
        }
        return NULL;
    }

    Vendedor* getVendedorAt(int index) const {
        if (index >= 0 && index < qtde) {
            return osVendedores[index];
        }
        return NULL;
    }

    double valorVendas() const {
        double total = 0.0;
        for (int i = 0; i < qtde; i++) {
            total += osVendedores[i]->valorVendas();
        }
        return total;
    }

    double valorComissao() const {
        double total = 0.0;
        for (int i = 0; i < qtde; i++) {
            total += osVendedores[i]->valorComissao();
        }
        return total;
    }
};

int lerInteiro(string mensagem, int minimo = 0) {
    int valor;
    while (true) {
        cout << mensagem;
        if (cin >> valor && valor >= minimo) {
            return valor;
        }
        cout << "[Erro] Entrada invalida! Introduza um numero inteiro valido (>= " << minimo << ").\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double lerDouble(string mensagem, double minimo = 0.0) {
    double valor;
    while (true) {
        cout << mensagem;
        if (cin >> valor && valor >= minimo) {
            return valor;
        }
        cout << "[Erro] Entrada invalida! Introduza um valor numerico valido (>= " << minimo << ").\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    cout << fixed << setprecision(2);

    Vendedores equipe(10);
    int opcao = -1;

    while (opcao != 0) {
        cout << "\n========================================\n";
        cout << "    SISTEMA DE GESTAO COMERCIAL\n";
        cout << "========================================\n";
        cout << "1. Cadastrar vendedor\n";
        cout << "2. Consultar vendedor\n";
        cout << "3. Excluir vendedor\n";
        cout << "4. Registrar venda\n";
        cout << "5. Listar vendedores\n";
        cout << "0. Sair\n";

        opcao = lerInteiro("Escolha uma opcao: ", 0);

        switch (opcao) {
            case 1: {
                cout << "\n--- Cadastrar Vendedor ---\n";
                if (equipe.getQtde() >= equipe.getMax()) {
                    cout << "[Erro] Limite maximo de 10 vendedores atingido!\n";
                    break;
                }

                int id = lerInteiro("ID do Vendedor: ", 1);
                if (equipe.searchVendedorPorId(id) != NULL) {
                    cout << "[Erro] Ja existe um vendedor registado com este ID.\n";
                    break;
                }

                cout << "Nome do Vendedor: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string nome;
                getline(cin, nome);

                double percComissao = lerDouble("Percentual de Comissao (%): ", 0.0);

                Vendedor* novoVendedor = new Vendedor(id, nome, percComissao);
                if (equipe.addVendedor(novoVendedor)) {
                    cout << "[Sucesso] Vendedor cadastrado com sucesso!\n";
                } else {
                    delete novoVendedor;
                    cout << "[Erro] Nao foi possivel cadastrar o vendedor.\n";
                }
                break;
            }
            case 2: {
                cout << "\n--- Consultar Vendedor ---\n";
                int idCons = lerInteiro("Informe o ID do vendedor: ", 1);
                Vendedor* vCons = equipe.searchVendedorPorId(idCons);

                if (vCons != NULL) {
                    cout << "\n[Vendedor Encontrado]\n";
                    cout << "ID: " << vCons->getId() << "\n";
                    cout << "Nome: " << vCons->getNome() << "\n";
                    cout << "Percentual de Comissao: " << vCons->getPercComissao() << "%\n";
                    cout << "Valor Total das Vendas: R$ " << vCons->valorVendas() << "\n";
                    cout << "Valor da Comissao Devida: R$ " << vCons->valorComissao() << "\n";

                    cout << "\n-- Detalhes Diarios (Dias com Registo) --\n";
                    bool temVenda = false;
                    for (int dia = 1; dia <= 31; dia++) {
                        Venda* vd = vCons->getVendaDia(dia);
                        if (vd != NULL) {
                            temVenda = true;
                            cout << "  Dia " << (dia < 10 ? "0" : "") << dia 
                                 << " | Qtde Vendas: " << vd->getQtde() 
                                 << " | Valor Total: R$ " << vd->getValor() 
                                 << " | Valor Medio Diario: R$ " << vd->valorMedio() << "\n";
                        }
                    }
                    if (!temVenda) {
                        cout << "  Nenhum registo de venda encontrado para este vendedor.\n";
                    }
                } else {
                    cout << "[Erro] Vendedor nao encontrado.\n";
                }
                break;
            }
            case 3: {
                cout << "\n--- Excluir Vendedor ---\n";
                int idExc = lerInteiro("Informe o ID do vendedor para exclusao: ", 1);
                Vendedor* vExc = equipe.searchVendedorPorId(idExc);

                if (vExc != NULL) {
                    if (vExc->possuiVendas()) {
                        cout << "[Erro] O vendedor so podera ser excluido enquanto nao houver nenhuma venda associada a ele.\n";
                    } else {
                        if (equipe.delVendedor(vExc)) {
                            cout << "[Sucesso] Vendedor excluido com sucesso!\n";
                        } else {
                            cout << "[Erro] Nao foi possivel excluir o vendedor.\n";
                        }
                    }
                } else {
                    cout << "[Erro] Vendedor nao encontrado.\n";
                }
                break;
            }
            case 4: {
                cout << "\n--- Registrar Venda ---\n";
                int idVenda = lerInteiro("Informe o ID do vendedor: ", 1);
                Vendedor* vVenda = equipe.searchVendedorPorId(idVenda);

                if (vVenda != NULL) {
                    int dia = lerInteiro("Informe o dia do mes (1 a 31): ", 1);
                    if (dia > 31) {
                        cout << "[Erro] O mes possui no maximo 31 dias.\n";
                        break;
                    }

                    int qtde = lerInteiro("Quantidade de vendas realizadas no dia: ", 1);
                    double valor = lerDouble("Valor total financeiro das vendas no dia (R$): ", 0.0);

                    Venda novaVenda(qtde, valor);
                    vVenda->registrarVenda(dia, novaVenda);
                    cout << "[Sucesso] Venda registada com sucesso para o dia " << dia << "!\n";
                } else {
                    cout << "[Erro] Vendedor nao encontrado.\n";
                }
                break;
            }
            case 5: {
                cout << "\n--- Listagem Geral de Vendedores ---\n";
                if (equipe.getQtde() == 0) {
                    cout << "Nenhum vendedor registado no sistema.\n";
                    break;
                }

                for (int i = 0; i < equipe.getQtde(); i++) {
                    Vendedor* vList = equipe.getVendedorAt(i);
                    cout << "ID: " << vList->getId() 
                         << " | Nome: " << vList->getNome() 
                         << " | Vendas: R$ " << vList->valorVendas() 
                         << " | Comissao: R$ " << vList->valorComissao() << "\n";
                }

                cout << "--------------------------------------------------\n";
                cout << "TOTAL GERAL DA EQUIPE -> Vendas: R$ " << equipe.valorVendas() 
                     << " | Comissoes Devidas: R$ " << equipe.valorComissao() << "\n";
                break;
            }
            case 0:
                cout << "A encerrar o sistema...\n";
                break;
            default:
                cout << "Opcao invalida! Escolha uma opcao do menu.\n";
                break;
        }
    }

    return 0;
}

