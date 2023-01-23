/*! @file automation.cpp
@ref des::Automation class tests source file.
@authors A. Kozov
@date 2021/12/21 */

#include <boost/test/unit_test.hpp>

#include "automation.hpp"


// Test of Automation default constructor.
BOOST_AUTO_TEST_CASE(CreateAutomation) {
  des::Automation a;
  BOOST_CHECK(a.getStateQuantity() == 0);
  BOOST_CHECK(a.getEventQuantity() == 0);
}

// Test of Automation copy and move constructors.
BOOST_AUTO_TEST_CASE(CreateAutomationyOtherAutomation) {
  des::Automation other;
  const std::string state_name_1 = "TestState1", state_name_2 = "TestState2";
  const std::string event_name_1 = "testEvent1", event_name_2 = "testEvent2";
  other.addState(state_name_1, 2);
  other.addState(state_name_2);
  other.addEvent(event_name_1, des::EventType::macro);
  other.addEvent(event_name_2, des::EventType::controllable);
  other.setLinkFromStateToEvent(state_name_1, event_name_1);
  other.setLinkFromStateToEvent(state_name_2, event_name_2);
  other.setLinkFromEventToState(event_name_1, state_name_2);
  other.setLinkFromEventToState(event_name_2, state_name_1);
  des::Automation copy_result(other); // create by copying other state
  BOOST_CHECK(copy_result.getStateNameSet() == other.getStateNameSet());
  BOOST_CHECK(copy_result.getEventNameSet() == other.getEventNameSet());
  BOOST_CHECK(copy_result.getType(event_name_1) == other.getType(event_name_1));
  BOOST_CHECK(copy_result.getType(event_name_2) == other.getType(event_name_2));
  BOOST_CHECK(copy_result.getReadyEvents() == other.getReadyEvents());
  des::Automation move_result(std::move(other)); // create by moving other state
  BOOST_CHECK(move_result.getStateNameSet() == copy_result.getStateNameSet());
  BOOST_CHECK(move_result.getEventNameSet() == copy_result.getEventNameSet());
  BOOST_CHECK(move_result.getType(event_name_1) == copy_result.getType(event_name_1));
  BOOST_CHECK(move_result.getType(event_name_2) == copy_result.getType(event_name_2));
  BOOST_CHECK(move_result.getReadyEvents() == copy_result.getReadyEvents());
}

// Test of addState with creation and getState methods.
BOOST_AUTO_TEST_CASE(AutomationAddStateWithCreation) {
  des::Automation a;
  const std::string invalid_name = "*?()";
  BOOST_CHECK_THROW(a.addState(invalid_name), std::invalid_argument);
  const std::string name = "TestState";
  BOOST_CHECK(a.getStateInputLinkQuantity(name) == 0);
  BOOST_CHECK(a.getStateOutputLinkQuantity(name) == 0);
  BOOST_CHECK_NO_THROW(a.addState(name, 10)); // addition with creation
  BOOST_CHECK(a.getStateQuantity() == 1);
  BOOST_CHECK(a.getStateInputLinkQuantity(name) == 0);
  BOOST_CHECK(a.getStateOutputLinkQuantity(name) == 0);
  BOOST_CHECK_THROW(a.addState(name), std::invalid_argument);
  BOOST_CHECK_THROW(auto r = a.getState(invalid_name), std::invalid_argument);
  des::State state = a.getState(name);
  state.viewProperties() = { L"Test hint", 5.0, 0.5, 1.0 };
  BOOST_CHECK(!(state == a.getState(name)));
  BOOST_CHECK_NO_THROW(a.getState(name) = state);
  BOOST_CHECK(state == a.getState(name));
  const size_t states_quantity = a.getStateQuantity();
  for (unsigned i = 1; i < 10; i++) {
    const std::string next_name = "T_" + std::to_string(i);
    BOOST_CHECK_NO_THROW(a.addState(next_name));
    BOOST_CHECK(a.getStateQuantity() == states_quantity + i);
    BOOST_CHECK(a.getStateInputLinkQuantity(next_name) == 0);
    BOOST_CHECK(a.getStateOutputLinkQuantity(next_name) == 0);
  }
}

// Test of addState with addition or replacement and getStateCopy methods.
BOOST_AUTO_TEST_CASE(AutomationAddStateWithReplacement) {
  des::Automation a;
  des::State state(5);
  const std::string invalid_name = "*?()";
  BOOST_CHECK_THROW(a.addState(invalid_name, state), std::invalid_argument);
  const std::string name = "TestState";
  BOOST_CHECK_NO_THROW(a.addState(name, state)); // addition with copying
  BOOST_CHECK(a.getStateQuantity() == 1);
  BOOST_CHECK(a.getStateInputLinkQuantity(name) == 0);
  BOOST_CHECK(a.getStateOutputLinkQuantity(name) == 0);
  state.viewProperties() = { L"Test hint", 5.0, 0.5, 1.0 };
  BOOST_CHECK(!(state == a.getStateCopy(name)));
  BOOST_CHECK_NO_THROW(a.addState(name, state)); // replacement with copying
  BOOST_CHECK(state == a.getStateCopy(name));
  BOOST_CHECK_NO_THROW(a.addState("Test", std::move(des::State()))); // addition with moving
  BOOST_CHECK_NO_THROW(a.addState("Test", std::move(des::State()))); // replacement with moving
  BOOST_CHECK_THROW(a.addState(invalid_name, std::move(des::State())), std::invalid_argument);
  const size_t states_quantity = a.getStateQuantity();
  for (unsigned i = 1; i < 10; i++) {
    const std::string next_name = "T_" + std::to_string(i);
    BOOST_CHECK_NO_THROW(a.addState(next_name, state));
    BOOST_CHECK(a.getStateQuantity() == states_quantity + i);
    BOOST_CHECK(a.getStateInputLinkQuantity(next_name) == 0);
    BOOST_CHECK(a.getStateOutputLinkQuantity(next_name) == 0);
  }
}

// Test of checkState method.
BOOST_AUTO_TEST_CASE(AutomationCheckState) {
  des::Automation a;
  const std::string name = "TestState";
  BOOST_CHECK(!a.checkState(name));
  a.addState(name);
  BOOST_CHECK(a.checkState(name));
}

// Test of getStateNameSet method.
BOOST_AUTO_TEST_CASE(AutomationGetStatesNames) {
  des::Automation a;
  const std::string state_name_1 = "TestState1", state_name_2 = "TestState2";
  const std::set<std::string> one_state = { state_name_1 }, two_states = { state_name_1, state_name_2 };
  BOOST_CHECK(a.getStateNameSet().empty());
  a.addState(state_name_1);
  BOOST_CHECK(a.getStateNameSet() == one_state);
  a.addState(state_name_2);
  BOOST_CHECK(a.getStateNameSet() == two_states);
}

// Test of setActivity and getActivity methods.
BOOST_AUTO_TEST_CASE(AutomationActivity) {
  des::Automation a;
  const std::string name = "TestState";
  BOOST_CHECK_THROW(a.setActivity("invalid name", 5), std::invalid_argument);
  BOOST_CHECK(a.getActivity(name) == 0);
  des::State state(5);
  a.addState(name, state);
  const unsigned activity = 100;
  BOOST_CHECK_NO_THROW(a.setActivity(name, activity));
  BOOST_CHECK(a.getActivity(name) == activity);
  BOOST_CHECK_NO_THROW(a.setActivity(name, 0));
  BOOST_CHECK(a.getActivity(name) == 0);
}

// Test of removeState method.
BOOST_AUTO_TEST_CASE(AutomationRemoveState) {
  des::Automation a;
  const std::string state_name = "TestState";
  BOOST_CHECK_NO_THROW(a.removeState(state_name));
  BOOST_CHECK(a.getStateInputLinkQuantity(state_name) == 0);
  BOOST_CHECK(a.getStateOutputLinkQuantity(state_name) == 0);
  BOOST_CHECK(a.getStateQuantity() == 0);
  const des::State state(5);
  a.addState(state_name, state);
  BOOST_CHECK(a.getStateQuantity() == 1);
  BOOST_CHECK_NO_THROW(a.removeState(state_name));
  BOOST_CHECK(a.getStateInputLinkQuantity(state_name) == 0);
  BOOST_CHECK(a.getStateOutputLinkQuantity(state_name) == 0);
  BOOST_CHECK(a.getStateQuantity() == 0);
  a.addState(state_name, state);
  const std::string event_name = "testEvent";
  a.addEvent(event_name, des::EventType::macro);
  a.setLinkFromStateToEvent(state_name, event_name, 10);
  a.setLinkFromEventToState(event_name, state_name, 5);
  BOOST_CHECK_NO_THROW(a.removeState(state_name));
  BOOST_CHECK(a.getStateInputLinkQuantity(state_name) == 0);
  BOOST_CHECK(a.getStateOutputLinkQuantity(state_name) == 0);
  BOOST_CHECK(a.getEventOutputLinksQuantity(event_name) == 0);
  BOOST_CHECK(a.getEventInputLinksQuantity(event_name) == 0);
  BOOST_CHECK(a.getStateQuantity() == 0);
}

// Test of addEvent with creation and getEvent methods.
BOOST_AUTO_TEST_CASE(AutomationAddEventWithCreation) {
  des::Automation a;
  const std::string invalid_name = "*?()";
  BOOST_CHECK_THROW(a.addEvent(invalid_name, des::EventType::controllable), std::invalid_argument);
  const std::string name = "testEvent";
  BOOST_CHECK(a.getEventInputLinksQuantity(name) == 0);
  BOOST_CHECK(a.getEventOutputLinksQuantity(name) == 0);
  BOOST_CHECK_NO_THROW(a.addEvent(name, des::EventType::macro)); // addition with creation
  BOOST_CHECK(a.getEventQuantity() == 1);
  BOOST_CHECK(a.getEventInputLinksQuantity(name) == 0);
  BOOST_CHECK(a.getEventOutputLinksQuantity(name) == 0);
  BOOST_CHECK_THROW(a.addEvent(name, des::EventType::macro), std::invalid_argument);
  BOOST_CHECK_THROW(auto r = a.getEvent(invalid_name), std::invalid_argument);
  des::Event event = a.getEvent(name);
  event.viewProperties() = { L"Test hint", 5.0, 0.5, 1.0 };
  event.behaviorAttributes() = { { L"int", {} }, { L"float", {} } };
  event.addMaskedEvent("SimpleEvent");
  event.addMappingRule({ L"Test name", L"Test function" });
  BOOST_CHECK(!(event == a.getEvent(name)));
  BOOST_CHECK_NO_THROW(a.getEvent(name) = event);
  BOOST_CHECK(event == a.getEvent(name));
  const size_t event_quantity = a.getEventQuantity();
  for (unsigned i = 1; i < 10; i++) {
    const std::string next_name = "T" + std::to_string(i);
    BOOST_CHECK_NO_THROW(a.addEvent(next_name, des::EventType::uncontrollable));
    BOOST_CHECK(a.getEventQuantity() == event_quantity + i);
    BOOST_CHECK(a.getEventInputLinksQuantity(next_name) == 0);
    BOOST_CHECK(a.getEventOutputLinksQuantity(next_name) == 0);
  }
}

// Test of addEvent with addition or replacement and getEventCopy methods (for simple events).
BOOST_AUTO_TEST_CASE(AutomationAddEventWithReplacement) {
  des::Automation a;
  des::Event event(des::EventType::controllable);
  const std::string invalid_name = "*?()";
  BOOST_CHECK_THROW(a.addEvent(invalid_name, event), std::invalid_argument);
  const std::string name = "testEvent";
  BOOST_CHECK(a.getEventInputLinksQuantity(name) == 0);
  BOOST_CHECK(a.getEventOutputLinksQuantity(name) == 0);
  BOOST_CHECK_NO_THROW(a.addEvent(name, event)); // addition with copying
  BOOST_CHECK(a.getEventQuantity() == 1);
  BOOST_CHECK(a.getEventInputLinksQuantity(name) == 0);
  BOOST_CHECK(a.getEventOutputLinksQuantity(name) == 0);
  event.viewProperties() = {L"Test hint", 5.0, 0.5, 1.0 };
  event.behaviorAttributes() = { {L"int", {} }, {L"float", {} } };
  BOOST_CHECK(!(event == a.getEventCopy(name)));
  BOOST_CHECK_NO_THROW(a.addEvent(name, event)); // replacement with copying
  BOOST_CHECK(event == a.getEventCopy(name));
  BOOST_CHECK_NO_THROW(a.addEvent("test", std::move(des::Event(des::EventType::expected)))); // addition with moving
  BOOST_CHECK_NO_THROW(a.addEvent("test", std::move(des::Event(des::EventType::expected)))); // replacement with moving
  BOOST_CHECK_THROW(a.addEvent(invalid_name, std::move(des::Event(des::EventType::expected))), std::invalid_argument);
  const size_t event_quantity = a.getEventQuantity();
  for (unsigned i = 1; i < 10; i++) {
    const std::string next_name = "T_" + std::to_string(i);
    BOOST_CHECK_NO_THROW(a.addEvent(next_name, des::Event(des::EventType::uncontrollable)));
    BOOST_CHECK(a.getEventQuantity() == event_quantity + i);
    BOOST_CHECK(a.getEventInputLinksQuantity(next_name) == 0);
    BOOST_CHECK(a.getEventOutputLinksQuantity(next_name) == 0);
  }
}

// Test of addEvent methods for macro events.
BOOST_AUTO_TEST_CASE(AutomationAddEventForMacroEvent) {
  des::Automation a;
  des::Event macro_event(des::EventType::macro);
  const std::string simple_event_name_1 = "testSimpleEvent1",  simple_event_name_2 = "testSimpleEvent2";
  macro_event.addMaskedEvent(simple_event_name_1);
  macro_event.addMaskedEvent(simple_event_name_2);
  BOOST_CHECK_NO_THROW(a.addEvent(simple_event_name_1, des::Event(des::EventType::expected)));
  const std::string macro_event_name = "testMacroEvent";
  BOOST_CHECK_THROW(a.addEvent(macro_event_name, macro_event), std::runtime_error);
  des::Event  event_copy(macro_event);
  BOOST_CHECK_THROW(a.addEvent(macro_event_name, std::move(event_copy)), std::runtime_error);
  BOOST_CHECK_NO_THROW(a.addEvent(simple_event_name_2, des::Event(des::EventType::expected)));
  BOOST_CHECK_NO_THROW(a.addEvent(macro_event_name, std::move(event_copy)));
}

// Test of checkEvent and getType methods.
BOOST_AUTO_TEST_CASE(AutomationCheckEvent) {
  des::Automation a;
  const std::string name = "testEvent";
  const des::EventType type = des::EventType::controllable;
  BOOST_CHECK(!a.checkEvent(name));
  BOOST_CHECK_THROW(auto r = a.getType(name), std::invalid_argument);
  a.addEvent(name, type);
  BOOST_CHECK(a.checkEvent(name));
  BOOST_CHECK(a.getType(name) == type);
}

// Test of getEventNameSet method.
BOOST_AUTO_TEST_CASE(AutomationGetEventsNames) {
  des::Automation a;
  const std::string event_name_1 = "testEvent1", event_name_2 = "testEvent2";
  const std::set<std::string> one_event = { event_name_1 }, two_events = { event_name_1, event_name_2 };
  BOOST_CHECK(a.getEventNameSet().empty());
  a.addEvent(event_name_1, des::EventType::macro);
  BOOST_CHECK(a.getEventNameSet() == one_event);
  a.addEvent(event_name_2, des::EventType::macro);
  BOOST_CHECK(a.getEventNameSet() == two_events);
}

// Test of removeEvent method.
BOOST_AUTO_TEST_CASE(AutomationRemoveEvent) {
  des::Automation a;
  const std::string event_name = "testEvent";
  BOOST_CHECK_NO_THROW(a.removeEvent(event_name));
  BOOST_CHECK(a.getEventInputLinksQuantity(event_name) == 0);
  BOOST_CHECK(a.getEventOutputLinksQuantity(event_name) == 0);
  BOOST_CHECK(a.getEventQuantity() == 0);
  const des::Event event(des::EventType::macro);
  a.addEvent(event_name, event);
  BOOST_CHECK(a.getEventQuantity() == 1);
  BOOST_CHECK_NO_THROW(a.removeEvent(event_name));
  BOOST_CHECK(a.getEventInputLinksQuantity(event_name) == 0);
  BOOST_CHECK(a.getEventOutputLinksQuantity(event_name) == 0);
  BOOST_CHECK(a.getEventQuantity() == 0);
  a.addEvent(event_name, event);
  const std::string state_name = "TestState";
  a.addState(state_name, 5);
  a.setLinkFromStateToEvent(state_name, event_name, 10);
  a.setLinkFromEventToState(event_name, state_name, 5);
  BOOST_CHECK_NO_THROW(a.removeEvent(event_name));
  BOOST_CHECK(a.getEventInputLinksQuantity(event_name) == 0);
  BOOST_CHECK(a.getEventOutputLinksQuantity(event_name) == 0);
  BOOST_CHECK(a.getStateInputLinkQuantity(state_name) == 0);
  BOOST_CHECK(a.getStateOutputLinkQuantity(state_name) == 0);
  BOOST_CHECK(a.getEventQuantity() == 0);
}

// Test of setLinkFromStateToEvent method.
BOOST_AUTO_TEST_CASE(AutomationSetLinkFromStateToEvent) {
  des::Automation a;
  const std::string state_name = "TestState", event_name = "testEvent";
  BOOST_CHECK_THROW(a.setLinkFromStateToEvent(state_name, event_name), std::invalid_argument);
  a.addState(state_name);
  a.addEvent(event_name, des::EventType::uncontrollable);
  BOOST_CHECK_THROW(a.setLinkFromStateToEvent(state_name, state_name), std::invalid_argument);
  BOOST_CHECK(a.getLinksFromStateToEvent(state_name, state_name) == 0);
  BOOST_CHECK_THROW(a.setLinkFromStateToEvent(event_name, event_name), std::invalid_argument);
  BOOST_CHECK(a.getLinksFromStateToEvent(event_name, event_name) == 0);
  const unsigned multiplicity = 15;
  BOOST_CHECK_NO_THROW(a.setLinkFromStateToEvent(state_name, event_name, multiplicity));
  BOOST_CHECK(a.getLinksFromStateToEvent(state_name, event_name) == multiplicity);
  BOOST_CHECK(a.getStateInputLinkQuantity(state_name) == 0);
  BOOST_CHECK(a.getStateOutputLinkQuantity(state_name) == 1);
  BOOST_CHECK(a.getEventInputLinksQuantity(event_name) == 1);
  BOOST_CHECK(a.getEventOutputLinksQuantity(event_name) == 0);
  for (unsigned i = 0; i < 10; i++) {
    BOOST_CHECK_NO_THROW(a.setLinkFromStateToEvent(state_name, event_name, i));
    BOOST_CHECK(a.getLinksFromStateToEvent(state_name, event_name) == i);
  }
  BOOST_CHECK_NO_THROW(a.setLinkFromStateToEvent(state_name, event_name, 0));
  BOOST_CHECK(a.getStateOutputLinkQuantity(state_name) == 0);
  BOOST_CHECK(a.getEventInputLinksQuantity(event_name) == 0);
}

// Test of setLinkFromEventToState method.
BOOST_AUTO_TEST_CASE(AutomationSetLinkFromEventToState) {
  des::Automation a;
  const std::string state_name = "TestState", event_name = "testEvent";
  BOOST_CHECK_THROW(a.setLinkFromEventToState(event_name, state_name), std::invalid_argument);
  a.addState(state_name);
  a.addEvent(event_name, des::EventType::uncontrollable);
  BOOST_CHECK_THROW(a.setLinkFromEventToState(state_name, state_name), std::invalid_argument);
  BOOST_CHECK(a.getLinksFromEventToState(state_name, state_name) == 0);
  BOOST_CHECK_THROW(a.setLinkFromEventToState(event_name, event_name), std::invalid_argument);
  BOOST_CHECK(a.getLinksFromEventToState(event_name, event_name) == 0);
  const unsigned multiplicity = 15;
  BOOST_CHECK_NO_THROW(a.setLinkFromEventToState(event_name, state_name, multiplicity));
  BOOST_CHECK(a.getLinksFromEventToState(event_name, state_name) == multiplicity);
  BOOST_CHECK(a.getEventInputLinksQuantity(event_name) == 0);
  BOOST_CHECK(a.getEventOutputLinksQuantity(event_name) == 1);
  BOOST_CHECK(a.getStateInputLinkQuantity(state_name) == 1);
  BOOST_CHECK(a.getStateOutputLinkQuantity(state_name) == 0);
  for (unsigned i = 0; i < 10; i++) {
    BOOST_CHECK_NO_THROW(a.setLinkFromEventToState(event_name, state_name, i));
    BOOST_CHECK(a.getLinksFromEventToState(event_name, state_name) == i);
  }
  BOOST_CHECK_NO_THROW(a.setLinkFromEventToState(event_name, state_name, 0));
  BOOST_CHECK(a.getEventOutputLinksQuantity(event_name) == 0);
  BOOST_CHECK(a.getStateInputLinkQuantity(state_name) == 0);
}

// Test of getInputsOfEvent, getOutputsOfEvent, getInputsOfState, getOutputsOfState methods.
BOOST_AUTO_TEST_CASE(AutomationGetInputsAndOutputs) {
  des::Automation a;
  const std::string state_name_1 = "TestState1", state_name_2 = "TestState2";
  const std::string event_name_1 = "testEvent1", event_name_2 = "testEvent2";
  const std::set<std::string> empty = {}, one_event = { event_name_1 }, one_state = { state_name_1 };
  const std::set<std::string> two_events = { event_name_1, event_name_2 }, two_states = { state_name_1, state_name_2 };
  const std::string invalid_name = "invalid name";
  BOOST_CHECK(a.getEventInputs(invalid_name) == empty);
  BOOST_CHECK(a.getEventOutputs(invalid_name) == empty);
  BOOST_CHECK(a.getStateInputs(invalid_name) == empty);
  BOOST_CHECK(a.getStateOutputs(invalid_name) == empty);
  a.addState(state_name_1, 1);
  a.addState(state_name_2);
  a.addEvent(event_name_1, des::EventType::uncontrollable);
  a.addEvent(event_name_2, des::EventType::uncontrollable);
  a.setLinkFromStateToEvent(state_name_1, event_name_1);
  a.setLinkFromEventToState(event_name_1, state_name_2);
  a.setLinkFromEventToState(event_name_2, state_name_2);
  a.setLinkFromEventToState(event_name_2, state_name_1);
  BOOST_CHECK(a.getStateOutputs(state_name_1) == one_event);
  BOOST_CHECK(a.getEventInputs(event_name_1) == one_state);
  BOOST_CHECK(a.getStateInputs(state_name_2) == two_events);
  BOOST_CHECK(a.getEventOutputs(event_name_2) == two_states);
}

// Test of linkStatesByEvent method.
BOOST_AUTO_TEST_CASE(AutomatioLinkStateByEvent) {
  des::Automation a;
  const std::string state_name_1 = "TestState1", state_name_2 = "TestState2", event_name = "testEvent";
  a.addState(state_name_1, 1);
  a.addState(state_name_2);
  a.addEvent(event_name, des::EventType::uncontrollable);
  a.setLinkFromEventToState(event_name, state_name_1, 3);
  a.setLinkFromEventToState(event_name, state_name_2, 3);
  BOOST_CHECK_NO_THROW(a.linkStatesByEvent(state_name_1, event_name, state_name_2));
  BOOST_CHECK(a.getLinksFromStateToEvent(state_name_1, event_name) == 1);
  BOOST_CHECK(a.getLinksFromEventToState(event_name, state_name_2) == 1);
  BOOST_CHECK(a.getLinksFromEventToState(event_name, state_name_1) == 3);
  BOOST_CHECK_THROW(a.linkStatesByEvent("invalid", event_name, state_name_1), std::invalid_argument);
}

// Test of checkMacro method.
BOOST_AUTO_TEST_CASE(AutomationCheckMacro) {
  des::Automation a;
  const std::string state_name = "TestState";
  const std::string event_name_1 = "testEvent1", event_name_2 = "testEvent2", macro_event_name = "testMacro";
  a.addEvent(macro_event_name, des::EventType::macro);
  BOOST_CHECK(a.checkMacro());
  a.getEvent(macro_event_name).addMaskedEvent(event_name_1);
  BOOST_CHECK(!a.checkMacro());
  a.addState(state_name, 1);
  a.addEvent(event_name_1, des::EventType::uncontrollable);
  a.addEvent(event_name_2, des::EventType::uncontrollable);
  BOOST_CHECK(a.checkMacro());
  a.setLinkFromStateToEvent(state_name, macro_event_name);
  BOOST_CHECK(!a.checkMacro()); // input link to macro event
  a.setLinkFromStateToEvent(state_name, macro_event_name, 0);
  a.setLinkFromEventToState(macro_event_name, state_name);
  BOOST_CHECK(!a.checkMacro()); // output link to macro event
  a.setLinkFromEventToState(macro_event_name, state_name, 0);
  BOOST_CHECK(a.checkMacro());
}

// Test of getReadyEvents method.
BOOST_AUTO_TEST_CASE(AutomationReadyEvents) {
  des::Automation a;
  const std::string state_name_1 = "TestState1", state_name_2 = "TestState2";
  const std::string event_name_1 = "testEvent1", event_name_2 = "testEvent2";
  const std::set<std::string> one_event_set = { event_name_1 }, two_events_set = { event_name_1, event_name_2 };
  a.addState(state_name_1, 1);
  a.addState(state_name_2);
  a.addEvent(event_name_1, des::EventType::uncontrollable);
  a.addEvent(event_name_2, des::EventType::uncontrollable);
  a.setLinkFromStateToEvent(state_name_1, event_name_1, 2);
  BOOST_CHECK(a.getReadyEvents().empty());
  a.setLinkFromStateToEvent(state_name_1, event_name_1, 1);
  BOOST_CHECK(a.getReadyEvents() == one_event_set);
  a.setLinkFromStateToEvent(state_name_2, event_name_1, 1);
  BOOST_CHECK(a.getReadyEvents().empty());
  a.setActivity(state_name_2, 2);
  BOOST_CHECK(a.getReadyEvents() == one_event_set);
  a.setLinkFromStateToEvent(state_name_2, event_name_2, 1);
  BOOST_CHECK(a.getReadyEvents() == two_events_set);
}

// Test of fire method.
BOOST_AUTO_TEST_CASE(AutomationFire) {
  des::Automation a;
  BOOST_CHECK_NO_THROW(a.fire()); // empty automation
  const std::string state_name_1 = "TestState1", state_name_2 = "TestState2";
  const std::string event_name_1 = "testEvent1", event_name_2 = "testEvent2";
  a.addState(state_name_1, 1);
  a.addState(state_name_2);
  a.addEvent(event_name_1, des::EventType::uncontrollable);
  a.addEvent(event_name_2, des::EventType::uncontrollable);
  BOOST_CHECK_NO_THROW(a.fire());
  a.linkStatesByEvent(state_name_1, event_name_1, state_name_2);
  a.linkStatesByEvent(state_name_2, event_name_2, state_name_1);
  BOOST_CHECK_THROW(a.fire(event_name_2), std::invalid_argument);
  BOOST_CHECK_NO_THROW(a.fire());
  BOOST_CHECK(a.getState(state_name_1).activity() == 0);
  BOOST_CHECK(a.getState(state_name_2).activity() == 1);
  BOOST_CHECK_NO_THROW(a.fire(event_name_2));
  BOOST_CHECK(a.getState(state_name_1).activity() == 1);
  BOOST_CHECK(a.getState(state_name_2).activity() == 0);
  a.setActivity(state_name_1, 0);
  BOOST_CHECK_NO_THROW(a.fire());
  BOOST_CHECK(a.getState(state_name_1).activity() == 0);
  BOOST_CHECK(a.getState(state_name_2).activity() == 0);
}
