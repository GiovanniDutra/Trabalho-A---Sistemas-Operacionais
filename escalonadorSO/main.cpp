#include "include/LeitorConfig.h"

#include <exception>
#include <iostream>
#include <string>

int main() {
    try {
        std::string caminho;
        std::cout << "Digite o caminho completo do arquivo de configuracao: ";
        std::getline(std::cin, caminho);

        // Permite colar o caminho entre aspas.
        if (caminho.size() >= 2 &&
            caminho.front() == '"' &&
            caminho.back() == '"') {
            caminho = caminho.substr(1, caminho.size() - 2);
        }

        ConfiguracaoSistema configuracao = carregarConfiguracao(caminho);

        // Cada ativacao periodica gera uma ocorrencia de execucao.
        for (Tarefa& tarefa : configuracao.tarefas) {
            for (int numero = 1; numero <= 10; numero++) {
                Ocorrencia ocorrencia{};

                ocorrencia.numero = numero;
                ocorrencia.instanteAtivacao =
                    tarefa.ingresso +
                    static_cast<long long>(numero - 1) * tarefa.periodo;

                ocorrencia.deadlineAbsoluto =
                    ocorrencia.instanteAtivacao + tarefa.prazo;

                ocorrencia.tempoRestante = tarefa.duracao;
                ocorrencia.estado = EstadoOcorrencia::Futura;
                ocorrencia.cpuAtual = -1;

                tarefa.ocorrencias.push_back(ocorrencia);
            }
        }

        std::cout << "\nConfiguracao lida com sucesso.\n";
        std::cout << "Algoritmo: " << configuracao.algoritmo << "\n";
        std::cout << "Quantum: " << configuracao.quantum << "\n";
        std::cout << "CPUs: " << configuracao.quantidadeCpus << "\n";
        std::cout << "Tarefas periodicas carregadas: "
                  << configuracao.tarefas.size() << "\n";
        std::cout << "Tarefas aperiodicas ignoradas: "
                  << configuracao.tarefasAperiodicasIgnoradas << "\n";

        for (const Tarefa& tarefa : configuracao.tarefas) {
            std::cout << "\nTarefa " << tarefa.id
                      << " | cor #" << tarefa.cor
                      << " | ingresso " << tarefa.ingresso
                      << " | duracao " << tarefa.duracao
                      << " | periodo " << tarefa.periodo
                      << " | prazo " << tarefa.prazo << "\n";
        }

        std::cout << "\nOcorrencias criadas:\n";

        for (const Tarefa& tarefa : configuracao.tarefas) {
            for (const Ocorrencia& ocorrencia : tarefa.ocorrencias) {
                std::cout << "Tarefa " << tarefa.id
                          << ", execucao " << ocorrencia.numero
                          << " | chega no tick "
                          << ocorrencia.instanteAtivacao
                          << " | prazo no tick "
                          << ocorrencia.deadlineAbsoluto
                          << " | falta executar "
                          << ocorrencia.tempoRestante
                          << " ticks\n";
            }
        }

        if (configuracao.tarefasAperiodicasIgnoradas > 0) {
            std::cout << "\nAviso: tarefas aperiodicas foram ignoradas "
                         "no Projeto A.\n";
        }

        if (configuracao.tarefas.empty()) {
            std::cout << "\nNao ha tarefas periodicas para simular.\n";
        }

        return 0;
    } catch (const std::exception& erro) {
        std::cerr << "\nErro: " << erro.what() << "\n";
        return 1;
    }
}