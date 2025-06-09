#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <limits>

// Реализация TCP клиента
void clientTcp(){

    std::cout << "Ingect Client" << std::endl;

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

    // std::cout << "Server: " << inet_ntoa(clientAdd.sin_addr) << ":" << ntohs(clientAdd.sin_port) << std::endl;
    if(connect(clientSoket, (struct sockaddr*)&clientAdd, sizeof(clientAdd)) == -1){
        perror("Error connect");
        close(clientSoket);
        return;
    }
    int i = 1;
    while(true){
        std::string messege;
        std::cout << "Введите текст для вывода на сервер [" << i << "]: ";
        
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, messege);

        if(send(clientSoket, messege.c_str(), messege.size(), 0) == -1){
            perror("Error send");
            close(clientSoket);
            break;
        }
        i++;

        if(messege == "Bay") break;
    }
    close(clientSoket);
}

int main(){
    clientTcp();
}
