//
// Created by lyj05 on 24-9-25.
//

#ifndef FSM_H
#define FSM_H

#include <cstdint>
#include <string>
#include <utility>
#include <iostream>
#include <map>
#include <memory>
#include <cstdarg>
#include <typeinfo>
#include <boost/core/demangle.hpp>

class FSM;

class State {
private:
    std::string name;
    std::int32_t state;

public:
//    State(std::string  name, int state) : name(std::move(name)), state(state) {};
    State(const std::string n, int s) : name(n), state(s) {
    }

    State(int state) : state(state) {
        name = boost::core::demangle(typeid(*this).name());
    };

    std::string getName() {
        return name;
    }

    std::int32_t getState() {
        return state;
    }

//    State(const State& other) : name(other.name), state(other.state) {
//        std::cout << "Copy[" << state << "][" << name << "]" << std::endl;
//    }

    virtual ~State() {
        std::cout << "~State" << std::endl;
    };

    virtual void OnSetFSM(FSM &fsm) {};
    virtual void OnEntry() {}
    virtual void OnExit() {}
    virtual void onEvent(std::int32_t evtId, const std::shared_ptr<void> data) {}
};

class FSM {
protected:
    std::string name;
    std::int32_t current_state;
    std::int32_t pre_state;
    std::int32_t next_state;
    bool in_state_transition = false;

    std::map<std::int32_t, std::shared_ptr<State>> states;

    void log(const std::string& msg) {
        std::cout << "[" << name << "]" << msg << std::endl;
    }

    void logf(const char* format, ...) {
        char buffer[1024];

        va_list args;
        va_start(args, format);
        std::vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        std::cout << "[" << name << "] " << buffer << std::endl;
    }
public:
    FSM(std::string  name) : name(std::move(name)) {};

    void addState(const std::shared_ptr<State>& st) {
        logf("addState: [%d]:[%s]", st->getState(), st->getName().c_str());
        states.emplace(st->getState(), st);
        st->OnSetFSM(*this);
    }

    void run() {
        if (!states.contains(current_state)) {
            log("transition: state not found");
            return;
        }
        std::cout << "[" << name << "] RUN" << std::endl;
        states[current_state]->OnEntry();
    }

    void transition(std::int32_t stId) {
        if (in_state_transition) {
            log("in state transition. skip ...");
            return;
        }

        if (!states.contains(stId)) {
            log("transition: state not found");
            return;
        }

        in_state_transition = true;

        states[current_state]->OnExit();

        logf("state transition [%s] ====> [%s]",
             states[current_state]->getName().c_str(), states[stId]->getName().c_str());

        pre_state = current_state;
        current_state = stId;

        states[current_state]->OnEntry();

        in_state_transition = false;
    }

    void dispatchEvent(std::int32_t evtId, const std::shared_ptr<void> data) {
        states[current_state]->onEvent(evtId, data);
    }

    virtual ~FSM() {} // 添加虚析构函数，使 FSM 成为多态类
};



#endif //FSM_H
