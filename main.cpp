#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "graph_translators.hpp"

#include "automation.hpp"
#include "analyse.hpp"

using namespace std;


int main() {
    try {
        des::Automation move_fsm;
        move_fsm.addState("Wait", 1);
        move_fsm.addState("Move");
        move_fsm.addEvent("start", des::EventType::controllable);
        move_fsm.addEvent("Success", des::EventType::expected);
        move_fsm.setLinkFromStateToEvent("Wait", "start", 1);
        move_fsm.setLinkFromEventToState("start", "Move", 1);
        move_fsm.setLinkFromStateToEvent("Move", "Success");
        move_fsm.setLinkFromEventToState("Success", "Wait");



        des::Automation exp1;
        exp1.addState("p1", 1);
        exp1.addState("p2");
        exp1.addState("p3");
        exp1.addState("p4");
        exp1.addState("p5");
        exp1.addEvent("t1", des::EventType::controllable);
        exp1.addEvent("t2", des::EventType::controllable);
        exp1.addEvent("t3", des::EventType::controllable);
        exp1.addEvent("t4", des::EventType::controllable);
        exp1.addEvent("t5", des::EventType::controllable);
        //exp1.addEvent("t6", des::EventType::controllable); // Добавим для теста

        exp1.setLinkFromStateToEvent("p1", "t1", 1);
        exp1.setLinkFromEventToState("t1", "p2", 1);
        exp1.setLinkFromStateToEvent("p2", "t2", 1);
        exp1.setLinkFromEventToState("t2", "p3", 1);
        exp1.setLinkFromStateToEvent("p3", "t3", 1);
        //exp1.setLinkFromEventToState("t2", "p4", 1);  // Добавим для теста

        exp1.setLinkFromEventToState("t3", "p4", 1);
        exp1.setLinkFromStateToEvent("p4", "t4", 1);
        exp1.setLinkFromEventToState("t4", "p5", 1);
        exp1.setLinkFromStateToEvent("p5", "t5", 1);
        exp1.setLinkFromEventToState("t5", "p1", 1);


        map<string, int> mark_e;

        for (const auto& s : exp1.getStateNameSet()) {
            mark_e[s] = exp1.getActivity(s);

        }

        Analyser an;

        map <string, bool> result;

        cout << "ANALYSIS RESULT №1" << endl;
        result = an.run_analyse(exp1);

      for (const auto& s : result) {
            cout<< s.first << " " << s.second << endl;
        }

        des::Automation exp2;
        exp2.addState("p1", 1);
        exp2.addState("p2");
        exp2.addState("p3", 1);
        exp2.addState("p4");
        exp2.addState("p5");
        exp2.addEvent("t1", des::EventType::controllable);
        exp2.addEvent("t2", des::EventType::controllable);
        exp2.addEvent("t3", des::EventType::controllable);
        exp2.addEvent("t4", des::EventType::controllable);
        exp2.addEvent("t5", des::EventType::controllable);

        exp2.setLinkFromStateToEvent("p1", "t1", 1);
        exp2.setLinkFromEventToState("t1", "p2", 1);
        exp2.setLinkFromStateToEvent("p2", "t2", 1);
        exp2.setLinkFromEventToState("t2", "p3", 1);
        exp2.setLinkFromStateToEvent("p3", "t3", 1);
        exp2.setLinkFromEventToState("t2", "p4", 1);
        exp2.setLinkFromStateToEvent("p4", "t4", 1);

        exp2.setLinkFromEventToState("t4", "p5", 1);
        exp2.setLinkFromStateToEvent("p5", "t5", 1);
        exp2.setLinkFromEventToState("t5", "p1", 1);
        cout<< "ANALYSIS RESULT №2"<< endl;
        map <string, bool> result2;



        result2 = an.run_analyse(exp2);
        for (const auto& s : result2) {
            cout<< s.first << " " << s.second << endl;
        }

        des::Automation exp3;
        exp3.addState("p1", 1);
        exp3.addState("p2");
        exp3.addState("p3", 1);
        exp3.addState("p4");
        exp3.addState("p5");
        exp3.addEvent("t1", des::EventType::controllable);
        exp3.addEvent("t2", des::EventType::controllable);
        exp3.addEvent("t3", des::EventType::controllable);
        exp3.addEvent("t4", des::EventType::controllable);
        exp3.addEvent("t5", des::EventType::controllable);

        exp3.setLinkFromStateToEvent("p1", "t1", 1);
        exp3.setLinkFromEventToState("t1", "p2", 1);
        exp3.setLinkFromStateToEvent("p2", "t4", 1);
        exp3.setLinkFromEventToState("t4", "p5", 1);
        exp3.setLinkFromStateToEvent("p5", "t5", 1);
        exp3.setLinkFromEventToState("t5", "p1", 1);
        exp3.setLinkFromStateToEvent("p3", "t3", 1);
        exp3.setLinkFromEventToState("t3", "p4", 1);
        exp3.setLinkFromStateToEvent("p4", "t2", 1);
        exp3.setLinkFromEventToState("t2", "p3", 1);

        cout<< "ANALYSIS RESULT №3"<< endl;

        map <string, bool> result3;
        result3 = an.run_analyse(exp3);
        for (const auto& s : result3) {
            cout<< s.first << " " << s.second << endl;
        }

        des::Automation exp4;
        exp4.addState("p1", 1);
        exp4.addState("p2");
        exp4.addState("p3", 1);
        exp4.addState("p4");
        exp4.addState("p5");
        exp4.addEvent("t1", des::EventType::controllable);
        exp4.addEvent("t2", des::EventType::controllable);
        exp4.addEvent("t3", des::EventType::controllable);
        exp4.addEvent("t4", des::EventType::controllable);
        exp4.addEvent("t5", des::EventType::controllable);

        exp4.setLinkFromStateToEvent("p1", "t1", 1);
        exp4.setLinkFromEventToState("t1", "p2", 1);
        exp4.setLinkFromStateToEvent("p2", "t4", 1);
        exp4.setLinkFromEventToState("t4", "p5", 1);
        exp4.setLinkFromStateToEvent("p5", "t5", 1);
        exp4.setLinkFromEventToState("t5", "p1", 1);
        exp4.setLinkFromStateToEvent("p3", "t3", 1);
        exp4.setLinkFromEventToState("t3", "p4", 1);
        exp4.setLinkFromStateToEvent("p4", "t2", 1);
        exp4.setLinkFromEventToState("t2", "p3", 1);
        exp4.setLinkFromEventToState("t4", "p1", 1);
        cout<< "ANALYSIS RESULT №4"<< endl;

        map <string, bool> result4;
        result4 = an.run_analyse(exp4);
        for (const auto& s : result4) {
            cout<< s.first << " " << s.second << endl;
        }


        des::Automation exp5;
        exp5.addState("p1", 1);
        exp5.addState("p2", 1);
        exp5.addState("p3", 1);
        exp5.addState("p4", 1);
        exp5.addState("p5");
        exp5.addState("p6");
        exp5.addState("p7");
        exp5.addState("p8");
        exp5.addState("p9");
        exp5.addEvent("t1", des::EventType::controllable);
        exp5.addEvent("t2", des::EventType::controllable);
        exp5.addEvent("t3", des::EventType::controllable);
        exp5.addEvent("t4", des::EventType::controllable);
        exp5.addEvent("t5", des::EventType::controllable);
        exp5.addEvent("t6", des::EventType::controllable);
        exp5.addEvent("t7", des::EventType::controllable);
        exp5.setLinkFromStateToEvent("p1", "t2", 1);
        exp5.setLinkFromStateToEvent("p1", "t3", 1);
        exp5.setLinkFromEventToState("t2", "p8", 1);
        exp5.setLinkFromEventToState("t3", "p9", 1);
        exp5.setLinkFromStateToEvent("p8", "t4", 1);
        exp5.setLinkFromStateToEvent("p9", "t5", 1);
        exp5.setLinkFromEventToState("t4", "p3", 1);
        exp5.setLinkFromEventToState("t5", "p3", 1);
        exp5.setLinkFromStateToEvent("p3", "t5", 1);
        exp5.setLinkFromStateToEvent("p3", "t4", 1);
        exp5.setLinkFromEventToState("t4", "p5", 1);
        exp5.setLinkFromEventToState("t5", "p6", 1);
        exp5.setLinkFromStateToEvent("p5", "t6", 1);
        exp5.setLinkFromStateToEvent("p6", "t7", 1);
        exp5.setLinkFromEventToState("t6", "p7", 1);
        exp5.setLinkFromEventToState("t7", "p7", 1);
        exp5.setLinkFromEventToState("t7", "p4", 1);
        exp5.setLinkFromEventToState("t6", "p2", 1);
        exp5.setLinkFromStateToEvent("p4", "t3", 1);
        exp5.setLinkFromStateToEvent("p2", "t2", 1);
        exp5.setLinkFromStateToEvent("p7", "t1", 1);
        exp5.setLinkFromEventToState("t1", "p1", 1);

        cout<< "ANALYSIS RESULT №5"<< endl;
        map <string, bool> result5;
        result5 = an.run_analyse(exp5);
        for (const auto& s : result5) {
            cout<< s.first << " " << s.second << endl;
        }





        des::Automation exp12;  
        exp12.addState("p1");
        exp12.addState("p2", 2);
        exp12.addState("p3");
        exp12.addState("p4", 1);
        exp12.addState("p5");
        exp12.addState("p6", 1);
        exp12.addEvent("t1", des::EventType::controllable);
        exp12.addEvent("t2", des::EventType::controllable);
        exp12.addEvent("t3", des::EventType::controllable);
        exp12.addEvent("t4", des::EventType::controllable);

        exp12.setLinkFromEventToState("t1", "p1", 1);
        exp12.setLinkFromEventToState("t1", "p3", 1);
        exp12.setLinkFromStateToEvent("p1", "t2", 1);
        exp12.setLinkFromEventToState("t2", "p3", 1);
        exp12.setLinkFromStateToEvent("p2", "t1", 1);
        exp12.setLinkFromStateToEvent("p6", "t2", 1);
        exp12.setLinkFromStateToEvent("p3", "t3", 1);
        exp12.setLinkFromEventToState("t3", "p6", 1);
        exp12.setLinkFromEventToState("t3", "p2", 1);
        exp12.setLinkFromStateToEvent("p4", "t3", 1);

        exp12.setLinkFromEventToState("t3", "p5", 1);
        exp12.setLinkFromStateToEvent("p5", "t4", 1);
        exp12.setLinkFromEventToState("t4", "p4", 1);

        map <string, bool> result12;
        cout<< "ANALYSIS RESULT №6"<< endl;
        result12 = an.run_analyse(exp12);
        for (const auto& s : result12) {
            cout<< s.first << " " << s.second << endl;
        }

        des::Automation exp13;  
        exp13.addState("p1", 1);
        exp13.addState("p2");
        exp13.addState("p3");
        exp13.addState("p4");

        exp13.addEvent("t1", des::EventType::controllable);
        exp13.addEvent("t2", des::EventType::controllable);
        exp13.addEvent("t3", des::EventType::controllable);
        exp13.addEvent("t4", des::EventType::controllable);
        exp13.addEvent("t5", des::EventType::controllable);
        exp13.addEvent("t6", des::EventType::controllable);


        exp13.setLinkFromStateToEvent("p1", "t1", 1);
        exp13.setLinkFromEventToState("t1", "p2", 1);
        exp13.setLinkFromStateToEvent("p1", "t2", 1);
        exp13.setLinkFromEventToState("t2", "p2", 1);
        exp13.setLinkFromEventToState("t2", "p3", 1);
        exp13.setLinkFromStateToEvent("p1", "t3", 1);
        exp13.setLinkFromEventToState("t3", "p3", 1);

        exp13.setLinkFromStateToEvent("p2", "t4", 1);
        exp13.setLinkFromEventToState("t4", "p3", 1);

        exp13.setLinkFromStateToEvent("p3", "t5", 2);
        exp13.setLinkFromEventToState("t5", "p4", 1);
        exp13.setLinkFromStateToEvent("p4", "t6", 1);
        exp13.setLinkFromEventToState("t6", "p1", 1);

        map <string, bool> result13;
        cout<< "ANALYSIS RESULT №7"<< endl;
        result13 = an.run_analyse(exp13);
        for (const auto& s : result13) {
            cout<< s.first << " " << s.second << endl;
        }
        des::Automation exp14;  
        exp14.addState("p1", 2);
        exp14.addState("p2", 1);
        exp14.addState("p3");
        exp14.addState("p4");
        exp14.addState("p5");
        exp14.addState("p6");
        exp14.addState("p7");
        exp14.addEvent("t1", des::EventType::controllable);
        exp14.addEvent("t2", des::EventType::controllable);
        exp14.addEvent("t3", des::EventType::controllable);
        exp14.addEvent("t4", des::EventType::controllable);
        exp14.addEvent("t5", des::EventType::controllable);


        exp14.setLinkFromStateToEvent("p1", "t1", 1);
        exp14.setLinkFromEventToState("t1", "p3", 1);
        exp14.setLinkFromEventToState("t1", "p4", 1);
        exp14.setLinkFromStateToEvent("p2", "t1", 1);
        exp14.setLinkFromStateToEvent("p5", "t2", 1);
        exp14.setLinkFromEventToState("t2", "p7", 1);
        exp14.setLinkFromStateToEvent("p7", "t5", 1);
        exp14.setLinkFromEventToState("t5", "p5", 1);
        exp14.setLinkFromStateToEvent("p3", "t3", 1);
        exp14.setLinkFromEventToState("t3", "p6", 1);
        exp14.setLinkFromStateToEvent("p4", "t4", 1);
        exp14.setLinkFromEventToState("t4", "p6", 1);

        map <string, bool> result14;
        cout<< "ANALYSIS RESULT №8"<< endl;
        result14 = an.run_analyse(exp14);
        for (const auto& s : result14) {
            cout<< s.first << " " << s.second << endl;
        }

        des::Automation exp15;  
        exp15.addState("p1", 1);
        exp15.addState("p2", 2);
        exp15.addState("p3");
        exp15.addState("p4");
        exp15.addState("p5");
        exp15.addState("p6");
        exp15.addState("p7");
        exp15.addState("p8");
        exp15.addEvent("t1", des::EventType::controllable);
        exp15.addEvent("t2", des::EventType::controllable);
        exp15.addEvent("t3", des::EventType::controllable);
        exp15.addEvent("t4", des::EventType::controllable);
        exp15.addEvent("t5", des::EventType::controllable);

        exp15.setLinkFromStateToEvent("p1", "t1", 1);
        exp15.setLinkFromEventToState("t1", "p3", 1);
        exp15.setLinkFromEventToState("t1", "p4", 1);
        exp15.setLinkFromStateToEvent("p2", "t2", 1);
        exp15.setLinkFromEventToState("t2", "p6", 1);
        exp15.setLinkFromStateToEvent("p3", "t3", 1);
        exp15.setLinkFromEventToState("t3", "p5", 1);
        exp15.setLinkFromStateToEvent("p4", "t4", 1);
        exp15.setLinkFromStateToEvent("p4", "t2", 1);
        exp15.setLinkFromEventToState("t4", "p7", 1);
        exp15.setLinkFromStateToEvent("p5", "t5", 1);
        exp15.setLinkFromStateToEvent("p5", "t4", 1);
        exp15.setLinkFromStateToEvent("p6", "t5", 1);
        exp15.setLinkFromEventToState("t5", "p8", 1);
        exp15.setLinkFromEventToState("t4", "p7", 1);

        map <string, bool> result15;
        cout<< "ANALYSIS RESULT №9"<< endl;
        result15 = an.run_analyse(exp15);
        for (const auto& s : result15) {
            cout<< s.first << " " << s.second << endl;
        }



        des::Automation exp16;  
        exp16.addState("p1", 1);
        exp16.addState("p2");
        exp16.addState("p3", 1);
        exp16.addState("p4");
        exp16.addState("p5");

        exp16.addEvent("t1", des::EventType::controllable);
        exp16.addEvent("t2", des::EventType::controllable);
        exp16.addEvent("t3", des::EventType::controllable);


        exp16.setLinkFromStateToEvent("p1", "t1", 1);
        exp16.setLinkFromEventToState("t1", "p2", 1);

        exp16.setLinkFromStateToEvent("p2", "t2", 1);
        exp16.setLinkFromEventToState("t2", "p3", 1);
        exp16.setLinkFromStateToEvent("p3", "t1", 1);
        exp16.setLinkFromEventToState("t2", "p4", 1);
        exp16.setLinkFromStateToEvent("p4", "t3", 1);

        exp16.setLinkFromEventToState("t3", "p1", 1);

        exp16.setLinkFromEventToState("t3", "p5", 1);

        map <string, bool> result16;
                cout<< "ANALYSIS RESULT №10"<< endl;
                result16 = an.run_analyse(exp16);
                for (const auto& s : result16) {
                    cout<< s.first << " " << s.second << endl;
                }

    }

    catch (const std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
    return 0;
}
