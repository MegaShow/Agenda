// MegaShow
#include <string>
#include "User.hpp"


User::User(const std::string &t_userName, const std::string &t_userPassword,
       const std::string &t_userEmail, const std::string &t_userPhone)
    : m_name(t_userName), m_password(t_userPassword), m_email(t_userEmail), m_phone(t_userPhone) {}


User::User(const User &t_user) : m_name(t_user.m_name), m_password(t_user.m_password),
    m_email(t_user.m_email), m_phone(t_user.m_phone) {}


std::string User::getName() const {
    return m_name;
}


void User::setName(const std::string &t_name) {
    m_name = t_name;
}


std::string User::getPassword() const {
    return m_password;
}


void User::setPassword(const std::string &t_password) {
    m_password = t_password;
}


std::string User::getEmail() const {
    return m_email;
}


void User::setEmail(const std::string &t_email) {
    m_email = t_email;
}


std::string User::getPhone() const {
    return m_phone;
}


void User::setPhone(const std::string &t_phone) {
    m_phone = t_phone;
}

// MegaShow
/*#include <string>
#include <iostream>
#include "User.hpp"
using namespace std;

void print(const User& u) {
    cout << "User: " << u.getName() << endl
         << "Psw:  " << u.getPassword() << endl
         << "Email: " << u.getEmail() << endl
         << "Phone: " << u.getPhone() << endl << endl;
}

int main() {
    string name, psw, email, phone;
    string n, pw, e, p;
    cin >> n >> pw >> e >> p;
    User u1;
    User u2(n, pw, e, p);
    print(u1);
    print(u2);
    cin >> name >> psw >> email >> phone;
    u2.setName(name);
    User u3(u2);
    print(u3);
    u3.setPassword(psw);
    u1 = u3;
    print(u1);
    u1.setEmail(email);
    u1.setPhone(phone);
    User u4 = u1;
    print(u4);
}*/

