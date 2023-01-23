/*! @file event_tests.cpp
@ref des::Event class tests source file.
@authors A. Kozov
@date 2021/12/19 */

#include <boost/test/unit_test.hpp>

#include "event.hpp"


// Test of Event default constructor.
BOOST_AUTO_TEST_CASE(CreateEvent) {
  des::Event simple_event(des::EventType::controllable);
  BOOST_CHECK(simple_event.type() == des::EventType::controllable);
  des::ViewProperties p; // default view properties values
  BOOST_CHECK(simple_event.viewProperties() == p);
  BOOST_CHECK(simple_event.behaviorAttributes().empty());
  BOOST_CHECK_THROW(auto r = simple_event.getMaskedEvents(), std::logic_error);
  BOOST_CHECK_THROW(auto r = simple_event.getMappingRules(), std::logic_error);
  des::Event macro_event(des::EventType::macro);
  BOOST_CHECK(macro_event.type() == des::EventType::macro);
  BOOST_CHECK_NO_THROW(auto r = macro_event.getMaskedEvents());
  BOOST_CHECK(macro_event.getMaskedEvents().empty());
  BOOST_CHECK_NO_THROW(auto r = macro_event.getMappingRules());
  BOOST_CHECK(macro_event.getMappingRules().empty());
}

// Test of Event copy and move constructors.
BOOST_AUTO_TEST_CASE(CreateEventByOtherEvent) {
  const des::EventType type = des::EventType::macro;
  const des::ViewProperties properties = { L"Test hint", 15.0, 5.0, 0.5 };
  const std::list<des::BehaviorAttribute> attributes = { { L"int" }, { L"float" } };
  const std::list<std::string> event_list = { "TestEvent1", "TestEvent1" };
  const std::list<des::BehaviorAttributeRestriction> mapping_rules = { { L"fail", L"" } };
  des::Event other(type);
  other.viewProperties() = properties;
  other.behaviorAttributes() = attributes;
  other.setMaskedEvents(event_list);
  other.setMappingRules(mapping_rules);
  des::Event copy_result(other);
  BOOST_CHECK(copy_result.type() == type);
  BOOST_CHECK(copy_result.viewProperties() == properties);
  BOOST_CHECK(copy_result.behaviorAttributes() == attributes);
  BOOST_CHECK(copy_result.getMaskedEvents() == event_list);
  BOOST_CHECK(copy_result.getMappingRules() == mapping_rules);
  des::Event move_result(std::move(other));
  BOOST_CHECK(move_result.type() == type);
  BOOST_CHECK(move_result.viewProperties() == properties);
  BOOST_CHECK(move_result.behaviorAttributes() == attributes);
  BOOST_CHECK(move_result.getMaskedEvents() == event_list);
  BOOST_CHECK(move_result.getMappingRules() == mapping_rules);
}

// Test of Event copy and move assignments.
BOOST_AUTO_TEST_CASE(AssignEventByOtherEvent) {
  const des::EventType type = des::EventType::macro;
  const des::ViewProperties properties = { L"Test hint", 15.0, 5.0, 0.5 };
  const std::list<des::BehaviorAttribute> attributes = { { L"int" }, { L"float" } };
  const std::list<std::string> event_list = { "TestEvent1", "TestEvent1" };
  const std::list<des::BehaviorAttributeRestriction> mapping_rules = { { L"fail", L"" } };
  des::Event other(type);
  other.viewProperties() = properties;
  other.behaviorAttributes() = attributes;
  other.setMaskedEvents(event_list);
  other.setMappingRules(mapping_rules);
  des::Event result(des::EventType::controllable);
  BOOST_CHECK_NO_THROW(result = other);
  BOOST_CHECK(result.type() == type);
  BOOST_CHECK(result.viewProperties() == properties);
  BOOST_CHECK(result.behaviorAttributes() == attributes);
  BOOST_CHECK(result.getMaskedEvents() == event_list);
  BOOST_CHECK(result.getMappingRules() == mapping_rules);
  BOOST_CHECK_NO_THROW(result = std::move(other));
  BOOST_CHECK(result.type() == type);
  BOOST_CHECK(result.viewProperties() == properties);
  BOOST_CHECK(result.behaviorAttributes() == attributes);
  BOOST_CHECK(result.getMaskedEvents() == event_list);
  BOOST_CHECK(result.getMappingRules() == mapping_rules);
}

// Test of Event behaviorAttributes method.
BOOST_AUTO_TEST_CASE(EventBehaviorAttributes) {
  const des::BehaviorAttribute attribute_int = { L"int", {} }, attribute_float = { L"float", {} };
  std::list<des::BehaviorAttribute> attributes;
  attributes.push_back(attribute_int);
  attributes.push_back(attribute_float);
  des::Event event(des::EventType::controllable);
  BOOST_CHECK_NO_THROW(event.behaviorAttributes() = attributes);
  BOOST_CHECK(event.behaviorAttributes().size() == 2);
  BOOST_CHECK(event.behaviorAttributes().front() == attribute_int);
  BOOST_CHECK(event.behaviorAttributes().back() == attribute_float);
  BOOST_CHECK_NO_THROW(event.behaviorAttributes().pop_back());
  BOOST_CHECK(event.behaviorAttributes().size() == 1);
  BOOST_CHECK(event.behaviorAttributes().back() == attribute_int);
  BOOST_CHECK_NO_THROW(event.behaviorAttributes().clear());
  BOOST_CHECK(event.behaviorAttributes().empty());
}

// Test of Event getMaskedEvent, setMaskedEvent and addMaskedEvent methods.
BOOST_AUTO_TEST_CASE(EventMaskedEvents) {
  des::Event simple_event(des::EventType::uncontrollable); // simple event
  BOOST_CHECK_THROW(auto r = simple_event.getMaskedEvents(), std::logic_error);
  BOOST_CHECK_THROW(simple_event.setMaskedEvents({}), std::logic_error);
  BOOST_CHECK_THROW(simple_event.addMaskedEvent("TestEvent"), std::logic_error);
  const std::string event_name_1 = "TestEvent1", event_name_2 = "TestEvent2";
  des::Event macro_event(des::EventType::macro); // macro event
  BOOST_CHECK_NO_THROW(auto r = macro_event.getMaskedEvents());
  BOOST_CHECK(macro_event.getMaskedEvents().empty());
  BOOST_CHECK_NO_THROW(macro_event.setMaskedEvents({}));
  BOOST_CHECK(macro_event.getMaskedEvents().empty());
  std::list<std::string> events_list = { event_name_1 };
  BOOST_CHECK_NO_THROW(macro_event.setMaskedEvents(events_list));
  BOOST_CHECK(macro_event.getMaskedEvents() == events_list);
  BOOST_CHECK_NO_THROW(macro_event.addMaskedEvent(event_name_2));
  events_list.push_back(event_name_2);
  BOOST_CHECK(macro_event.getMaskedEvents() == events_list);
}

// Test of Event getMappingRules, setMappingRules and addMappingRule methods.
BOOST_AUTO_TEST_CASE(EventMappingRules) {
  des::Event simple_event(des::EventType::uncontrollable); // simple event
  BOOST_CHECK_THROW(auto r = simple_event.getMappingRules(), std::logic_error);
  BOOST_CHECK_THROW(simple_event.setMappingRules({}), std::logic_error);
  BOOST_CHECK_THROW(simple_event.addMappingRule({ L"Name", L"Function" }), std::logic_error);
  des::Event macro_event(des::EventType::macro); // macro event
  BOOST_CHECK_NO_THROW(auto r = macro_event.getMappingRules());
  BOOST_CHECK(macro_event.getMappingRules().empty());
  BOOST_CHECK_NO_THROW(macro_event.setMappingRules({}));
  BOOST_CHECK(macro_event.getMappingRules().empty());
  std::list<des::BehaviorAttributeRestriction> rules_list = { { L"Name1", L"1" }, { L"Name2", L"2" } };
  BOOST_CHECK_NO_THROW(macro_event.setMappingRules(rules_list));
  BOOST_CHECK(macro_event.getMappingRules() == rules_list);
  des::BehaviorAttributeRestriction rule = { L"Name3", L"3" };
  BOOST_CHECK_NO_THROW(macro_event.addMappingRule(rule));
  rules_list.push_back(rule);
  BOOST_CHECK(macro_event.getMappingRules() == rules_list);
}

// Test of check Event type functions.
BOOST_AUTO_TEST_CASE(CheckEventTypeFunctions) {
  des::Event controllable(des::EventType::controllable);
  des::Event uncontrollable(des::EventType::uncontrollable);
  des::Event expected(des::EventType::expected);
  des::Event macro(des::EventType::macro);
  BOOST_CHECK(des::isControllable(controllable));
  BOOST_CHECK(!des::isControllable(uncontrollable));
  BOOST_CHECK(!des::isControllable(expected));
  BOOST_CHECK(!des::isControllable(macro));
  BOOST_CHECK(!des::isUncontrollable(controllable));
  BOOST_CHECK(des::isUncontrollable(uncontrollable));
  BOOST_CHECK(des::isUncontrollable(expected));
  BOOST_CHECK(des::isUncontrollable(macro));
  BOOST_CHECK(!des::isMacro(controllable));
  BOOST_CHECK(!des::isMacro(uncontrollable));
  BOOST_CHECK(!des::isMacro(expected));
  BOOST_CHECK(des::isMacro(macro));
}

// Test of Event equivalence.
BOOST_AUTO_TEST_CASE(CompareEvents) {
  des::Event event(des::EventType::macro);
  event.viewProperties() = { L"Test hint", 5.0, 0.5, 1.0 };
  event.behaviorAttributes() = { { L"int", {} }, { L"float", {} } };
  event.addMaskedEvent("TestEvent");
  event.addMappingRule({ L"Test name", L"Test function" });
  BOOST_CHECK(!(event == des::Event(des::EventType::controllable)));
  des::Event other_event(event.type());
  BOOST_CHECK(!(event == other_event));
  other_event.viewProperties() = event.viewProperties();
  BOOST_CHECK(!(event == other_event));
  other_event.behaviorAttributes() = event.behaviorAttributes();
  BOOST_CHECK(!(event == other_event));
  other_event.setMaskedEvents(event.getMaskedEvents());
  BOOST_CHECK(!(event == other_event));
  other_event.setMappingRules(event.getMappingRules());
  BOOST_CHECK(event == other_event);
}
