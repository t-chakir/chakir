#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>

void dateClient(int clientSoket){

    while (true){
        char buffer[2048]; // Локальный буфер
        memset(buffer, 0, sizeof(buffer)); // Очищаем буфер от прошлых данных
        int binar = recv(clientSoket, buffer, sizeof(buffer), 0); // Полученые данные от клиента
        // Если клиент отключился или ошибка
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

    // Закрывалем сокер клиента
    close(clientSoket);
}

// Реализация TCP сервера
void srvTcp(){

    // Создаем сокет сервера типо сокета TCP(SOCK_STREAM) семейство IPv4(AF_INET)
    int srvSoket = socket(AF_INET, SOCK_STREAM, 0);
    if(srvSoket == -1){
        perror("Error soket");
        close(srvSoket);
        return;
    } 

    // Определяем адрес сервера
    sockaddr_in servrAdd;
    servrAdd.sin_family = AF_INET;
    servrAdd.sin_port = htons(2222);
    servrAdd.sin_addr.s_addr = INADDR_ANY; // Если нету какого либо конкретного IP

    std::cout << "Server: " << inet_ntoa(servrAdd.sin_addr) << ":" << ntohs(servrAdd.sin_port) << std::endl;

    // Призываем сокета сервера
    if(bind(srvSoket, (struct sockaddr*)&servrAdd, sizeof(servrAdd)) == -1){
        perror("Error bind");
        close(srvSoket);
        return;
    }

    // Устанавливаем очередь ожидания подключений (максимум 5 клиентов)
    if(listen(srvSoket, 5) == -1){
        perror("Error listen");
        close(srvSoket);
        return;
    }

    // Соединение с клиентом
    while(true){
        socklen_t sizeServrAdd = sizeof(servrAdd);
        int clientSoket = accept(srvSoket, (struct sockaddr*)&servrAdd, &sizeServrAdd);
        if(clientSoket == -1){
            perror("Error accept");
            close(srvSoket);
            continue;       
        }
        std::thread(dateClient, clientSoket).detach(); // Создаем новый поток для клиента
    }

    // Закрывалем сокер сервера (НО! она не выполнится никогда так как у нас бесконечный цикл)
    close(srvSoket);
}

int main(){
    srvTcp();
}
