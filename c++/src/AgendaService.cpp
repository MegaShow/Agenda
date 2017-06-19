// MegaShow
#include "AgendaService.hpp"


AgendaService::AgendaService() : m_storage(nullptr) {
    startAgenda();
}


AgendaService::~AgendaService() {}


bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    std::list<User> lu = m_storage->queryUser([&](const User& u) {
        return u.getName() == userName && u.getPassword() == password;
    });
    return !lu.empty();
}


bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone) {
    std::list<User> lu = m_storage->queryUser([&](const User& u) { return u.getName() == userName; });
    if (lu.empty()) {
        m_storage->createUser(User(userName, password, email, phone));
        return true;
    } else {
        return false;
    }
}


bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    int del = m_storage->deleteUser([&](const User& u) { 
        return u.getName() == userName && u.getPassword() == password; 
    });
    std::list<std::string> title;
    if (del) {
        m_storage->deleteMeeting([&](const Meeting& m) { return m.getSponsor() == userName; });
        m_storage->updateMeeting([&](const Meeting& m) {
            return m.isParticipator(userName);
        }, [&](Meeting& m) {
            m.removeParticipator(userName);
            if (m.getParticipator().size() == 0) {
                title.push_back(m.getTitle());
            }
        });
        for (auto& i: title) {
            m_storage->deleteMeeting([&](const Meeting& m) {
                return m.getTitle() == i;
            });
        }
    }
    return del;
}


std::list<User> AgendaService::listAllUsers(void) const {
    return m_storage->queryUser([](const User& u) { return true; });
}


bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator) {
    std::list<User> slu = m_storage->queryUser([&](const User& u) {
        return u.getName() == userName;
    });
    // if username don't exist
    if (slu.empty()) {
        return false;
    }
    // if participator is null
    if (participator.empty()) {
        return false;
    }
    // if participator don't exist or participator == username
    for (auto& i: participator) {
        std::list<User> lu = m_storage->queryUser([&](const User& u) {
            return u.getName() == i;
        });
        if (userName == i || lu.empty()) {
            return false;
        }
    }
    // if participators are the same
    for (std::size_t i = 0; i < participator.size(); i++) {
        for (std::size_t j = i + 1; j < participator.size(); j++) {
            if (participator[i] == participator[j]) {
                return false;
            }
        }
    }
    Date s(startDate);
    Date e(endDate);
    std::list<Meeting> lm = m_storage->queryMeeting([&](const Meeting& m) {
        return m.getTitle() == title;
    });
    // if start/end time is invalid or start >= end or title exist 
    if (Date::isValid(s) && Date::isValid(e) && (s < e) && lm.empty()) {
        // if username has no time
        std::list<Meeting> in = m_storage->queryMeeting([&](const Meeting& m) {
            return m.getSponsor() == userName || m.isParticipator(userName);
        });
        for (auto& i: in) {
            if (e <= i.getStartDate() || i.getEndDate() <= s) {
                continue;
            } else {
                return false;
            }
        }
        // if participator has no time
        for (auto& j: participator) {
            std::list<Meeting> pm = m_storage->queryMeeting([&](const Meeting& m) {
                return m.getSponsor() == j || m.isParticipator(j);
            });
            for (auto& i: pm) {
                if (e <= i.getStartDate() || i.getEndDate() <= s) {
                    continue;
                } else {
                    return false;
                }
            }
        }
        m_storage->createMeeting(Meeting(userName, participator, s, e, title));
        return true;
    } else {
        return false;
    }
}


bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator) {
    bool flag = true;
    // if usename or participator don't exist
    std::list<User> slu = m_storage->queryUser([&](const User& u) {
        return u.getName() == userName || u.getName() == participator;
    });
    if (slu.size() != 2) {
        return false;
    }
    int add = m_storage->updateMeeting([&](const Meeting& m) {
        return m.getSponsor() == userName && !m.isParticipator(participator)
            && m.getTitle() == title;
    }, [&](Meeting& m) {
        std::list<Meeting> in = m_storage->queryMeeting([&](const Meeting& m) {
            return m.getSponsor() == participator || m.isParticipator(participator);
        });
        // if participator has no time
        for (auto& i: in) {
            if (m.getEndDate() <= i.getStartDate() || i.getEndDate() <= m.getStartDate()) {
                continue;
            } else {
                flag = false;
                break;
            }
        }
        if (flag) {
            m.addParticipator(participator);
        }
    });
    return add && flag;
}


bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator) {
    // if username or participator don't exist
    std::list<User> slu = m_storage->queryUser([&](const User& u) {
        return u.getName() == userName || u.getName() == participator;
    });
    if (slu.size() != 2) {
        return false;
    }
    bool flag = false;
    int um = m_storage->updateMeeting([&](const Meeting& m) {
        return m.getSponsor() == userName && m.isParticipator(participator)
            && m.getTitle() == title;
    }, [&](Meeting& m) {
        m.removeParticipator(participator);
        if (m.getParticipator().size() == 0) {
            flag = true;
        }
    });
    if (flag) {
        m_storage->deleteMeeting([&](const Meeting& d) {
            return d.getTitle() == title;
        });
    }
    return um;
}


bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    bool flag = false;
    int um = m_storage->updateMeeting([&](const Meeting& m) {
        return m.isParticipator(userName) && m.getTitle() == title;
    }, [&](Meeting& m) {
        m.removeParticipator(userName);
        if (m.getParticipator().size() == 0) {
            flag = true;
        }
    });
    if (flag) {
        m_storage->deleteMeeting([&](const Meeting& d) {
            return d.getTitle() == title;
        });
    }
    return um;
}


std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const {
    return m_storage->queryMeeting([&](const Meeting& m) {
        return (m.getSponsor() == userName || m.isParticipator(userName))
            && m.getTitle() == title;
    });
}


std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const {
    Date s(startDate);
    Date e(endDate);
    if (!Date::isValid(s) || !Date::isValid(e) || s > e) {
        return std::list<Meeting>();
    }
    return m_storage->queryMeeting([&](const Meeting& m) {
        return (m.getSponsor() == userName || m.isParticipator(userName))
            && !(e < m.getStartDate() || m.getEndDate() < s);
    });
}


std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    return m_storage->queryMeeting([&](const Meeting& m) {
        return m.getSponsor() == userName || m.isParticipator(userName);
    });
}


std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    return m_storage->queryMeeting([&](const Meeting& m) {
        return m.getSponsor() == userName;
    });
}


std::list<Meeting> AgendaService::listAllParticipateMeetings(
      const std::string &userName) const {
    return m_storage->queryMeeting([&](const Meeting& m) {
        return m.isParticipator(userName);
    });
}


bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    return m_storage->deleteMeeting([&](const Meeting& m) {
        return m.getSponsor() == userName && m.getTitle() == title;
    });
}


bool AgendaService::deleteAllMeetings(const std::string &userName) {
    return m_storage->deleteMeeting([&](const Meeting& m) {
        return m.getSponsor() == userName;
    });
}


void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}


void AgendaService::quitAgenda(void) {
    m_storage->sync();
}
