#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <limits>

// Реализация TCP клиента
void clientTcp(){

    int clientSoket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSoket == -1){
        perror("Error soket");
        close(clientSoket);
        return;
    } 

    sockaddr_in clientAdd;
    clientAdd.sin_family = AF_INET;
    clientAdd.sin_port = htons(2222);
    clientAdd.sin_addr.s_addr = INADDR_ANY;

    std::cout << "Server: " << inet_ntoa(clientAdd.sin_addr) << ":" << ntohs(clientAdd.sin_port) << std::endl;
    
    // Подключение к серверу
    if(connect(clientSoket, (struct sockaddr*)&clientAdd, sizeof(clientAdd)) == -1){
        perror("Error connect");
        close(clientSoket);
        return;
    }
    int i = 1;
    while(true){
        std::string messege;
        std::cout << "Введите текст для вывода на сервер [" << i << "]: ";
        std::getline(std::cin, messege);

        // Заносим данные на сервер
        if(send(clientSoket, messege.c_str(), messege.size(), 0) == -1){
            perror("Error send");
            close(clientSoket);
            continue;
        }
        i++;

        // Можно убрать но будет бесконесный цикл если хотите определить количесвто клиентво замените while(true) на for(...)
        if(messege == "Bye!") break;
    }

    // Закрывалем сокер клиента
    close(clientSoket);
}

int main(){
    clientTcp();
}
