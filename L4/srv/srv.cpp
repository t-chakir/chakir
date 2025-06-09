#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>

void dateClient(int clientSoket){

    while (true){
        char buffer[2048];
        memset(buffer, 0, sizeof(buffer));
        int binar = recv(clientSoket, buffer, sizeof(buffer), 0);
        if(binar <= 0){
            if(binar == 0){
                std::cout << "Клиент отключитсял" << std::endl;
            } 
            else{
                perror("Error recv");
            }
            break;
        }  
        std::cout << "Количество байтов: " << sizeof(buffer) 
            << ", Количество символов: " << strlen(buffer) 
            << ", Количество бинарных данных: " << binar << std::endl;
        std::cout << "\nСообщение от клиента: " << buffer << std::endl;
    }

    close(clientSoket);
}

// Реализация TCP сервера
void srvTcp(){

    std::cout << "Ingect Server" << std::endl;

    int srvSoket = socket(AF_INET, SOCK_STREAM, 0);
    if(srvSoket == -1){
        perror("Error soket");
        close(srvSoket);
        return;
    } 

    sockaddr_in servrAdd;
    servrAdd.sin_family = AF_INET;
    servrAdd.sin_port = htons(2222);
    servrAdd.sin_addr.s_addr = INADDR_ANY;

    std::cout << "Server: " << inet_ntoa(servrAdd.sin_addr) << ":" << ntohs(servrAdd.sin_port) << std::endl;

    if(bind(srvSoket, (struct sockaddr*)&servrAdd, sizeof(servrAdd)) == -1){
        perror("Error bind");
        close(srvSoket);
        return;
    }

    if(listen(srvSoket, 5) == -1){
        perror("Error listen");
        close(srvSoket);
        return;
    }

    while(true){
        socklen_t sizeServrAdd = sizeof(servrAdd);
        int clientSoket = accept(srvSoket, (struct sockaddr*)&servrAdd, &sizeServrAdd);
        if(clientSoket == -1){
            perror("Error accept");
            close(srvSoket);
            return;       
        }
        std::thread(dateClient, clientSoket).detach();
    }

    close(srvSoket);
}

int main(){
    srvTcp();
}
