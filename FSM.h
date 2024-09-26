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

class FSM;

class State {
private:
    std::string name;
    std::int32_t state;

public:
    State(std::string  name, int state) : name(std::move(name)), state(state) {};

//    virtual ~State() = default;

    virtual void OnSetFSM(FSM &fsm) {}
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

    std::map<std::int32_t, std::shared_ptr<State>> states;

    void log(const std::string& msg) {
        std::cout << "[" << name << "]" << msg << std::endl;
    }
public:
    FSM(std::string  name) : name(std::move(name)) {};

    void addState(std::int32_t stId, const std::shared_ptr<State>& st) {
        states.emplace(stId, st);
        st->OnSetFSM(*this);
    }

    void run() {
        if (!states.contains(current_state)) {
            log("transition: state not found");
            return;
        }

        std::cout << "[" << name << "] RUN" << std::endl;
        states[current_state]->OnEntry();
        std::cout << "[" << name << "] OVER" << std::endl;

    }

    void transition(std::int32_t stId) {
        if (!states.contains(stId)) {
            log("transition: state not found");
            return;
        }

        states[current_state]->OnExit();

        pre_state = current_state;
        current_state = stId;

        states[current_state]->OnEntry();
    }

    void dispatchEvent(std::int32_t evtId, const std::shared_ptr<void> data) {
        states[current_state]->onEvent(evtId, data);
    }

    virtual ~FSM() {} // 添加虚析构函数，使 FSM 成为多态类
};



#endif //FSM_H
