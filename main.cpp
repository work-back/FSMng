#include <iostream>
#include <utility>

#include "FSM.h"

using namespace std;

#define _FSM do { my_sm = dynamic_cast<mySM*>(&fsm); if (!my_sm) return; } while (0)


class mySM : public FSM {
private:
    std::string fsmNameTT = "mySM";

    enum {
        MY_FSM_ST_ID_INIT = 1,
        MY_FSM_ST_ID_RUN,
    };

    enum {
        MY_FSM_ST_EVT_ID_RUN = 1,
        MY_FSM_ST_EVT_ID_STOP,
    };

    struct UData {
        int id;
        std::string name;
    };

    public:
    mySM(const std::string& name) : FSM(name) {
        st_init = std::make_shared<StInit>(MY_FSM_ST_ID_INIT);
        st_run = std::make_shared<StRun>(MY_FSM_ST_ID_RUN);


        logf("st_init:id:%d,name:%s", st_init->getState(), st_init->getName().c_str());
        logf("st_run:id:%d,name:%s", st_run->getState(), st_run->getName().c_str());

        addState(st_init);
        addState(st_run);

//        st_init->OnEntry();
//        current_state = 1;
        setInitState(MY_FSM_ST_ID_INIT);

        run();

        std::shared_ptr<UData> _d(new UData());

        _d->id = 123;
        _d->name = "Hello";

        FSM::dispatchEvent(MY_FSM_ST_EVT_ID_RUN, _d);
    }

private:
    class StInit : public State {
    private:
         mySM* my_sm;
    public:
        StInit(int state) : State(_N, state) {}

        void OnSetFSM(FSM &fsm) override {_FSM;}

        void OnEntry() override {
            std::cout << "Entering StInit state" << std::endl;
            std::cout << "V:" << my_sm->fsmNameTT << std::endl;
        }

        void OnExit() override {
            std::cout << "Exiting StInit state" << std::endl;
        }

        void onEvent(std::int32_t evtId, const std::shared_ptr<void> data) override {
            std::cout << "StInit state handling event: " << evtId << std::endl;
            switch (evtId) {
                case MY_FSM_ST_EVT_ID_RUN : {
                    auto _d = std::static_pointer_cast<UData>(data);
                    std::cout << "id:" << _d->id << "name:" << _d->name << evtId << std::endl;
                    my_sm->transition(MY_FSM_ST_ID_RUN);
                    break;
                }

            }
        }
    };

    class StRun : public State {
    private:
        mySM* my_sm;
    public:
        StRun(int state) : State(_N, state) {}

        void OnSetFSM(FSM &fsm) override {_FSM;}

        void OnEntry() override {
            std::cout << "Entering StRun state" << std::endl;
        }

        void OnExit() override {
            std::cout << "Exiting StRun state" << std::endl;
        }

        void onEvent(std::int32_t evtId, const std::shared_ptr<void> data) override {
            std::cout << "StRun state handling event: " << evtId << std::endl;

        }
    };

    std::shared_ptr<State> st_init;
    std::shared_ptr<State> st_run;
};

int main() {
    mySM mysm("mysm");

    return 0;
}
