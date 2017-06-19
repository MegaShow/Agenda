// MegaShow
#include <iostream>
#include <cmath>
#include <csignal>
#include <sstream>
#include "AgendaLinuxPro.hpp"
#include "AgendaUI.hpp"
using std::cout;
using std::cin;
using std::endl;


AgendaUI::AgendaUI() {
    startAgenda();
}

void AgendaUI::quitAgendaWithSIGINT(int sign) {
    quitAgenda();
    exit(0);
}

void AgendaUI::OperationLoop(void) {
    bool flag = true;
    bool helpFirst = true;
    while (flag) {
        if (helpFirst) {
            listHelp();
            helpFirst = false;
        }
        flag = executeOperation(getOperation());
    }
    AgendaLinuxPro::setIONormal();
}


void AgendaUI::listHelp() {
    if (m_userName.size() == 0) {
        // no login status
        AgendaLinuxPro::drawLine("Agenda", true, '-');
        cout << "Action:" << endl;
        cout << "l    - log in Agenda by username and password" << endl;
        cout << "r    - register an Agenda account" << endl;
        cout << "q    - quit Agenda" << endl;
        cout << "h    - help" << endl;
        AgendaLinuxPro::drawLine('-');
    } else {
        // login status
        AgendaLinuxPro::drawLine("Agenda", true, '-');
        cout << "o    - log out Agenda" << endl;
        cout << "dc   - delete Agenda account" << endl;
        cout << "lu   - list all Agenda user" << endl;
        cout << "cm   - create a meeting" << endl;
        cout << "amp  - add meeting participator" << endl;
        cout << "rmp  - remove meeting participator" << endl;
        cout << "rqm  - request to quit meeting" << endl;
        cout << "la   - list all meetings" << endl;
        cout << "las  - list all sponsor meetings" << endl;
        cout << "lap  - list all participator meeting" << endl;
        cout << "qm   - query meeting by title" << endl;
        cout << "qt   - query meeting by time interval" << endl;
        cout << "dm   - delete meeting by title" << endl;
        cout << "da   - delete all meetings" << endl;
        cout << "h    - help" << endl;
        AgendaLinuxPro::drawLine('-');
    }
    cout << endl;
}

  /**
   * constructor
   */
void AgendaUI::startAgenda(void) {
    m_agendaService.startAgenda();
    AgendaLinuxPro::clear();
    AgendaLinuxPro::setIOReverse();
    AgendaLinuxPro::setColor(AgendaLinuxPro::Yellow);
    AgendaLinuxPro::drawLine('=');
    AgendaLinuxPro::drawLine("Welcome to Agenda!", true);
    AgendaLinuxPro::drawLine('=');
    AgendaLinuxPro::setIONormal();
    AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
}


std::string AgendaUI::getOperation() {
    AgendaLinuxPro::setColor(AgendaLinuxPro::Yellow);
    cout << "Agenda";
    AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    if (m_userName.size()) {
        cout << "@";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Green);
        cout << m_userName;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
        cout << " :~# ";
    } else {
        cout << " :~$ ";
    }
    std::string str;
    /*char c = cin.get();
    std::getline(cin, str);
    std::stringstream ss;
    ss << c << str;
    ss >> str;
    for (auto i = str.begin(); i != str.end(); i++) {
        if (*i == ' ') {
            i = str.erase(i);
            i--;
        }
    }*/
    cin >> str;
    return str;
}


bool AgendaUI::executeOperation(const std::string &t_operation) {
    if (t_operation.size() == 0) {
        return true;
    }
    if (m_userName.size() == 0) {
        if (t_operation == "q") {
            quitAgenda();
            return false;
        } else if (t_operation == "l") {
            userLogIn();
        } else if (t_operation == "r") {
            userRegister();
        } else if (t_operation == "h") {
            listHelp();
        } else {
            AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
            cout << "Invalid Operation." << endl;
            AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
        }
    } else {
        if (t_operation == "o") {
            userLogOut();
        } else if (t_operation == "dc") {
            deleteUser();
        } else if (t_operation == "lu") {
            listAllUsers();
        } else if (t_operation == "cm") {
            createMeeting();
        } else if (t_operation == "amp") {
            addMeetingParticipator();
        } else if (t_operation == "rmp") {
            removeMeetingParticipator();
        } else if (t_operation == "rqm") {
            quitMeeting();
        } else if (t_operation == "la") {
            listAllMeetings();
        } else if (t_operation == "las") {
            listAllSponsorMeetings();
        } else if (t_operation == "lap") {
            listAllParticipateMeetings();
        } else if (t_operation == "qm") {
            queryMeetingByTitle();
        } else if (t_operation == "qt") {
            queryMeetingByTimeInterval();
        } else if (t_operation == "dm") {
            deleteMeetingByTitle();
        } else if (t_operation == "da") {
            deleteAllMeetings();
        } else if (t_operation == "h") {
            listHelp();
        } else {
            AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
            cout << "Invalid Operation." << endl;
            AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
        }
    }
    return true;
}


void AgendaUI::userLogIn(void) {
    cout << "[log in] [username]" << endl;
    cout << "[log in] ";
    std::string username, psw;
    cin >> username;
    cout << "[log in] [password]" << endl;
    cout << "[log in] ";
    AgendaLinuxPro::setIONone();
    cin >> psw;
    AgendaLinuxPro::setIONormal();
    if (m_agendaService.userLogIn(username, psw)) {
        m_userName = username;
        m_userPassword = psw;
        cout << "[log in] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Green);
        cout << "Succeed!" << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    } else {
        cout << "[log in] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
        cout << "Password error or user don't exist." << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    }
    cout << endl;
}


void AgendaUI::userRegister(void) {
    cout << "[register] [username]" << endl;
    cout << "[register] ";
    std::string username, psw, email, phone;
    cin >> username;
    cout << "[register] [password]" << endl;
    cout << "[register] ";
    AgendaLinuxPro::setIONone();
    cin >> psw;
    AgendaLinuxPro::setIONormal();
    cout << "[register] [email] [phone]" << endl;
    cout << "[register] ";
    cin >> email >> phone;
    if (m_agendaService.userRegister(username, psw, email, phone)) {
        cout << "[register] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Green);
        cout << "Succeed!" << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    } else {
        cout << "[register] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
        cout << "This username has been register." << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    }
    cout << endl;
}


void AgendaUI::userLogOut(void) {
    cout << "[log out]" << endl << endl;
    m_userName.clear();
    m_userPassword.clear();
}

  /**
   * quit the Agenda
   */
void AgendaUI::quitAgenda(void) {
    cout << "[quit]" << endl << endl;
    AgendaLinuxPro::setIOReverse();
    AgendaLinuxPro::setColor(AgendaLinuxPro::Yellow);
    AgendaLinuxPro::drawLine('-');
    AgendaLinuxPro::drawLine("Quit Agenda!", true);
    AgendaLinuxPro::drawLine('-');
    AgendaLinuxPro::setIONormal();
    AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
}

  /**
   * delete a user from storage
   */
void AgendaUI::deleteUser(void) {
    if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
        m_userName.clear();
        m_userPassword.clear();
        cout << "[delete Agenda account] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Green);
        cout << "Succeed!" << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    } else {
        cout << "[delete Agenda account] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
        cout << "Failed!" << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    }
    cout << endl;
}

  /**
   * list all users from storage
   */
void AgendaUI::listAllUsers(void) {
    cout << "[list all users]" << endl;
    AgendaLinuxPro::setColor(AgendaLinuxPro::Blue);
    AgendaLinuxPro::drawLine('+');
    AgendaLinuxPro::drawTable(std::vector<std::string>{"name", "email", "phone"});
    AgendaLinuxPro::drawLine('+');
    AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    std::list<User> u = m_agendaService.listAllUsers();
    for (auto& i: u) {
        AgendaLinuxPro::drawTable(std::vector<std::string>{i.getName(), i.getEmail(), i.getPhone()});
    }
    AgendaLinuxPro::drawLine('-');
    cout << endl;
}

  /**
   * user create a meeting with someone else
   */
void AgendaUI::createMeeting(void) {
    std::vector<std::string> part;
    std::string str, s, e;
    cout << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    int var;
    cin >> var;
    for (int i = 0; i < var; i++) {
        cout << "[create meeting] [please enter the participator " << i << " ]" << endl;
        cout << "[create meeting] ";
        cin >> str;
        part.push_back(str);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    cin >> str >> s >> e;
    if (m_agendaService.createMeeting(m_userName, str, s, e, part)) {
        cout << "[create meeting] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Green);
        cout << "Succeed!" << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    } else {
        cout << "[create meeting] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
        cout << "Create meeting failed." << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    }
    cout << endl;
}

  /**
   * sponsor add a participator to the meeting
   */
void AgendaUI::addMeetingParticipator(void) {
    std::string title, name;
    cout << "[add participator] [meeting title] [participator username]" << endl;
    cout << "[add participator] ";
    cin >> title >> name;
    if (m_agendaService.addMeetingParticipator(m_userName, title, name)) {
        cout << "[add participator] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Green);
        cout << "Succeed!" << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    } else {
        cout << "[add participator] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
        cout << "Add participator failed." << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    }
    cout << endl;
}

  /**
   * sponsor add a participator to the meeting
   */
void AgendaUI::removeMeetingParticipator(void) {
    std::string title, name;
    cout << "[remove participator] [meeting title] [participator username]" << endl;
    cout << "[remove participator] ";
    cin >> title >> name;
    if (m_agendaService.removeMeetingParticipator(m_userName, title, name)) {
        cout << "[remove participator] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Green);
        cout << "Succeed!" << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    } else {
        cout << "[remove participator] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
        cout << "Remove participator failed." << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    }
    cout << endl;
}

  /**
   * user quit from meeting
   */
void AgendaUI::quitMeeting(void) {
    std::string title;
    cout << "[quit meeting] [meeting title]" << endl;
    cout << "[quit meeting] ";
    cin >> title;
    if (m_agendaService.quitMeeting(m_userName, title)) {
        cout << "[quit meeting] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Green);
        cout << "Succeed!" << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    } else {
        cout << "[quit meeting] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
        cout << "Quit meeting failed." << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    }
    cout << endl;
}

  /**
   * list all meetings from storage
   */
void AgendaUI::listAllMeetings(void) {
    cout << "[list all meeting]" << endl;
    printMeetings(m_agendaService.listAllMeetings(m_userName));
}

  /**
   * list all meetings that this user sponsored
   */
void AgendaUI::listAllSponsorMeetings(void) {
    cout << "[list all sponsor meeting]" << endl;
    printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}

  /**
   * list all meetings that this user take part in
   */
void AgendaUI::listAllParticipateMeetings(void) {
    cout << "[list all meeting]" << endl;
    printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
}

  /**
   * search meetings by title from storage
   */
void AgendaUI::queryMeetingByTitle(void) {
    std::string title;
    cout << "[query meeting] [title]" << endl;
    cout << "[query meeting] ";
    cin >> title;
    printMeetings(m_agendaService.meetingQuery(m_userName, title));
}

  /**
   * search meetings by timeinterval from storage
   */
void AgendaUI::queryMeetingByTimeInterval(void) {
    std::string s, e;
    cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting] ";
    cin >> s >> e;
    printMeetings(m_agendaService.meetingQuery(m_userName, s, e));
}

  /**
   * delete meetings by title from storage
   */
void AgendaUI::deleteMeetingByTitle(void) {
    std::string title;
    cout << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title)) {
        cout << "[delete meeting] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Green);
        cout << "Succeed!" << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    } else {
        cout << "[delete meeting] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
        cout << "Delete meeting failed." << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    }
    cout << endl;
}

  /**
   * delete all meetings that this user sponsored
   */
void AgendaUI::deleteAllMeetings(void) {
    if (m_agendaService.deleteAllMeetings(m_userName)) {
        cout << "[delete all meeting] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Green);
        cout << "Succeed!" << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    } else {
        cout << "[delete all meeting] ";
        AgendaLinuxPro::setColor(AgendaLinuxPro::Red);
        cout << "Delete all meeting failed." << endl;
        AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    }
    cout << endl;
}

  /**
   * show the meetings in the screen
   */
void AgendaUI::printMeetings(const std::list<Meeting> &m) {
    AgendaLinuxPro::setColor(AgendaLinuxPro::Blue);
    AgendaLinuxPro::drawLine('+');
    AgendaLinuxPro::drawTable(std::vector<std::string>{"title", "sponsor", "start time", "end time"});
    cout << "participator" << endl;
    AgendaLinuxPro::drawLine('+');
    AgendaLinuxPro::setColor(AgendaLinuxPro::Write);
    for (auto& i: m) {
        std::string str = i.getParticipator()[0];
        for (std::size_t t = 1; t < i.getParticipator().size(); t++) {
            str = str + ',' + i.getParticipator()[t];
        }
        AgendaLinuxPro::drawTable(std::vector<std::string>{i.getTitle(), i.getSponsor(),
            Date::dateToString(i.getStartDate()), Date::dateToString(i.getEndDate())});
        cout << str << endl;
        AgendaLinuxPro::drawLine('-');
    }
    cout << endl;
}
