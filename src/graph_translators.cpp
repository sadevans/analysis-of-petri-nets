/*! @file graph_translators.cpp
Translator functions cpp file.
@authors A. Kozov
@date 2021/08/29 */

#include "graph_translators.hpp"

#define HEADER_STRING "# DES model generated file v0.1"
#define XLABEL_SUB(i, sub) "[xlabel=<" << i << "<SUB>" << sub <<"</SUB>>]"
#define XLABEL_SUP_SUB(i, sup, sub) "[xlabel=<" << i << "<SUP>" << sup << "</SUP><SUB>" << sub <<"</SUB>>]"
#define COLOR_LABEL(c) "[fillcolor=" << c << "]"

#define DIGRAPH_OPEN "digraph PetriNet {\n\trankdir=LR"
#define DIGRAPH_CLOSE "}"

#define PLACE_OPEN "\tsubgraph place {\n\t\tnode [shape=circle, fixedsize=true, label=\"\", height=0.5, width=0.5]"
#define PLACE_CLOSE "\t}"

#define PLACE_INDEX_SUB(n, i, sub) "\t\t\"p_" << n << "\" " << XLABEL_SUB(i, sub)
#define PLACE_INDEX_SUP_SUB(n, i, sup, sub) "\t\t\"p_" << n << "\" " << XLABEL_SUP_SUB(i, sup, sub)
#define TOKEN(q) "[label=\"*" << q << "\"]"

#define TRANSITION_OPEN "\tsubgraph transition {\n\tnode [style=filled, shape=rect, label=\"\", height=0.5, width=0.1]"
#define TRANSITION_CLOSE "\t}"

#define TRANSITION_INDEX_SUB(n, i, sub) "\t\t\"t_" << n << "\" " << XLABEL_SUB(i, sub)
#define TRANSITION_INDEX_SUP_SUB(n, i, sup, sub) "\t\t\"t_" << n << "\" " << XLABEL_SUP_SUB(i, sup, sub)

#define MACRO_EVENT_OPEN(n) "\t\tsubgraph \"cluster_" << n << "\" {\n\t\t\tstyle=\"dotted\"; label=\"" << n << "\""
#define MASKED_EVENT(n) "\t\t\t\"t_" << n << "\""
#define MACRO_EVENT_CLOSE "\t\t}"

#define ARC_TO_TRANSITION_NUMBER(p, t) "\t\"p_" << p << "\" -> \"" << "t_" << t << "\""
#define ARC_TO_PLACE_NUMBER(t, p) "\t\"t_" << t << "\" -> \"" << "p_" << p << "\""


using namespace std;
using namespace des;


//std::string translateAutomationToDOT(const Automation& a) {
//  stringstream s;
//  s << a;
//  return s.str();
//}

// Stream output.
ostream& des::operator<<(ostream& o, const Automation& a) {
  o << HEADER_STRING << endl;
  // add net
  o << DIGRAPH_OPEN << endl;
  // add places
  o << PLACE_OPEN << endl;
  for (const auto& state : a.getStateNameSet()) {
    auto index = state.find_first_of('/');
    if (index != string::npos) {
      o << PLACE_INDEX_SUP_SUB(state, "p", string(state, 0, index), string(state, index + 1));
    } else {
      o << PLACE_INDEX_SUB(state, "p", state);
    }
    const unsigned t = a.getActivity(state);
    if (t > 0) {
      o << TOKEN(t);
    }
    o << endl;
  }
  o << PLACE_CLOSE << endl; // close subgraph place
  // add transitions
  o << TRANSITION_OPEN << endl;
  for (const auto& t : a.getEventNameSet()) {
    const auto& event = a.getEventCopy(t);
    auto index = t.find_first_of('/');
    if (a.getType(t) != EventType::macro) {
      if (index != string::npos) {
        o << TRANSITION_INDEX_SUP_SUB(t, "t", string(t, 0, index), string(t, index + 1));
      } else {
        o << TRANSITION_INDEX_SUB(t, "t", t);
      }
      // colors for type
      if (a.getType(t) == EventType::uncontrollable) {
        o << COLOR_LABEL("coral");
      } else if (a.getType(t) == EventType::controllable) {
        o << COLOR_LABEL("cornflowerblue");
      }
    } else { // macro
      o << MACRO_EVENT_OPEN(t) << endl; // todo
      for (const auto& n : event.getMaskedEvents()) {
        o << MASKED_EVENT(n) << endl;
      }
      o << MACRO_EVENT_CLOSE;
    }
    o << endl;
  }
  o << TRANSITION_CLOSE << endl; // close subgraph transition
  // add arcs
  for (const auto& p : a.getStateNameSet()) {
    for (const auto& t : a.getEventNameSet()) {
      for (unsigned i = 0; i < a.getLinksFromStateToEvent(p, t); i++) {
        o << ARC_TO_TRANSITION_NUMBER(p, t) << endl;
      }
      for (unsigned i = 0; i < a.getLinksFromEventToState(t, p); i++) {
        o << ARC_TO_PLACE_NUMBER(t, p) << endl;
      }
    }
  }
  o << DIGRAPH_CLOSE << endl; // close digraph PN
  return  o;
}
