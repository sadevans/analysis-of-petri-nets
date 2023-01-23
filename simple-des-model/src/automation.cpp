/*! @file automation.cpp
@ref des::Automation class source file.
@authors A. Kozov
@date 2021/12/20 */

#include "automation.hpp"


using namespace std;
using namespace des;

// Constructor of des::Automation object.
Automation::Automation() : _states(), _events() {
}

// State addition with creation.
void Automation::addState(const string& n, const unsigned& a) {
  if (!checkNameString(n)) {
    throw invalid_argument("addState: state name is invalid");
  }
  if (_states.find(n) == _states.end()) {
    _states.emplace(n, ComponentWithLinks(State(a)));
  } else {
    throw invalid_argument("addState: state name already exists");
  }
}

// State addition by copying.
void Automation::addState(const string& n, const State& s) {
  if (!checkNameString(n)) {
    throw invalid_argument("addState: state name is invalid");
  }
  if (_states.find(n) == _states.end()) {
    _states.emplace(n, ComponentWithLinks(s));
  } else {
    _states.at(n).Component = s;
  }
}

// State addition by moving.
void Automation::addState(const string& n, State&& s) {
  if (!checkNameString(n)) {
    throw invalid_argument("addState: state name is invalid");
  }
  if (_states.find(n) == _states.end()) {
    _states.emplace(n, ComponentWithLinks(std::move(s)));
  } else {
    _states.at(n).Component = std::move(s);
  }
}

// State getting by name.
State& Automation::getState(const string& n) {
  if (_states.find(n) != _states.end()) {
    return _states.at(n).Component;
  } else {
    throw invalid_argument("getState: state name doesn't exist");
  }
}

// Set of state names.
set<string> Automation::getStateNameSet() const noexcept {
  std::set<std::string> result = {};
  for (const auto& s : _states) {
    result.insert(s.first);
  }
  return result;
}

// Change activity token quantity.
void Automation::setActivity(const string& n, const unsigned int& a) {
  if (checkState(n)) {
    getState(n).setActivity(a);
  } else {
    throw invalid_argument("setActivity: state name doesn't exist");
  }
}

// Quantity of input links of state.
size_t Automation::getStateInputLinkQuantity(const string& n) const noexcept {
  if (_states.find(n) != _states.end()) {
    return _states.at(n).Inputs.size();
  }
  return 0;
}

// Quantity of output links of state.
size_t Automation::getStateOutputLinkQuantity(const string& n) const noexcept {
  if (_states.find(n) != _states.end()) {
    return _states.at(n).Outputs.size();
  }
  return 0;
}

// Set of input event names.
set<string> Automation::getStateInputs(const string& n) const noexcept {
  set<string> inputs = {};
  if (checkState(n)) {
    for (const auto& i : _states.at(n).Inputs) {
      inputs.insert(i.first);
    }
  }
  return inputs;
}

// Set of output event names.
set<string> Automation::getStateOutputs(const string& n) const noexcept {
  set<string> outputs = {};
  if (checkState(n)) {
    for (const auto& i : _states.at(n).Outputs) {
      outputs.insert(i.first);
    }
  }
  return outputs;
}

// State deletion.
void Automation::removeState(const string& n) noexcept {
  if (_states.find(n) != _states.end()) {
    for (const auto& event : _states.at(n).Inputs) {
      _events.at(event.first).Outputs.erase(n);
    }
    for (const auto& event : _states.at(n).Outputs) {
      _events.at(event.first).Inputs.erase(n);
    }
    _states.erase(n);
  }
}

// Event addition with creation.
void Automation::addEvent(const string& n, const EventType& t) {
  if (!checkNameString(n)) {
    throw invalid_argument("addEvent: event name is invalid");
  }
  if (_events.find(n) == _events.end()) {
    _events.emplace(n, ComponentWithLinks(Event(t)));
  } else {
    throw invalid_argument("addEvent: event name already exists");
  }
}

// Event addition by copying.
void Automation::addEvent(const string& n, const Event& e) {
  if (!checkNameString(n)) {
    throw invalid_argument("addEvent: event name is invalid");
  }
  if (!checkMacroEvent(e)) {
    throw runtime_error("addEvent: masked events list of macro event is invalid");
  }
  if (_events.find(n) == _events.end()) {
    _events.emplace(n, ComponentWithLinks(e));
  } else {
    _events.at(n).Component = e;
  }
}

// Event addition by moving.
void Automation::addEvent(const string& n, Event&& e) {
  if (!checkNameString(n)) {
    throw invalid_argument("addEvent: event name is invalid");
  }
  if (!checkMacroEvent(e)) {
    throw runtime_error("addEvent: masked events list of macro event is invalid");
  }
  if (_events.find(n) == _events.end()) {
    _events.emplace(n, ComponentWithLinks(std::move(e)));
  } else {
    _events.at(n).Component = std::move(e);
  }
}

// Event getting by name.
Event& Automation::getEvent(const string &n) {
  if (_events.find(n) != _events.end()) {
    return _events.at(n).Component;
  } else {
    throw invalid_argument("getEvent: event name doesn't exist");
  }
}

// Set of event names.
set<string> Automation::getEventNameSet() const noexcept {
  std::set<std::string> result = {};
  for (const auto& e : _events) {
    result.insert(e.first);
  }
  return result;
}

// Quantity of input links of event.
size_t Automation::getEventInputLinksQuantity(const string& n) const noexcept {
  if (_events.find(n) != _events.end()) {
    return _events.at(n).Inputs.size();
  }
  return 0;
}

// Quantity of output links of event.
size_t Automation::getEventOutputLinksQuantity(const string& n) const noexcept {
  if (_events.find(n) != _events.end()) {
    return _events.at(n).Outputs.size();
  }
  return 0;
}

// Set of input state names.
set<string> Automation::getEventInputs(const string& n) const noexcept {
  set<string> inputs = {};
  if (checkEvent(n)) {
    for (const auto& i : _events.at(n).Inputs) {
      inputs.insert(i.first);
    }
  }
  return inputs;
}

// Set of output state names.
set<string> Automation::getEventOutputs(const string& n) const noexcept {
  set<string> outputs = {};
  if (checkEvent(n)) {
    for (const auto& i : _events.at(n).Outputs) {
      outputs.insert(i.first);
    }
  }
  return outputs;
}

// Type of event.
EventType Automation::getType(const string &n) const {
  if (_events.find(n) != _events.end()) {
    return _events.at(n).Component.type();
  } else {
    throw std::invalid_argument("getType: event name doesn't exist");
  }
}

// Event deletion.
void Automation::removeEvent(const string& n) noexcept {
  if (_events.find(n) != _events.end()) {
    for (const auto& state : _events.at(n).Inputs) {
      _states.at(state.first).Outputs.erase(n);
    }
    for (const auto& state : _events.at(n).Outputs) {
      _states.at(state.first).Inputs.erase(n);
    }
    _events.erase(n);
  }
}

// Link from state to event.
void Automation::setLinkFromStateToEvent(const string& s, const string& e, const unsigned int& m) {
  if (!checkState(s)) {
    throw invalid_argument("setLinkFromStateToEvent: state name doesn't exist");
  }
  if (!checkEvent(e)) {
    throw invalid_argument("setLinkFromStateToEvent: event name doesn't exist");
  }
  if (m) {
    _states.at(s).Outputs[e] = m;
    _events.at(e).Inputs[s] = m;
  } else {
    _states.at(s).Outputs.erase(e);
    _events.at(e).Inputs.erase(s);
  }
}

// Multiplicity of link from state to event.
unsigned Automation::getLinksFromStateToEvent(const string& s, const string& e) const noexcept {
  if (checkState(s) && checkEvent(e)) {
    if (_events.at(e).Inputs.find(s) != _events.at(e).Inputs.end()) {
      return _events.at(e).Inputs.at(s); // because it's positive
    }
  }
  return 0;
}

// Link from event to state.
void Automation::setLinkFromEventToState(const string& e, const string& s, const unsigned int& m) {
  if (!checkEvent(e)) {
    throw invalid_argument("setLinkFromEventToState: event name doesn't exist");
  }
  if (!checkState(s)) {
    throw invalid_argument("setLinkFromEventToState: state name doesn't exist");
  }
  if (m) {
    _events.at(e).Outputs[s] = m;
    _states.at(s).Inputs[e] = m;
  } else {
    _events.at(e).Outputs.erase(s);
    _states.at(s).Inputs.erase(e);
  }
}

// Multiplicity of link from event to state.
unsigned Automation::getLinksFromEventToState(const string& e, const string& s) const noexcept {
  if (checkEvent(e) && checkState(s)) {
    if (_states.at(s).Inputs.find(e) != _states.at(s).Inputs.end()) {
      return _states.at(s).Inputs.at(e); // because it's positive
    }
  }
  return 0;
}

// Link two states by event.
void Automation::linkStatesByEvent(const string& f, const string& e, const string& s) {
  try {
    setLinkFromStateToEvent(f, e);
    setLinkFromEventToState(e, s);
  } catch (const invalid_argument&) {
    throw invalid_argument("linkStatesByEvent: name doesn't exist");
  }
}

// Correctness of events link.
bool Automation::checkMacro() const noexcept {
  for (const auto& e : _events) {
    if (isMacro(e.second.Component)) {
      if (!getEventInputs(e.first).empty() || !getEventOutputs(e.first).empty()) {
        return false; // macro event links check
      }
      if (!checkMacroEvent(e.second.Component)) {
        return false; // masked event list check
      }
    }
  } // events cycle
  return true;
}

// Ready events names set.
set<string> Automation::getReadyEvents() const noexcept {
  set<string> result = {};
  for (const auto& e : _events) {
    bool is_ready = false;
    for (const auto& s : e.second.Inputs) {
      const auto& multiplicity = s.second;
      const auto& state_name = s.first;
      if (multiplicity <= _states.at(state_name).Component.activity()) {
        is_ready = true;
      } else {
        is_ready = false; // not enough tokens for this link
        break;
      }
    }
    if (is_ready) {
      result.insert(e.first);
    }
  }
  return result;
}

// Step to next state.
void Automation::fire(const string& e) {
  const auto& ready_events = getReadyEvents();
  if (!e.empty() && ready_events.find(e) == ready_events.end()) {
    throw invalid_argument("fire: name of firing event is invalid");
  }
  if (ready_events.empty()) {
    return; // no ready events
  }
  string firing_event = e;
  if (firing_event.empty()) {
    firing_event = *ready_events.begin();
  }
  for (const auto& input : _events.at(firing_event).Inputs) {
    const auto& activity = _states.at(input.first).Component.activity();
    _states.at(input.first).Component.setActivity(activity - input.second);
  }
  for (const auto& output : _events.at(firing_event).Outputs) {
    const auto& activity = _states.at(output.first).Component.activity();
    _states.at(output.first).Component.setActivity(activity + output.second);
  }
}

// Correctness of macro event.
bool Automation::checkMacroEvent(const Event& e) const noexcept {
  if (!isMacro(e)) {
    return true;
  }
  for (const auto& n : e.getMaskedEvents()) {
    if (!checkEvent(n)) {
      return false;
    }
  } // masked events cycle
  return true;
}
