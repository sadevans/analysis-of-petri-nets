/*! @file components.cpp
DES model components classes source file.
@authors A. Kozov
@date 2021/12/15 */

#include "components.hpp"
#include "state.hpp"
#include "event.hpp"


using namespace std;
using namespace des;

// Equivalence check of two des::ViewProperties objects.
bool des::operator==(const ViewProperties& l, const ViewProperties& r) noexcept {
  return l.Hint == r.Hint && l.X == r.X && l.Y == r.Y && l.Angle == r.Angle;
}

// Equivalence check of two des::BehaviorAttributeRestriction objects.
bool des::operator==(const BehaviorAttributeRestriction& l, const BehaviorAttributeRestriction& r) noexcept {
  return l.Name == r.Name && l.Function == r.Function;
}

// Equivalence check of two des::BehaviorAttribute objects.
bool des::operator==(const BehaviorAttribute& l, const BehaviorAttribute& r) noexcept {
  return l.Type == r.Type && l.Restrictions == r.Restrictions;
}

// Correctness check of name string.
bool des::checkNameString(const string& s) {
  const static regex r("[[:alpha:]][[:alpha:][:digit:]_/]*");
  return s.length() < max_name_length && regex_match(s, r);
}


// State class.

// Constructor of the des::State object.
State::State(const unsigned& a) : _activity(a), _properties() {
}

// Activity token setting.
void State::setActivity(const unsigned& a) noexcept {
  _activity = a;
}

// Equivalence check of two des::State objects.
bool State::operator==(const State& s) const noexcept {
  return _activity == s._activity && _properties == s._properties;
}


// Event class.

// Constructor of the des::Event object.
Event::Event(const EventType& t) : _type(t), _properties(), _attributes(), _masked_events(), _mapping_rules() {
}

// Copy of masked event list from macro event.
std::list<string> Event::getMaskedEvents() const {
  if (_type != EventType::macro) {
    throw std::logic_error("getMaskedEvents: event isn't macro event");
  }
  return _masked_events;
}

// Masked event list setting.
void Event::setMaskedEvents(const std::list<string>& l) {
  if (_type != EventType::macro) {
    throw std::logic_error("setMaskedEvents: event isn't macro event");
  }
  _masked_events = l;
}

// Masked event addition.
void Event::addMaskedEvent(const string& e) {
  if (_type != EventType::macro) {
    throw std::logic_error("addMaskedEvent: event isn't macro event");
  }
  _masked_events.emplace(_masked_events.end(), e);
}

// Copy of mapping rule list from macro event.
std::list<BehaviorAttributeRestriction> Event::getMappingRules() const {
  if (_type != EventType::macro) {
    throw std::logic_error("getMappingRules: event isn't macro event");
  }
  return _mapping_rules;
}

// Mapping rule list setting.
void Event::setMappingRules(const std::list<BehaviorAttributeRestriction>& l) {
  if (_type != EventType::macro) {
    throw std::logic_error("setMappingRules: event isn't macro event");
  }
  _mapping_rules = l;
}

// Mapping rule addition.
void Event::addMappingRule(const BehaviorAttributeRestriction& r) {
  if (_type != EventType::macro) {
    throw std::logic_error("addMappingRule: event isn't macro event");
  }
  _mapping_rules.emplace(_mapping_rules.end(), r);
}

// Equivalence check of two des::Event objects.
bool Event::operator==(const Event& e) const noexcept {
  const bool& s = _type == e._type && _properties == e._properties && _attributes == e._attributes;
  return s && _masked_events == e._masked_events && _mapping_rules == e._mapping_rules;
}

// Controllable event check.
bool des::isControllable(const Event& e) {
  return e.type() == EventType::controllable;
}

// Uncontrollable event check.
bool des::isUncontrollable(const Event& e) {
  return e.type() != EventType::controllable;
}

// Macro event check.
bool des::isMacro(const Event& e) {
  return e.type() == EventType::macro;
}
