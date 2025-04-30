#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#define PORT 8080
#define BUFFER_SIZE 1024
#define SECRET_KEY "1234"

void xor_encrypt_decrypt(std::string &data, const std::string &key) {
    for (size_t i = 0; i < data.size(); ++i)
        data[i] ^= key[i % key.length()];
}

void receive_file_list(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes = read(sock, buffer, BUFFER_SIZE);
    std::string file_list(buffer, bytes);
    xor_encrypt_decrypt(file_list, SECRET_KEY);
    std::cout << "Available files:\n" << file_list;
}

void download_file(int sock, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    char buffer[BUFFER_SIZE];
    int bytes;
    while ((bytes = read(sock, buffer, BUFFER_SIZE)) > 0) {
        std::string chunk(buffer, bytes);
        xor_encrypt_decrypt(chunk, SECRET_KEY);
        file.write(chunk.c_str(), chunk.size());
        if (bytes < BUFFER_SIZE) break;
    }
    file.close();
    std::cout << "Downloaded " << filename << "\n";
}

void upload_file(int sock, const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cout << "File not found.\n";
        return;
    }
    char buffer[BUFFER_SIZE];
    while (!file.eof()) {
        file.read(buffer, BUFFER_SIZE);
        int read_size = file.gcount();
        std::string chunk(buffer, read_size);
        xor_encrypt_decrypt(chunk, SECRET_KEY);
        send(sock, chunk.c_str(), chunk.size(), 0);
    }
    file.close();
    std::cout << "Uploaded " << filename << "\n";
}

int main() {
    int sock;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // Authentication
    std::string username, password;
    std::cout << "Username: "; std::cin >> username;
    std::cout << "Password: "; std::cin >> password;
    std::string credentials = username + ":" + password;
    xor_encrypt_decrypt(credentials, SECRET_KEY);
    send(sock, credentials.c_str(), credentials.size(), 0);

    char response[BUFFER_SIZE] = {0};
    read(sock, response, BUFFER_SIZE);
    std::string reply(response);
    xor_encrypt_decrypt(reply, SECRET_KEY);

    if (reply == "AUTH_FAIL") {
        std::cout << "Authentication failed.\n";
        return 1;
    } else if (reply != "AUTH_OK") {
        std::cout << "Unexpected server response.\n";
        return 1;
    }

    std::cout << "\n1. List Files\n2. Download File\n3. Upload File\nChoice: ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        std::string cmd = "LIST";
        xor_encrypt_decrypt(cmd, SECRET_KEY);
        send(sock, cmd.c_str(), cmd.size(), 0);
        receive_file_list(sock);

    } else if (choice == 2) {
        std::string filename;
        std::cout << "Filename to download: "; std::cin >> filename;
        std::string cmd = "DOWNLOAD:" + filename;
        xor_encrypt_decrypt(cmd, SECRET_KEY);
        send(sock, cmd.c_str(), cmd.size(), 0);
        download_file(sock, filename);

    } else if (choice == 3) {
        std::string filename;
        std::cout << "Filename to upload: "; std::cin >> filename;
        std::string cmd = "UPLOAD:" + filename;
        xor_encrypt_decrypt(cmd, SECRET_KEY);
        send(sock, cmd.c_str(), cmd.size(), 0);
        upload_file(sock, filename);
    }

    close(sock);
    return 0;
}

