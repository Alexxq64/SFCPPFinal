#include "chatDB.h"
#include "globalsettings.h"
//#include <string>




bool openDB() {
    // Создаем подключение к базе данных SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/ChatDB/ChatDB.sqlite"); // Укажите путь к вашей базе данных SQLite

    // Открываем соединение с базой данных
    if (!db.open()) {
        qDebug() << "Ошибка при открытии базы данных: " << db.lastError().text();
        return false;
    }

    qDebug() << "База данных успешно открыта";
    globalString = "openDB()";



//    SetConsoleOutputCP(CP_UTF8);
    getMessages();
    globalString = globalString + "\ngetMessages()";
    clearChat();
    globalString = globalString + "\nclearChat()";
    getAllClients();
    globalString = globalString + "\ngetAllClients()";
    getSignedInClients();
    return true;
}


void clearChat() {
    QSqlQuery query;
    query.prepare("UPDATE Users SET LoggedIn = 0");
    if (!query.exec()) {
        qDebug() << "Ошибка при изменении статуса: " << query.lastError().text();
    }
}

void fixAddress(std::string cName, std::string IP, int port) {
//    std::stringstream query;
//    query << "UPDATE users SET signed_in = 1, IP = '" << IP << "', port = " << port << " WHERE username = '" << cName << "'";
//    if (mysql_query(&mysql, query.str().c_str()) != 0) {
//        std::cout << "Ошибка при изменении статуса: " << mysql_error(&mysql) << std::endl;
//    }
}

void insertNewClient(const std::string cName, const std::string IP, int port) {
//    // Insert new client data into the 'users' table in the database
//    std::string insertQuery = "INSERT INTO users (username, ip, port, password) VALUES ('" + cName + "', '" + IP + "', " + std::to_string(port) + ", '" + cName + "')";
//    mysql_query(&mysql, insertQuery.c_str());
}

void getMessages() {
    QSqlQuery query;
    query.prepare("SELECT * FROM Messages");
    if (query.exec()) {
        while (query.next()) {
            int id = query.value("ID").toInt();
            QString date = query.value("Date").toString();
            int fromUserID = query.value("FromUserID").toInt();
            int toUserID = query.value("ToUserID").toInt();
            QString text = query.value("Text").toString();

            qDebug() << "ID:" << id << " Date:" << date << " FromUserID:" << fromUserID << " ToUserID:" << toUserID << " Text:" << text;
        }
    } else {
        qDebug() << "Ошибка при выполнении запроса: " << query.lastError().text();
    }
}

QString getSignedInClients() {
    QSqlQuery query("SELECT Name FROM Users WHERE LoggedIn = 1");
    QString list = "\nThere are in the chat now:  \n";

    if (query.exec()) {
        int thereAreInTheChatNow = 0;
        while (query.next()) {
            thereAreInTheChatNow++;
            list += query.value("Name").toString() + ", ";
        }

        if (thereAreInTheChatNow > 0) {
            list.chop(2); // Убираем последнюю запятую и пробел
            list += '.';
        } else {
            list = "There is nobody in the chat now.";
        }
    } else {
        list = "Ошибка при выполнении запроса: " + query.lastError().text();
    }

    qDebug() << list;
    return list;
}

void getAllClients() {
    QSqlQuery query("SELECT * FROM Users");

    if (query.exec()) {
        qDebug() << "Список пользователей:";
        while (query.next()) {
            int id = query.value("ID").toInt();
            QString name = query.value("Name").toString();
            QString password = query.value("Password").toString();
            QString ip = query.value("IP").toString();
            int port = query.value("Port").toInt();
            int loggedIn = query.value("LoggedIn").toInt();
            int banned = query.value("Banned").toInt();

            qDebug() << "ID:" << id << " Name:" << name << " Password:" << password << " IP:" << ip << " Port:" << port << " LoggedIn:" << loggedIn << " Banned:" << banned;
        }
    } else {
        qDebug() << "Ошибка при выполнении запроса: " << query.lastError().text();
    }
}


bool isSignedUp(const std:: string& name) {
//    std::string query = "SELECT * FROM users WHERE username = '" + name + "'";
//    if (mysql_query(&mysql, query.c_str()) == 0) {
//        res = mysql_store_result(&mysql);
//        if (res) {
//            return mysql_num_rows(res) > 0;
//        }
//    }
    return false;
}

bool isSignedIn(const std::string& name) {
//    std::string query = "SELECT * FROM users WHERE username = '" + name + "' AND signed_in = 1";
//    if (mysql_query(&mysql, query.c_str()) == 0) {
//        res = mysql_store_result(&mysql);
//        if (res) {
//            return mysql_num_rows(res) > 0;
//        }
//    }
    return false;
}


void closeBD()
{
//    // Закрываем соединение с сервером базы данных
//    mysql_free_result(res);
//    mysql_close(&mysql);
}

bool verifyPw(const std::string& name, const std::string& password) {
//    std::string query = "SELECT password FROM users WHERE username = '" + name + "'";
//    if (mysql_query(&mysql, query.c_str()) == 0) {
//        res = mysql_store_result(&mysql);
//        if (res) {
//            if (mysql_num_rows(res) > 0) {
//                // Если имя пользователя найдено, проверяем пароль
//                MYSQL_ROW row = mysql_fetch_row(res);
//                if (row[0] == password) {
//                    std::string query = "UPDATE users SET signed_in = 1 WHERE username = '" + name + "'";
//                    if (mysql_query(&mysql, query.c_str()) != 0) {
//                        std::cout << "Ошибка при изменении статуса: " << mysql_error(&mysql) << std::endl;
//                    }
//                    return true; // Пароль совпадает
//                }
//            }
//        }
//    }
    return false; // Имя пользователя не найдено или пароль не совпадает
}


void savePw(const std::string& name, const std::string& password) {
//    std::string query = "UPDATE users SET password = '" + password + "' WHERE username = '" + name + "'";
//    if (mysql_query(&mysql, query.c_str()) != 0) {
//        std::cout << "Ошибка при сохранении пароля: " << mysql_error(&mysql) << std::endl;
//    }
//    else {
//        std::string query = "UPDATE users SET signed_in = 1 WHERE username = '" + name + "'";
//        if (mysql_query(&mysql, query.c_str()) != 0) {
//            std::cout << "Ошибка при изменении статуса: " << mysql_error(&mysql) << std::endl;
//        }
//    }
}

std::string getIPfromDB(std::string checkName) {
//    std::string query = "SELECT ip FROM users WHERE username = '" + checkName + "'";
//    mysql_query(&mysql, query.c_str());
//    res = mysql_store_result(&mysql);
//    return mysql_fetch_row(res)[0];
    return "";
}

int getPortFromDB(std::string checkName){
//    std::string query = "SELECT port FROM users WHERE username = '" + checkName + "'";
//    mysql_query(&mysql, query.c_str());
//    res = mysql_store_result(&mysql);
//    return atoi (mysql_fetch_row(res)[0]);
    return 0;
}


int getClientID(std::string client){
//    std::string query = "SELECT user_id FROM users WHERE username = '" + client + "'";
//    mysql_query(&mysql, query.c_str());
//    res = mysql_store_result(&mysql);
//    return atoi(mysql_fetch_row(res)[0]);
    return 0;
}

std::string getNow() {
    std::time_t now = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &now);

    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}


void saveMessageIntoDB(std::string sender, std::string receiver, std::string message) {
//    std::string insertQuery = "INSERT INTO messages (send_date, sender_id, receiver_id, message_text) VALUES ('"
//                              + getNow() + "', " + std::to_string(getClientID(sender)) + ", " + std::to_string(getClientID(receiver)) + ", '" + message + "')";
//    mysql_query(&mysql, insertQuery.c_str());
}

std::string getMessageHistory(std::string currentClientName) {
//    std::stringstream query;
//    query << "SELECT sender_id, receiver_id, send_date, message_text FROM messages WHERE sender_id = '"
//          << getClientID(currentClientName) << "' OR receiver_id = '" << getClientID(currentClientName) << "'";
//    if (mysql_query(&mysql, query.str().c_str()) == 0) {
//        mysql_set_character_set(&mysql, "utf8");
//        MYSQL_RES* result = mysql_store_result(&mysql);
//        if (result) {
//            std::stringstream messageHistory;
//            MYSQL_ROW row;
//            while ((row = mysql_fetch_row(result))) {
//                std::string sender = getNameByID(atoi(row[0]));
//                std::string receiver = getNameByID(atoi(row[1]));
//                std::string sendDate = row[2];
//                std::string messageText = row[3];

//                if (sender == currentClientName) {
//                    messageHistory << ">> " << receiver << " " << sendDate << " " << messageText << "\n";
//                }
//                else {
//                    messageHistory << "<< " << sender << " " << sendDate << " " << messageText << "\n";
//                }
//            }

//            mysql_free_result(result);
//            return messageHistory.str();
//        }
//    }

    return "Error fetching message history.";
}

std::string getNameByID(int id) {
//    std::stringstream query;
//    query << "SELECT username FROM users WHERE user_id = " << id;

//    if (mysql_query(&mysql, query.str().c_str()) == 0) {
//        MYSQL_RES* result = mysql_store_result(&mysql);
//        if (result) {
//            MYSQL_ROW row = mysql_fetch_row(result);
//            if (row) {
//                std::string username = row[0];
//                mysql_free_result(result);
//                return username;
//            }
//        }
//    }

    return "User not found.";
}

std::string getNameByAddress(const std::string IP, int port){
//    std::string query = "SELECT username FROM users WHERE ip = '" + IP + "'" + " AND port = " + std::to_string(port);
//    mysql_query(&mysql, query.c_str());
//    res = mysql_store_result(&mysql);
//    return mysql_fetch_row(res)[0];
    return "";
}
