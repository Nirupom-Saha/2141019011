#include <iostream>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <netinet/in.h>
#include <unordered_map>

#define PORT 8080
#define BUFFER_SIZE 1024
#define SHARED_DIR "./shared/"
#define SECRET_KEY "1234"

void xor_encrypt_decrypt(std::string &data, const std::string &key) {
    for (size_t i = 0; i < data.size(); ++i)
        data[i] ^= key[i % key.length()];
}

bool authenticate(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    read(client_socket, buffer, BUFFER_SIZE);
    std::string credentials(buffer);
    xor_encrypt_decrypt(credentials, SECRET_KEY);

    std::unordered_map<std::string, std::string> users = {
        {"user", "pass"}, {"admin", "admin123"}
    };

    std::string user = credentials.substr(0, credentials.find(":"));
    std::string pass = credentials.substr(credentials.find(":") + 1);

    return users[user] == pass;
}

void send_file_list(int client_socket) {
    DIR* dir = opendir(SHARED_DIR);
    std::string file_list;
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG)
                file_list += entry->d_name + std::string("\n");
        }
        closedir(dir);
    } else file_list = "No files found.\n";

    xor_encrypt_decrypt(file_list, SECRET_KEY);
    send(client_socket, file_list.c_str(), file_list.size(), 0);
}

void send_file(int client_socket, const std::string &filename) {
    std::ifstream file(SHARED_DIR + filename, std::ios::binary);
    if (!file) return;

    char buffer[BUFFER_SIZE];
    while (!file.eof()) {
        file.read(buffer, BUFFER_SIZE);
        int read_size = file.gcount();
        std::string chunk(buffer, read_size);
        xor_encrypt_decrypt(chunk, SECRET_KEY);
        send(client_socket, chunk.c_str(), chunk.size(), 0);
    }
    file.close();
}

void receive_file(int client_socket, const std::string &filename) {
    std::ofstream file(SHARED_DIR + filename, std::ios::binary);
    char buffer[BUFFER_SIZE];
    int bytes;
    while ((bytes = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        std::string chunk(buffer, bytes);
        xor_encrypt_decrypt(chunk, SECRET_KEY);
        file.write(chunk.c_str(), chunk.size());
        if (bytes < BUFFER_SIZE) break;
    }
    file.close();
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        std::cout << "Client connected.\n";

        if (!authenticate(client_socket)) {
            std::string fail = "AUTH_FAIL";
            xor_encrypt_decrypt(fail, SECRET_KEY);
            send(client_socket, fail.c_str(), fail.size(), 0);
            close(client_socket);
            continue;
        }

        // Send AUTH_OK
        std::string success = "AUTH_OK";
        xor_encrypt_decrypt(success, SECRET_KEY);
        send(client_socket, success.c_str(), success.size(), 0);

        char buffer[BUFFER_SIZE] = {0};
        read(client_socket, buffer, BUFFER_SIZE);
        std::string command(buffer);
        xor_encrypt_decrypt(command, SECRET_KEY);

        if (command == "LIST") {
            send_file_list(client_socket);
        } else if (command.rfind("DOWNLOAD:", 0) == 0) {
            send_file(client_socket, command.substr(9));
        } else if (command.rfind("UPLOAD:", 0) == 0) {
            receive_file(client_socket, command.substr(7));
        }

        close(client_socket);
    }

    close(server_fd);
    return 0;
}

