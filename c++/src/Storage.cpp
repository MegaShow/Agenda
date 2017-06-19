// MegaShow
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include "Storage.hpp"
#include "Path.hpp"

#include <iostream>


std::shared_ptr<Storage> Storage::m_instance(nullptr);

Storage::Storage() : m_dirty(false){
    readFromFile();
}


bool Storage::readFromFile(void) {
    std::string str;
    std::ifstream fin(Path::userPath, std::ios::in);
    if (!fin.is_open()) {
        return false;
    }
    while (!fin.eof()) {
        std::string info[4];
        // name, psw, email, phone;
        // fin >> str;
        std::getline(fin, str);
        if (str.size() == 0) {
            continue;
        }
        size_t pos = 1, pos_end;
        for (int i = 0; i < 4; i++) {
            pos_end = str.find('\"', pos);
            info[i] = str.substr(pos, pos_end - pos);
            pos = pos_end + 3;
        }
        m_userList.push_back(User(info[0], info[1], info[2], info[3]));
    }
    fin.close();

    fin.open(Path::meetingPath, std::ios::in);
    if (!fin.is_open()) {
        return false;
    }
    while (!fin.eof()) {
        std::string info[5];
        // sponsor, part, startTime, endTime, title;
        // fin >> str;
        std::getline(fin, str);
        if (str.size() == 0) {
            continue;
        }
        size_t pos = 1, pos_end;
        for (int i = 0; i < 5; i++) {
            pos_end = str.find('\"', pos);
            info[i] = str.substr(pos, pos_end - pos);
            pos = pos_end + 3;
        }
        std::vector<std::string> parts;
        pos = pos_end = 0;
        while ((pos_end = info[1].find('&', pos)) != std::string::npos) {
            parts.push_back(info[1].substr(pos, pos_end - pos));
            pos = pos_end + 1;
        }
        parts.push_back(info[1].substr(pos));
        m_meetingList.push_back(Meeting(info[0], parts, info[2], info[3], info[4]));
    }
    fin.close();
    return true;
}


bool Storage::writeToFile(void) {
    bool flag = false;
    // user
    std::ofstream fout(Path::userPath, std::ios::out | std::ios::trunc);
    if (!fout.is_open()) {
        return false;
    }
    for (auto& i: m_userList) {
        if (flag) {
            fout << std::endl;
        } else {
            flag = true;
        }
        fout << '\"' << i.getName() 
              << "\",\"" << i.getPassword()
              << "\",\"" << i.getEmail()
              << "\",\"" << i.getPhone() << '\"';
    }
    fout.close();
    
    // meeting
    fout.open(Path::meetingPath, std::ios::out | std::ios::trunc);
    if (!fout.is_open()) {
        return false;
    }
    flag = false;
    for (auto& i: m_meetingList) {
        if (flag) {
            fout << std::endl;
        } else {
            flag = true;
        }
        std::string str;
        if (i.getParticipator().size()) {
            str = i.getParticipator()[0];
        }
        for (size_t k = 1; k < i.getParticipator().size(); k++) {
            str = str + '&' + i.getParticipator()[k];
        }
        fout << '\"' << i.getSponsor()
             << "\",\"" << str 
             << "\",\"" << Date::dateToString(i.getStartDate())
             << "\",\"" << Date::dateToString(i.getEndDate())
             << "\",\"" << i.getTitle() << '\"';
    }
    fout.close();
    return true;
}


std::shared_ptr<Storage> Storage::getInstance(void) {
    if (m_instance == nullptr) {
        m_instance = std::shared_ptr<Storage>(new Storage());
    }
    return m_instance;
}


Storage::~Storage() {
    sync();
    m_instance = nullptr;
}



void Storage::createUser(const User &t_user) {
    m_dirty = true;
    m_userList.push_back(t_user);
}


std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
    std::list<User> tmp;
    for (auto& i: m_userList) {
        if (filter(i)) {
            tmp.push_back(i);
        }
    }
    return tmp;
}


int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher) {
    int count = 0;
    for (auto& i: m_userList) {
        if (filter(i)) {
            switcher(i);
            count++;
            m_dirty = true;
        }
    }
    return count;
}


int Storage::deleteUser(std::function<bool(const User &)> filter) {
    int count = 0;
    auto i = m_userList.begin();
    while (i != m_userList.end()) {
        if (filter(*i)) {
            auto tmp = i;
            i++;
            m_userList.erase(tmp);
            count++;
            m_dirty = true;
        } else {
            i++;
        }
    }
    return count;
}


void Storage::createMeeting(const Meeting &t_meeting) {
    m_dirty = true;
    m_meetingList.push_back(t_meeting);
}


std::list<Meeting> Storage::queryMeeting(
      std::function<bool(const Meeting &)> filter) const {
    std::list<Meeting> tmp;
    for (auto& i: m_meetingList) {
        if (filter(i)) {
            tmp.push_back(i);
        }
    }
    return tmp;
}


int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher) {
    int count = 0;
    for (auto& i: m_meetingList) {
        if (filter(i)) {
            switcher(i);
            count++;
            m_dirty = true;
        }
    }
    return count;
}


int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
    int count = 0;
    auto i = m_meetingList.begin();
    while (i != m_meetingList.end()) {
        if (filter(*i)) {
            auto tmp = i;
            i++;
            m_meetingList.erase(tmp);
            count++;
            m_dirty = true;
        } else {
            i++;
        }
    }
    return count;
}


bool Storage::sync(void) {
    if (m_dirty) {
        m_dirty = false;
        return writeToFile();
    } else {
        return false;
    }
}


// ===TEST===
/*#ifdef MEGACODE
#include <iostream>
#include <algorithm>
int main() {

    // Test readFromFile
    std::shared_ptr<Storage> s = Storage::getInstance();
    s->updateUser([](const User&){ return true; }, [](User& u){
        std::cout << u.getName() << ' ' << u.getPassword() << ' ' << u.getEmail()
                  << ' ' << u.getPhone() << std::endl;
    });
    std::shared_ptr<Storage> a = Storage::getInstance();
    if (a == s) {
        std::cout << std::endl << "Instance True" << std::endl << std::endl;
    }

    s->deleteUser([](const User &u)->bool { return true; });
    s->updateUser([](const User&){ return true; }, [](User& u){
        std::cout << u.getName() << ' ' << u.getPassword() << ' ' << u.getEmail()
                  << ' ' << u.getPhone() << std::endl;
    });
    std::cout << std::endl;

    // Test createUser
    s->createUser(User("zhenly", "123456", "zhenly.cn", "534"));
    s->updateUser([](const User&){ return true; }, [](User& u){
        std::cout << u.getName() << ' ' << u.getPassword() << ' ' << u.getEmail()
                  << ' ' << u.getPhone() << std::endl;
    });
    std::cout << std::endl;
    s->createUser(User("mega", "123456", "icytown.com", "000"));
    s->updateUser([](const User&){ return true; }, [](User& u){
        std::cout << u.getName() << ' ' << u.getPassword() << ' ' << u.getEmail()
                  << ' ' << u.getPhone() << std::endl;
    });
    std::cout << std::endl;

    // Test writeToFile
    std::cout << "Sync: " << s->sync() << std::endl << std::endl;
    s->updateUser([](const User&){ return true; }, [](User& u){
        std::cout << u.getName() << ' ' << u.getPassword() << ' ' << u.getEmail()
                  << ' ' << u.getPhone() << std::endl;
    });
    std::cout << std::endl;

    // Test queryUser
    std::list<User> li = s->queryUser([](const User& u)->bool {
        return u.getPassword() == "123456";
    });
    for (auto& u: li) {
        std::cout << u.getName() << ' ' << u.getPassword() << ' ' << u.getEmail()
                  << ' ' << u.getPhone() << std::endl;
    }
    std::cout << std::endl;

    // Test updateUser
    s->updateUser([](const User& u)->bool {
        return u.getPassword() == "123456";
    }, [](User& u) {
        u.setEmail("sysu.edu.cn");
    });
    s->updateUser([](const User&)->bool{ return true; }, [](User& u){
        std::cout << u.getName() << ' ' << u.getPassword() << ' ' << u.getEmail()
                  << ' ' << u.getPhone() << std::endl;
    });
    std::cout << std::endl;

    // Test deleteUser
    s->deleteUser([](const User &u)->bool { return u.getName() == "ha"; });


    // Test meeting
    s->updateMeeting([](const Meeting&){ return true; }, [](Meeting& i){
        std::string str = i.getParticipator()[0];
        for (size_t k = 1; k < i.getParticipator().size(); k++) {
            str = str + '&' + i.getParticipator()[k];
        }
        std::cout << '\"' << i.getSponsor()
             << "\",\"" << str 
             << "\",\"" << Date::dateToString(i.getStartDate())
             << "\",\"" << Date::dateToString(i.getEndDate())
             << "\",\"" << i.getTitle() << '\"' << std::endl;
    });
    std::cout << std::endl;
    
    // Test createMeeting
    s->createMeeting(Meeting("mega", std::vector<std::string>{"zhenly","a"},
        Date("2017-06-08/15:03"), Date("2017-09-30/00:00"), "sysu"));
    s->createMeeting(Meeting("zhenly", std::vector<std::string>{"mega"},
        Date("2017-06-08/15:60"), Date("2017-12-31/23:59"), "xm"));
    s->createMeeting(Meeting("zhenly", std::vector<std::string>(),
        Date("2017-06-08/15:60"), Date("2017-12-31/23:59"), "dalao"));
    s->updateMeeting([](const Meeting&){ return true; }, [](Meeting& i){
        std::string str;
        if (i.getParticipator().size()) {
            str = i.getParticipator()[0];
        }
        for (size_t k = 1; k < i.getParticipator().size(); k++) {
            str = str + '&' + i.getParticipator()[k];
        }
        std::cout << '\"' << i.getSponsor()
             << "\",\"" << str 
             << "\",\"" << Date::dateToString(i.getStartDate())
             << "\",\"" << Date::dateToString(i.getEndDate())
             << "\",\"" << i.getTitle() << '\"' << std::endl;
    });
    std::cout << std::endl;

    // Test queryMeeting
    std::list<Meeting> l = s->queryMeeting([](const Meeting& m)->bool {
        auto i = m.getParticipator();
        return m.getSponsor() == "zhenly" || std::find(i.begin(),i.end(),std::string("zhenly")) != i.end();
    });
    for (auto& i: l){
        std::string str;
        if (i.getParticipator().size()) {
            str = i.getParticipator()[0];
        }
        for (size_t k = 1; k < i.getParticipator().size(); k++) {
            str = str + '&' + i.getParticipator()[k];
        }
        std::cout << '\"' << i.getSponsor()
             << "\",\"" << str 
             << "\",\"" << Date::dateToString(i.getStartDate())
             << "\",\"" << Date::dateToString(i.getEndDate())
             << "\",\"" << i.getTitle() << '\"' << std::endl;
    }
    std::cout << std::endl;

    // Test updateMeeting
    s->updateMeeting([](const Meeting& m)->bool {
        auto i = m.getParticipator();
        return m.getSponsor() == "zhenly" || std::find(i.begin(),i.end(),std::string("zhenly")) != i.end();
    }, [](Meeting& m) {
        m.setTitle("error!");
    });
    s->updateMeeting([](const Meeting&){ return true; }, [](Meeting& i){
        std::string str;
        if (i.getParticipator().size()) {
            str = i.getParticipator()[0];
        }
        for (size_t k = 1; k < i.getParticipator().size(); k++) {
            str = str + '&' + i.getParticipator()[k];
        }
        std::cout << '\"' << i.getSponsor()
             << "\",\"" << str 
             << "\",\"" << Date::dateToString(i.getStartDate())
             << "\",\"" << Date::dateToString(i.getEndDate())
             << "\",\"" << i.getTitle() << '\"' << std::endl;
    });
    std::cout << std::endl;

    // Test Sync
    std::cout << "Sync: " << s->sync() << std::endl << std::endl;
    std::cout << "Sync: " << s->sync() << std::endl << std::endl;

    // Test deleteMeeting
    s->deleteMeeting([](const Meeting& m)->bool {
        auto i = m.getParticipator();
        return m.getSponsor() == "zhenly";
    });
    s->updateMeeting([](const Meeting&){ return true; }, [](Meeting& i){
        std::string str;
        if (i.getParticipator().size()) {
            str = i.getParticipator()[0];
        }
        for (size_t k = 1; k < i.getParticipator().size(); k++) {
            str = str + '&' + i.getParticipator()[k];
        }
        std::cout << '\"' << i.getSponsor()
             << "\",\"" << str 
             << "\",\"" << Date::dateToString(i.getStartDate())
             << "\",\"" << Date::dateToString(i.getEndDate())
             << "\",\"" << i.getTitle() << '\"' << std::endl;
    });
    std::cout << std::endl;

    s->deleteMeeting([](const Meeting& m)->bool {
        auto i = m.getParticipator();
        return m.getSponsor() == "maomao";
    });
    s->updateMeeting([](const Meeting&){ return true; }, [](Meeting& i){
        std::string str;
        if (i.getParticipator().size()) {
            str = i.getParticipator()[0];
        }
        for (size_t k = 1; k < i.getParticipator().size(); k++) {
            str = str + '&' + i.getParticipator()[k];
        }
        std::cout << '\"' << i.getSponsor()
             << "\",\"" << str 
             << "\",\"" << Date::dateToString(i.getStartDate())
             << "\",\"" << Date::dateToString(i.getEndDate())
             << "\",\"" << i.getTitle() << '\"' << std::endl;
    });
    std::cout << std::endl;
}
#endif*/

