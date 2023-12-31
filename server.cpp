#include "server.h"
#include "chatDB.h"
//#include "console.h"


std::string getIP(const sockaddr_in& socketAddress) {
    char* ip = inet_ntoa(socketAddress.sin_addr);
    if (ip)
        return std::string(ip);
    return "";
}

int getPort(const sockaddr_in& socketAddress) {
    return ntohs(socketAddress.sin_port);
}

sockaddr_in getAddress(const std::string& IP, int port) {
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP.c_str());
    address.sin_port = htons(port);
    return address;
}

bool sendMessage(const std::string& text, const sockaddr_in& address) {
    if (sendto(serverSocket, text.c_str(), text.size(), 0, (const sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        return false;
    }
    std::cout << "      Sent to " << getIP(address) << " : " << getPort(address) << "  <<  " << text << std::endl;
    return true;
}

bool sendQMessage(const QString& message, const sockaddr_in& address) {
    QUdpSocket udpSocket;
    if (!udpSocket.bind(QHostAddress::AnyIPv4, 0)) {
        qDebug() << "Failed to bind UDP socket:" << udpSocket.errorString();
        return false;
    }

    QByteArray datagram = message.toUtf8();
    qint64 bytesSent = udpSocket.writeDatagram(datagram, QHostAddress(address.sin_addr.s_addr), ntohs(address.sin_port));

    if (bytesSent == -1) {
        qDebug() << "Failed to send UDP datagram:" << udpSocket.errorString();
        return false;
    }

    qDebug() << "Sent to" << QHostAddress(ntohl(address.sin_addr.s_addr)).toString() << ":" << ntohs(address.sin_port) << " << " << message;
    return true;
}

bool startServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return false;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (const sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    std::cout << "Server started on " << getIP(serverAddress) << " : " << getPort(serverAddress) << std::endl;
    return true;
}

void getMessage() {
    memset(buffer, 0, sizeof(buffer));
    int clientAddressSize = sizeof(clientAddress);
    bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddress, &clientAddressSize);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Failed to receive message" << std::endl;
    }
    std::cout << "Received from " << getIP(clientAddress) << " : " << getPort(clientAddress) << "  >>  " << buffer << std::endl;
}

void registerClient(const std::string& cName) {
    currentClientName = cName;
    if (isSignedUp(cName)) {
        sendMessage("C:", clientAddress); // Instruct the client to verify the password
        fixAddress(cName, getIP(clientAddress), getPort(clientAddress));
    }
    else {
        insertNewClient(cName, getIP(clientAddress), getPort(clientAddress));
        sendMessage("P:", clientAddress); // Instruct the client to enter the password
    }
}


bool checkClient(const std::string& checkName) {
    currentClientName = checkName;
    std::string text = "E";
    // if the client is in the chat now
    if (isSignedIn(checkName)) {
        std::cout << checkName << " is in the chat now" << std::endl;
        text = text + ":" + getIPfromDB(checkName) + ":" + std::to_string(getPortFromDB(checkName));
        sendMessage(text, clientAddress);
        receiver = checkName;
        return true;
    }
    // if the client is in the datebase but not in the chat now
    if (isSignedUp(checkName)) {
        std::cout << checkName << " is in the list, but now is not in the chat" << std::endl;
        sendMessage("A", clientAddress);
        return true;
    }
    std::cout << checkName << " is not in the list" << std::endl;
    sendMessage("N:", clientAddress);
    return false;
}

void handleMessage() {
    command = buffer[0];
    text = "";
    text.append(&buffer[2]);
    size_t delimiterPos = 0;
    switch (command) {
    // Registration
    case 'R':
        registerClient(text);
        break;
    // Check if there is this client in the chat now
    case 'C':
        checkClient(text);
        break;
    // Verify password
    case 'V':
        if (!verifyPw(currentClientName, text)) {
            // the wrong password
            sendMessage("B:", clientAddress);
        }
        else {
            // the correct password
            sendMessage("O:", clientAddress);
        }
        break;
    // Save password
    case 'S':
        savePw(currentClientName, text);
        break;
    // Send the list of signed in clients
    case 'L':
        sendQMessage("L:" + getSignedInClients(), clientAddress);
        break;
    // Send message history
    case 'H':
        currentClientName = getNameByAddress(getIP(clientAddress), getPort(clientAddress));
        sendMessage("H:" + getMessageHistory(currentClientName), clientAddress);
        break;
    // Print received message
    case 'M':
        std::cout << "M:" << text << std::endl;
        delimiterPos = text.find(">");

        if (delimiterPos != std::string::npos) {
            sender = text.substr(0, delimiterPos);
            text = text.substr(delimiterPos + 4);
        }
        saveMessageIntoDB(sender, receiver, text);
        logger->writeLog(sender  + " > " + receiver + ": " + text);
        break;
    default:
        break;
    }
    std::cout << "\nEnter a command ('help' to get list of commands): \n";
}


void consoleThread() {
    bool toStop = false;
    std::string command;


    while (!toStop) {
        std::cout << "\nEnter a command (or 'exit' to quit): \n";
        std::getline(std::cin, command);

        if (command == "exit") {
            std::cout << "Bye! See you later!";
            toStop = true;
        }
        else if (command == "last") {
            std::cout << logger->getLastLog() << std::endl;
            //std::vector<std::string> logs = logger.getLastNLogs(10); // get 10 last logs
            //for (const auto& log : logs) {
            //    std::cout << log << std::endl;
        }
        else if (command == "all") {
            std::cout << std::endl;
            std::vector<std::string> allLogs = logger->getAllLogs();
            for (const auto& log : allLogs) {
                std::cout << log << std::endl;
            }
            std::cout << std::endl;

        }
        else if (command == "test") {
            std::cout << "------    Test    --------" << std::endl;
        }
        else if (command == "help") {
            std::cout << "last - to get last log\n";
            std::cout << "all - to get all the logs\n";
            std::cout << "help - to get list of commands\n";
            std::cout << "exit - to quit\n";
        }
        else std::cout << "Wrong input. Try again ('help' to get list of commands):\n";
    }
}


//int main() {
//    if (!openDB()) return -1;
//    if (!startServer()) return -1;
//    logger = new Logger("log.txt");
//    logger->writeLog("Session opened at " + getNow());
//    //logger->initLog();

//    std::thread console(consoleThread);

//    while (true) {
//        getMessage();
//        handleMessage();
//    }

//    WSACleanup();

//    closeBD();
//    console.join();
//    delete logger;

//    return 0;
//}
