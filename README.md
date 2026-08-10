# Banco Concorrente

Projeto desenvolvido em **C** com o objetivo de estudar conceitos de **redes, sockets TCP, concorrência e comunicação entre cliente e servidor**.

## ⚠️ Projeto de estudos

Este projeto foi desenvolvido exclusivamente para fins de **estudo e aprendizado**.

O código **não foi refatorado ou otimizado com foco em produção**. Algumas decisões de implementação foram mantidas dessa forma para facilitar o aprendizado e a experimentação dos conceitos estudados.

Portanto, o projeto pode conter:
- código que poderia ser melhor estruturado;
- tratamentos de erro simplificados;
- decisões de implementação voltadas ao aprendizado;
- pontos que poderiam ser refatorados em uma aplicação real.

O objetivo principal não é fornecer um sistema bancário pronto para uso, mas **estudar como um cliente e um servidor se comunicam e como o servidor pode atender múltiplos clientes simultaneamente**.

## 📚 Conceitos estudados

Durante o desenvolvimento foram trabalhados conceitos como:

- TCP/IP
- Sockets
- `socket()`
- `bind()`
- `listen()`
- `accept()`
- `send()`
- `recv()`
- Threads com `pthread`
- Mutex
- Concorrência
- File descriptors
- `select()`
- `fd_set`
- `SIGINT`
- `volatile sig_atomic_t`
- Comunicação entre Windows e Linux

## 🖥️ Arquitetura

O projeto possui dois programas:

```text
Cliente (Windows)
       │
       │ TCP
       ▼
Servidor (Linux)
       │
       ├── Thread → Cliente 1
       ├── Thread → Cliente 2
       └── Thread → Cliente 3
