#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

string doubleParaString(double valor) {
    stringstream ss;
    ss << valor;
    return ss.str();
}

string intParaString(int valor) {
    stringstream ss;
    ss << valor;
    return ss.str();
}

class Item {
private:
    int id;
    string descricao;
    double preco;

public:
    Item() : id(0), descricao(""), preco(0.0) {}

    Item(int id, string descricao, double preco) {
        this->id = id;
        this->descricao = descricao;
        this->preco = preco;
    }

    int getId() const { return id; }
    string getDescricao() const { return descricao; }
    double getPreco() const { return preco; }

    bool equals(const Item& outro) const {
        return this->id == outro.id;
    }
};

class Pedido {
private:
    int id;
    string cliente;
    Item itens[10];
    int qtdeItens;  

public:
    Pedido() : id(0), cliente(""), qtdeItens(0) {}

    Pedido(int id, string cliente) {
        this->id = id;
        this->cliente = cliente;
        this->qtdeItens = 0;
    }

    int getId() const { return id; }
    string getCliente() const { return cliente; }
    int getQtdeItens() const { return qtdeItens; }

    bool adicionarItem(Item item) {
        if (qtdeItens >= 10) {
            return false; 
        }
        itens[qtdeItens] = item;
        qtdeItens++;
        return true;
    }

    bool removerItem(Item item) {
        int indiceEncontrado = -1;
        for (int i = 0; i < qtdeItens; i++) {
            if (itens[i].equals(item)) {
                indiceEncontrado = i;
                break;
            }
        }
        
        if (indiceEncontrado == -1) {
            return false; 
        }

        
        for (int i = indiceEncontrado; i < qtdeItens - 1; i++) {
            itens[i] = itens[i + 1];
        }
        qtdeItens--;
        return true;
    }

    double calcularTotal() {
        double total = 0.0;
        for (int i = 0; i < qtdeItens; i++) {
            total += itens[i].getPreco();
        }
        return total;
    }

    string dadosDoPedido() {
        string dados = "Pedido ID: " + intParaString(id) + " | Cliente: " + cliente + "\nItens:\n";
        if (qtdeItens == 0) {
            dados += "  (Nenhum item adicionado)\n";
        }
        for (int i = 0; i < qtdeItens; i++) {
            dados += "  - " + itens[i].getDescricao() + " (R$ " + doubleParaString(itens[i].getPreco()) + ")\n";
        }
        dados += "Total do Pedido: R$ " + doubleParaString(calcularTotal());
        return dados;
    }

    bool equals(const Pedido& outro) const {
        return this->id == outro.id;
    }
};

class Restaurante {
private:
    int proxPedido;
    Pedido pedidos[50]; 
    int qtdePedidos;    

public:
    Restaurante() {
        this->proxPedido = 1; 
        this->qtdePedidos = 0;
    }

    int getProxPedido() const { return proxPedido; }
    int getQtdePedidos() const { return qtdePedidos; }

    bool novoPedido(Pedido pedido) {
        if (qtdePedidos >= 50) {
            return false; 
        }
        pedidos[qtdePedidos] = pedido;
        qtdePedidos++;
        proxPedido++; 
        return true;
    }

    Pedido buscarPedido(Pedido pedido) {
        for (int i = 0; i < qtdePedidos; i++) {
            if (pedidos[i].equals(pedido)) {
                return pedidos[i];
            }
        }
        return Pedido();
    }

    
    Pedido* buscarPedidoRef(int idBuscado) {
        for (int i = 0; i < qtdePedidos; i++) {
            if (pedidos[i].getId() == idBuscado) {
                return &pedidos[i];
            }
        }
        return NULL;
    }

    bool cancelarPedido(Pedido pedido) {
        int indiceEncontrado = -1;
        for (int i = 0; i < qtdePedidos; i++) {
            if (pedidos[i].equals(pedido)) {
                indiceEncontrado = i;
                break;
            }
        }

        if (indiceEncontrado == -1) {
            return false; 
        }

        for (int i = indiceEncontrado; i < qtdePedidos - 1; i++) {
            pedidos[i] = pedidos[i + 1];
        }
        qtdePedidos--;
        return true;
    }

    double calcularSomaGeralDoDia() {
        double somaGeral = 0.0;
        for (int i = 0; i < qtdePedidos; i++) {
            somaGeral += pedidos[i].calcularTotal();
        }
        return somaGeral;
    }

    void listarTodosPedidos() {
        if (qtdePedidos == 0) {
            cout << "Nenhum pedido registrado hoje.\n";
            return;
        }
        for (int i = 0; i < qtdePedidos; i++) {
            cout << "ID: " << pedidos[i].getId() 
                 << " | Cliente: " << pedidos[i].getCliente() 
                 << " | Total: R$ " << pedidos[i].calcularTotal() << "\n";
        }
    }
};


int main() {
    Restaurante cozinha;
    int opcao = -1;
    Item cardapio[5];
    cardapio[0] = Item(101, "Marmita Executiva Frango", 22.50);
    cardapio[1] = Item(102, "Marmita Executiva Carne", 25.00);
    cardapio[2] = Item(103, "Suco Natural de Laranja", 7.50);
    cardapio[3] = Item(104, "Refrigerante em Lata", 6.00);
    cardapio[4] = Item(105, "Sobremesa Pudim", 8.00);

    while (opcao != 0) {
        cout << "\n========== COZINHA INDUSTRIAL - SELETOR ==========\n";
        cout << "1. Criar novo pedido\n";
        cout << "2. Adicionar item ao pedido\n";
        cout << "3. Remover item do pedido\n";
        cout << "4. Consultar pedido (id, cliente, itens e total)\n";
        cout << "5. Cancelar pedido\n";
        cout << "6. Listar todos os pedidos (faturamento do dia)\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            if (cozinha.getQtdePedidos() >= 50) {
                cout << "[Erro] Capacidade esgotada! Limite de 50 pedidos atingido hoje.\n";
            } else {
                string cliente;
                cout << "Nome do Cliente: "; cin.ignore(); getline(cin, cliente);
                
                int idGerado = cozinha.getProxPedido();
                Pedido p(idGerado, cliente);
                
                cozinha.novoPedido(p);
                cout << "[Sucesso] Pedido numero " << idGerado << " criado para " << cliente << ".\n";
            }
        }
        else if (opcao == 2) {
            int idProj;
            cout << "Informe o ID do pedido: "; cin >> idProj;
            Pedido* p = cozinha.buscarPedidoRef(idProj);

            if (p != NULL) {
                cout << "\n--- CARDAPIO DISPONIVEL ---\n";
                for (int i = 0; i < 5; i++) {
                    cout << i + 1 << ". " << cardapio[i].getDescricao() << " - R$ " << cardapio[i].getPreco() << "\n";
                }
                int escolha;
                cout << "Escolha o numero do item para adicionar: "; cin >> escolha;

                if (escolha >= 1 && escolha <= 5) {
                    if (p->adicionarItem(cardapio[escolha - 1])) {
                        cout << "[Sucesso] Item adicionado ao pedido.\n";
                    } else {
                        cout << "[Erro] Nao foi possivel adicionar. Limite de 10 itens estourou.\n";
                    }
                } else {
                    cout << "[Erro] Opcao de cardapio invalida.\n";
                }
            } else {
                cout << "[Erro] Pedido nao encontrado.\n";
            }
        }
        else if (opcao == 3) {
            int idProj;
            cout << "Informe o ID do pedido: "; cin >> idProj;
            Pedido* p = cozinha.buscarPedidoRef(idProj);

            if (p != NULL) {
                int idItem;
                cout << "Informe o ID do item que deseja remover: "; cin >> idItem;
                
                // Instancia um objeto Item temporário apenas com o ID correspondente para comparação
                Item itemTemp(idItem, "", 0.0);
                if (p->removerItem(itemTemp)) {
                    cout << "[Sucesso] Item removido do pedido.\n";
                } else {
                    cout << "[Erro] Item nao localizado dentro deste pedido.\n";
                }
            } else {
                cout << "[Erro] Pedido nao encontrado.\n";
            }
        }
        else if (opcao == 4) {
            int idProj;
            cout << "Informe o ID do pedido para consulta: "; cin >> idProj;
            Pedido* p = cozinha.buscarPedidoRef(idProj);

            if (p != NULL) {
                cout << "\n================ DOS DADOS DO PEDIDO ================\n";
                cout << p->dadosDoPedido() << "\n";
			}
		}
	}
}