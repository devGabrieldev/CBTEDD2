using System;
using System.Collections.Generic;

namespace BibliotecaApp
{
    
    public class Emprestimo
    {
        private DateTime dtEmprestimo;
        private DateTime dtDevolucao;

        public DateTime DtEmprestimo
        {
            get { return dtEmprestimo; }
            set { dtEmprestimo = value; }
        }

        public DateTime DtDevolucao
        {
            get { return dtDevolucao; }
            set { dtDevolucao = value; }
        }

        public Emprestimo(DateTime dtEmprestimo)
        {
            this.dtEmprestimo = dtEmprestimo;
            this.dtDevolucao = DateTime.MinValue;
        }
    }

    public class Exemplar
    {
        private int tombo;
        private List<Emprestimo> emprestimos;

        public int Tombo
        {
            get { return tombo; }
            set { tombo = value; }
        }

        public List<Emprestimo> Emprestimos
        {
            get { return emprestimos; }
        }

        public Exemplar(int tombo)
        {
            this.tombo = tombo;
            this.emprestimos = new List<Emprestimo>();
        }

        public bool disponivel()
        {
            if (emprestimos.Count == 0) return true;
            Emprestimo ultimo = emprestimos[emprestimos.Count - 1];
            return ultimo.DtDevolucao != DateTime.MinValue;
        }

        public bool emprestar()
        {
            if (disponivel())
            {
                emprestimos.Add(new Emprestimo(DateTime.Now));
                return true;
            }
            return false;
        }

        public bool devolver()
        {
            if (!disponivel())
            {
                Emprestimo ultimo = emprestimos[emprestimos.Count - 1];
                ultimo.DtDevolucao = DateTime.Now;
                return true;
            }
            return false;
        }

        public int qtdeEmprestimos()
        {
            return emprestimos.Count;
        }
    }

    public class Livro
    {
        private int isbn;
        private string titulo;
        private string autor;
        private string editora;
        private List<Exemplar> exemplares;

        public int Isbn
        {
            get { return isbn; }
            set { isbn = value; }
        }

        public string Titulo
        {
            get { return titulo; }
            set { titulo = value; }
        }

        public string Autor
        {
            get { return autor; }
            set { autor = value; }
        }

        public string Editora
        {
            get { return editora; }
            set { editora = value; }
        }

        public List<Exemplar> Exemplares
        {
            get { return exemplares; }
        }

        public Livro(int isbn, string titulo, string autor, string editora)
        {
            this.isbn = isbn;
            this.titulo = titulo;
            this.autor = autor;
            this.editora = editora;
            this.exemplares = new List<Exemplar>();
        }

        public void adicionarExemplar(Exemplar exemplar)
        {
            exemplares.Add(exemplar);
        }

        public int qtdeExemplares()
        {
            return exemplares.Count;
        }

        public int qtdeDisponiveis()
        {
            int count = 0;
            foreach (var ex in exemplares)
            {
                if (ex.disponivel()) count++;
            }
            return count;
        }

        public int qtdeEmprestimos()
        {
            int count = 0;
            foreach (var ex in exemplares)
            {
                count += ex.qtdeEmprestimos();
            }
            return count;
        }

        public double percDisponibilidade()
        {
            if (qtdeExemplares() == 0) return 0.0;
            return ((double)qtdeDisponiveis() / qtdeExemplares()) * 100.0;
        }
    }

    public class Livros
    {
        private List<Livro> acervo;

        public List<Livro> Acervo
        {
            get { return acervo; }
        }

        public Livros()
        {
            this.acervo = new List<Livro>();
        }

        public void adicionar(Livro livro)
        {
            acervo.Add(livro);
        }

        public Livro pesquisar(Livro livro)
        {
            foreach (var l in acervo)
            {
                if (l.Isbn == livro.Isbn)
                    return l;
            }
            return null;
        }

        public Livro pesquisarPorIsbn(int isbn)
        {
            foreach (var l in acervo)
            {
                if (l.Isbn == isbn)
                    return l;
            }
            return null;
        }
    }

    class Program
    {
        static int LerNatural(string mensagem, int minimo = 0)
        {
            int valor;
            while (true)
            {
                Console.Write(mensagem);
                string entrada = Console.ReadLine();
                if (int.TryParse(entrada, out valor) && valor >= minimo)
                {
                    return valor;
                }
                Console.WriteLine($"[Erro] Entrada inválida! Digite apenas números naturais (maiores ou iguais a {minimo}).");
            }
        }

        static void Main(string[] args)
        {
            Livros biblioteca = new Livros();
            int opcao = -1;

            while (opcao != 0)
            {
                Console.WriteLine("\n==================================");
                Console.WriteLine("    SISTEMA DE BIBLIOTECA - MENU");
                Console.WriteLine("==================================");
                Console.WriteLine("1. Adicionar livro");
                Console.WriteLine("2. Pesquisar livro (sintético)");
                Console.WriteLine("3. Pesquisar livro (analítico)");
                Console.WriteLine("4. Adicionar exemplar");
                Console.WriteLine("5. Registrar empréstimo");
                Console.WriteLine("6. Registrar devolução");
                Console.WriteLine("0. Sair");

                opcao = LerNatural("Escolha uma opção: ", 0);

                switch (opcao)
                {
                    case 1:
                        Console.Write("\n--- Adicionar Livro ---\n");
                        int isbn = LerNatural("ISBN: ", 1);

                        if (biblioteca.pesquisarPorIsbn(isbn) != null)
                        {
                            Console.WriteLine("[Erro] Já existe um livro cadastrado com este ISBN.");
                            break;
                        }

                        Console.Write("Título: ");
                        string titulo = Console.ReadLine();
                        Console.Write("Autor: ");
                        string autor = Console.ReadLine();
                        Console.Write("Editora: ");
                        string editora = Console.ReadLine();

                        Livro novoLivro = new Livro(isbn, titulo, autor, editora);
                        biblioteca.adicionar(novoLivro);
                        Console.WriteLine("[Sucesso] Livro adicionado com sucesso!");
                        break;

                    case 2:
                        Console.Write("\n--- Pesquisa Sintética de Livro ---\n");
                        int isbnS = LerNatural("Informe o ISBN: ", 1);
                        Livro livroS = biblioteca.pesquisarPorIsbn(isbnS);

                        if (livroS != null)
                        {
                            Console.WriteLine($"\n[Livro Encontrado]");
                            Console.WriteLine($"Título: {livroS.Titulo} | Autor: {livroS.Autor} | Editora: {livroS.Editora} | ISBN: {livroS.Isbn}");
                            Console.WriteLine($"Total de Exemplares: {livroS.qtdeExemplares()}");
                            Console.WriteLine($"Exemplares Disponíveis: {livroS.qtdeDisponiveis()}");
                            Console.WriteLine($"Total de Empréstimos: {livroS.qtdeEmprestimos()}");
                            Console.WriteLine($"Percentual de Disponibilidade: {livroS.percDisponibilidade():F2}%");
                        }
                        else
                        {
                            Console.WriteLine("[Erro] Livro não encontrado.");
                        }
                        break;

                    case 3:
                        Console.Write("\n--- Pesquisa Analítica de Livro ---\n");
                        int isbnA = LerNatural("Informe o ISBN: ", 1);
                        Livro livroA = biblioteca.pesquisarPorIsbn(isbnA);

                        if (livroA != null)
                        {
                            Console.WriteLine($"\n[Livro Encontrado]");
                            Console.WriteLine($"Título: {livroA.Titulo} | Autor: {livroA.Autor} | Editora: {livroA.Editora} | ISBN: {livroA.Isbn}");
                            Console.WriteLine($"Total de Exemplares: {livroA.qtdeExemplares()}");
                            Console.WriteLine($"Exemplares Disponíveis: {livroA.qtdeDisponiveis()}");
                            Console.WriteLine($"Total de Empréstimos: {livroA.qtdeEmprestimos()}");
                            Console.WriteLine($"Percentual de Disponibilidade: {livroA.percDisponibilidade():F2}%");

                            Console.WriteLine("\n--- Detalhes dos Exemplares e Empréstimos ---");
                            if (livroA.Exemplares.Count == 0)
                            {
                                Console.WriteLine("Nenhum exemplar cadastrado para este livro.");
                            }
                            foreach (var ex in livroA.Exemplares)
                            {
                                string statusEx = ex.disponivel() ? "Disponível" : "Emprestado";
                                Console.WriteLine($"  -> Tombo: {ex.Tombo} | Status: {statusEx} | Qtde Empréstimos: {ex.qtdeEmprestimos()}");
                                foreach (var emp in ex.Emprestimos)
                                {
                                    string devStr = emp.DtDevolucao == DateTime.MinValue ? "Em aberto" : emp.DtDevolucao.ToString("dd/MM/yyyy HH:mm");
                                    Console.WriteLine($"      * Empréstimo: {emp.DtEmprestimo:dd/MM/yyyy HH:mm} | Devolução: {devStr}");
                                }
                            }
                        }
                        else
                        {
                            Console.WriteLine("[Erro] Livro não encontrado.");
                        }
                        break;

                    case 4:
                        Console.Write("\n--- Adicionar Exemplar ---\n");
                        int isbnEx = LerNatural("Informe o ISBN do livro: ", 1);
                        Livro livroEx = biblioteca.pesquisarPorIsbn(isbnEx);

                        if (livroEx != null)
                        {
                            int quantidadeUnidades = LerNatural("Quantas unidades de exemplares deseja adicionar? ", 1);

                            for (int i = 0; i < quantidadeUnidades; i++)
                            {
                                Console.WriteLine($"\n-> Cadastrando exemplar {i + 1} de {quantidadeUnidades}");
                                int tombo = LerNatural("Número do Tombo do Exemplar: ", 1);

                                bool tomboExiste = false;
                                foreach (var ex in livroEx.Exemplares)
                                {
                                    if (ex.Tombo == tombo) { tomboExiste = true; break; }
                                }

                                if (tomboExiste)
                                {
                                    Console.WriteLine("[Erro] Já existe um exemplar com este tombo para este livro. Tente novamente.");
                                    i--; 
                                }
                                else
                                {
                                    Exemplar novoEx = new Exemplar(tombo);
                                    livroEx.adicionarExemplar(novoEx);
                                    Console.WriteLine("[Sucesso] Exemplar adicionado com sucesso!");
                                }
                            }
                        }
                        else
                        {
                            Console.WriteLine("[Erro] Livro não encontrado.");
                        }
                        break;

                    case 5:
                        Console.Write("\n--- Registrar Empréstimo ---\n");
                        int isbnEmp = LerNatural("Informe o ISBN do livro: ", 1);
                        Livro livroEmp = biblioteca.pesquisarPorIsbn(isbnEmp);

                        if (livroEmp != null)
                        {
                            int tomboEmp = LerNatural("Informe o número do Tombo do exemplar: ", 1);
                            Exemplar exemplarAlvo = null;
                            foreach (var ex in livroEmp.Exemplares)
                            {
                                if (ex.Tombo == tomboEmp) { exemplarAlvo = ex; break; }
                            }

                            if (exemplarAlvo != null)
                            {
                                if (exemplarAlvo.emprestar())
                                {
                                    Console.WriteLine("[Sucesso] Empréstimo registrado com sucesso!");
                                }
                                else
                                {
                                    Console.WriteLine("[Erro] Exemplar encontra-se indisponível no momento.");
                                }
                            }
                            else
                            {
                                Console.WriteLine("[Erro] Exemplar não encontrado.");
                            }
                        }
                        else
                        {
                            Console.WriteLine("[Erro] Livro não encontrado.");
                        }
                        break;

                    case 6:
                        Console.Write("\n--- Registrar Devolução ---\n");
                        int isbnDev = LerNatural("Informe o ISBN do livro: ", 1);
                        Livro livroDev = biblioteca.pesquisarPorIsbn(isbnDev);

                        if (livroDev != null)
                        {
                            int tomboDev = LerNatural("Informe o número do Tombo do exemplar: ", 1);
                            Exemplar exemplarAlvoDev = null;
                            foreach (var ex in livroDev.Exemplares)
                            {
                                if (ex.Tombo == tomboDev) { exemplarAlvoDev = ex; break; }
                            }

                            if (exemplarAlvoDev != null)
                            {
                                if (exemplarAlvoDev.devolver())
                                {
                                    Console.WriteLine("[Sucesso] Devolução registrada com sucesso!");
                                }
                                else
                                {
                                    Console.WriteLine("[Erro] Este exemplar não possui empréstimo em aberto.");
                                }
                            }
                            else
                            {
                                Console.WriteLine("[Erro] Exemplar não encontrado.");
                            }
                        }
                        else
                        {
                            Console.WriteLine("[Erro] Livro não encontrado.");
                        }
                        break;

                    case 0:
                        Console.WriteLine("Encerrando o sistema...");
                        break;

                    default:
                        Console.WriteLine("Opção inválida! Escolha uma opção existente no menu.");
                        break;
                }
            }
        }
    }
}