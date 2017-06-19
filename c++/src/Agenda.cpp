// MegaShow
#include <iostream>
#include <algorithm>
#include <csignal>
#include "AgendaUI.hpp"

AgendaUI ui;

void sign_int(int sign);

int main() {
    signal(SIGINT, sign_int);
    ui.OperationLoop();
}

void sign_int(int sign) {
    ui.quitAgendaWithSIGINT(sign);
}

/*int main() {

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

    //s->deleteUser([](const User &u)->bool { return true; });
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
}*/
