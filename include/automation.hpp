/*! @file automation.hpp
@ref des::Automation class header file.
@authors A. Kozov
@date 2021/12/15 */

#ifndef AUTOMATION_HPP
#define AUTOMATION_HPP

#include <algorithm>
#include <exception>
#include <limits>
#include <map>
#include <set>
#include <string>

#include "event.hpp"
#include "state.hpp"


/// Namespace of DES model.
namespace des {

  /*! Class of an automation (state machine) corresponding to a Petri net.
  @details This class contains dictionaries with states, events and their relations. The states are @ref State objects
  and they correspond to Petri net places and can have activity tokens as Petri net tokens. The events are @ref Event
  objects, they correspond to Petri net transitions and can be fired as such transitions. Each state and each event has
  a unique (within an automation) name, input and output links. Input links of an event are output links of a state and
  input links of a state are an output links of an event. The links between states and events form transition graph of
  an automation. There aren't links between two states or links between two events. Quantity of links and quantity of
  activity tokens in each state are limited to UINT_MAX value. */
  class Automation {
  public:

    /*! Constructs an @ref Automation object by copying of other Automation object. */
    Automation(const Automation&) = default;

    /*! Constructs an @ref Automation object by moving of other Automation object. */
    Automation(Automation&&) = default;

    /*! Constructs an empty @ref Automation object.
    @details Default constructor creates an @ref Automation object without states and events. */
    Automation();

    /*! Adds new unconnected state into the automation.
    @details This method validates the name @a n by @ref checkNameString function, check uniqueness of name, creates
    and adds new state into the automation.
    @param n Unique name of state.
    @param a Quantity of activity tokens for state.
    @throw std::invalid_argument Invalid or existing name of state. */
    void addState(const std::string& n, const unsigned& a = 0);

    /*! Adds new unconnected state into the automation or _replaces_ existing state without links change.
    @details This method validates the name @a n by @ref checkNameString function before new state addition or existing
    state replacement.
    @param n Name of state.
    @param s State to addition.
    @throw std::invalid_argument Invalid name of state. */
    void addState(const std::string& n, const State& s);

    /*! Adds new unconnected state into the automation or _replaces_ existing state without links change.
    @details This method validates the name @a n by @ref checkNameString function before new state addition or existing
    state replacement.
    @param n Name of state.
    @param s State to addition (move semantic).
    @throw std::invalid_argument Invalid name of state. */
    void addState(const std::string& n, State&& s);

    /*! Returns true for state name existing in the automation.
    @param n Name of state for check.
    @return Result of the state name check. */
    [[nodiscard]] inline bool checkState(const std::string& n) const noexcept {
      return _states.find(n) != _states.end();
    }

    /*! Returns existing state of the automation by name.
    @param n Name of state.
    @return Reference to the existing state.
    @throw std::invalid_argument Nonexistent state name. */
    [[nodiscard]] State& getState(const std::string& n);

    /*! Returns copy of existing state of the automation by name.
    @param n Name of state.
    @return Copy of the existing state.
    @throw std::invalid_argument Nonexistent state name. */
    [[nodiscard]] inline State getStateCopy(const std::string& n) const {
      return const_cast<Automation*>(this)->getState(n);
    }

    /*! Returns quantity of states in the automation.
    @return Quantity of states. */
    [[nodiscard]] inline size_t getStateQuantity() const noexcept {
      return _states.size();
    }

    /*! Returns set of state names in the automation.
    @return Set of state names. */
    [[nodiscard]] std::set<std::string> getStateNameSet() const noexcept;

    /*! Changes quantity of activity tokens in specified state.
    @param n Name of state for change.
    @param a New quantity of activity tokens.
    @throw std::invalid_argument Nonexistent state name. */
    void setActivity(const std::string& n, const unsigned& a);

    /*! Returns quantity of activity tokens in specified state.
    @param n Name of state.
    @return Quantity of activity tokens. */
    [[nodiscard]] inline unsigned getActivity(const std::string& n) const noexcept {
      if (_states.find(n) != _states.end()) {
        return _states.at(n).Component.activity();
      }
      return 0;
    }

    /*! Returns quantity of input links for specified state.
    @param n Name of state.
    @return Quantity of input links. */
    [[nodiscard]] size_t getStateInputLinkQuantity(const std::string& n) const noexcept;

    /*! Returns quantity of output links for specified state.
    @param n Name of state.
    @return Quantity of output links. */
    [[nodiscard]] size_t getStateOutputLinkQuantity(const std::string& n) const noexcept;

    /*! Returns set of input event names for specified state.
    @param n Name of state.
    @return Set of input event names. */
    [[nodiscard]] std::set<std::string> getStateInputs(const std::string& n) const noexcept;

    /*! Returns set of output event names for specified state.
    @param n Name of state.
    @return Set of output event names. */
    [[nodiscard]] std::set<std::string> getStateOutputs(const std::string& n) const noexcept;

    /*! Removes state from the automation.
    @details This method removes state with specified name. Nothing happens, if the required state doesn't exist.
    @param n Name of removing state. */
    void removeState(const std::string& n) noexcept;

    /*! Adds new unconnected event into the automation.
    @details This method validates the name @a n by @ref checkNameString function, check uniqueness of name, creates
    and adds new event with specified type into the automation.
    @param n Unique name of event.
    @param t Type of event.
    @throw std::invalid_argument Invalid or existing name of event. */
    void addEvent(const std::string& n, const EventType& t);

    /*! Adds new unconnected event into the automation or _replaces_ existing event without links change.
    @details This method validates the name @a n by @ref checkNameString function and validates the event @a e before
    new event addition or existing event replacement.
    @param n Name of event.
    @param e Event to addition.
    @throw std::invalid_argument Invalid name of event.
    @throw std::runtime_error Invalid masked events list of macro event. */
    void addEvent(const std::string& n, const Event& e);

    /*! Adds new unconnected event into the automation or _replaces_ existing event without links change.
    @details This method validates the name @a n by @ref checkNameString function and validates the event @a e before
    new event addition or existing event replacement.
    @param n Name of event.
    @param e Event to addition (move semantic).
    @throw std::invalid_argument Invalid name of event.
    @throw std::runtime_error Invalid masked events list of macro event. */
    void addEvent(const std::string& n, Event&& e);

    /*! Returns true for existing event name.
    @param n Name of event for check.
    @return Result of the event name check. */
    [[nodiscard]] inline bool checkEvent(const std::string& n) const noexcept {
      return _events.find(n) != _events.end();
    }

    /*! Returns existing event of the automation by name.
    @param n Name of event.
    @return Reference to the existing event.
    @throw std::invalid_argument Nonexistent event name. */
    [[nodiscard]] Event& getEvent(const std::string& n);

    /*! Returns copy of existing event of the automation by name.
    @param n Name of event.
    @return Copy of the existing event.
    @throw std::invalid_argument Nonexistent event name. */
    [[nodiscard]] inline Event getEventCopy(const std::string& n) const {
      return const_cast<Automation*>(this)->getEvent(n);
    }

    /*! Returns quantity of events in the automation.
    @return Quantity of events. */
    [[nodiscard]] inline size_t getEventQuantity() const noexcept {
      return _events.size();
    }

    /*! Returns set of event names in the automation.
    @return Set of event names. */
    [[nodiscard]] std::set<std::string> getEventNameSet() const noexcept;

    /*! Returns quantity of input links for specified event.
    @param n Name of event.
    @return Quantity of input links. */
    [[nodiscard]] size_t getEventInputLinksQuantity(const std::string& n) const noexcept;

    /*! Returns quantity of output links for specified event.
    @param n Name of event.
    @return Quantity of output links. */
    [[nodiscard]] size_t getEventOutputLinksQuantity(const std::string& n) const noexcept;

    /*! Returns set of input state names for specified event.
    @param n Name of event.
    @return Set of input state names. */
    [[nodiscard]] std::set<std::string> getEventInputs(const std::string& n) const noexcept;

    /*! Returns set of output state names for specified event.
    @param n Name of event.
    @return Set of output state names. */
    [[nodiscard]] std::set<std::string> getEventOutputs(const std::string& n) const noexcept;

    /*! Returns type of specified event.
    @param n Name of event.
    @return Type of the event.
    @throw std::invalid_argument Nonexistent event name. */
    [[nodiscard]] EventType getType(const std::string& n) const;

    /*! Removes event from the automation.
    @details This method removes event with specified name. Nothing happens, if the required event doesn't exist.
    @param n Name of removing event. */
    void removeEvent(const std::string& n) noexcept;

    /*! Changes link from state to event.
    @details This method sets the multiplicity @a m for link from the state named @a s to the event named @a e in the
    automation. The method removes link, if the setting multiplicity @a m is equal to zero.
    @param s Name of linking state.
    @param e Name of linking event.
    @param m Multiplicity of link.
    @throw std::invalid_argument Nonexistent name of state or nonexistent name of event. */
    void setLinkFromStateToEvent(const std::string& s, const std::string& e, const unsigned& m = 1);

    /*! Returns multiplicity of link from state to event.
    @param s Name of state.
    @param e Name of event.
    @return Multiplicity value of existing link or zero. */
    [[nodiscard]] unsigned getLinksFromStateToEvent(const std::string& s, const std::string& e) const noexcept;

    /*! Changes link from event to state.
    @details This method sets the multiplicity @a m for link from the event named @a e to the state named @a s in the
    automation. The method removes link, if the setting multiplicity @a m is equal to zero.
    @param e Name of linking event.
    @param s Name of linking state.
    @param m Multiplicity of link.
    @throw std::invalid_argument Nonexistent name of event or nonexistent name of state. */
    void setLinkFromEventToState(const std::string& e, const std::string& s, const unsigned& m = 1);

    /*! Returns multiplicity of link from event to state.
    @param e Name of event.
    @param s Name of state.
    @return Multiplicity value of existing link or zero. */
    [[nodiscard]] unsigned getLinksFromEventToState(const std::string& e, const std::string& s) const noexcept;

    /*! Changes two links, from state to event and from event to state.
    @details This method sets two links with single multiplicity. The first links the state named @a f to the event
    named @a e, the second links the same event to the state named @a s.
    @param f Name of first linking state.
    @param e Name of linking event.
    @param s Name of second linking state.
    @throw std::invalid_argument Nonexistent names of states or nonexistent name of event. */
    void linkStatesByEvent(const std::string& f, const std::string& e, const std::string& s);

    /*! Returns true for the automation with correct macro events.
    @details This method checks macro events and returns true, if all macro events don't have any links and all their
    masked event lists contain only event names that exist in the automation.
    @return A result of macro events check. */
    [[nodiscard]] bool checkMacro() const noexcept;

    /*! Returns set of ready event names.
    @details This method checks all events of the automation, makes and returns set of names of ready events. An event
    is ready (ready to fire), if all of their input states have at least as many activity tokens as the multiplicity of
    corresponding link to this event. Any event of the result set can fire, but only one.
    @return Set of ready event names. */
    [[nodiscard]] std::set<std::string> getReadyEvents() const noexcept;

    /*! Passes the automation to next state.
    @details This method checks events and advances the automation to next state by firing of one of the ready events.
    If the event name @a e is specified, is non-empty, and belongs to a set of ready event names, the method fires this
    event. Nothing happens, if the name @a e is empty and there are no ready events.
    @param e Name of firing event or empty string.
    @throw std::invalid_argument Invalid name of firing event. */
    void fire(const std::string& e = "");

  protected:

    /*! Internal template class storing automation components and their relations.
    @details This class contains one object of automation component (state or event) and dictionaries of its input and
    output links to other components. */
    template<class T>
    class ComponentWithLinks {
    public:

      /*! Constructs a @ref ComponentWithLinks object by copying of other @ref ComponentWithLinks object. */
      ComponentWithLinks(const ComponentWithLinks&) = default;

      /*! Constructs a @ref ComponentWithLinks object by moving of other @ref ComponentWithLinks object. */
      ComponentWithLinks(ComponentWithLinks&&) noexcept = default;

      /*! Assigns new value of a @ref ComponentWithLinks object by copying of other @ref ComponentWithLinks object. */
      ComponentWithLinks& operator=(const ComponentWithLinks&) = default;

      /*! Assigns new value of a @ref ComponentWithLinks object by moving of other @ref ComponentWithLinks object. */
      ComponentWithLinks& operator=(ComponentWithLinks&&) noexcept = default;

      /*! Constructs a @ref ComponentWithLinks object with empty links dictionaries by copying of specified component.
      @param c Component for store. */
      explicit ComponentWithLinks(const T& c) : Component(c), Inputs(), Outputs() {}

      /*! Constructs a @ref ComponentWithLinks object with empty links dictionaries by moving of specified component.
      @param c Component for store (move semantic). */
      explicit ComponentWithLinks(T&& c) : Component(std::move(c)), Inputs(), Outputs() {}

      T Component;                             ///< Main component.
      std::map<std::string, unsigned> Inputs;  ///< Input link dictionary of component.
      std::map<std::string, unsigned> Outputs; ///< Output link dictionary of component.

    }; // ComponentWithLinks class

    /*! Returns true for macro event with correct masked event list.
    @details This method returns true, if specified event isn't macro or if all event names in its masked event list
    exist in the automation.
    @param e @ref Event object for check.
    @return Result of the event check. */
    [[nodiscard]] bool checkMacroEvent(const Event& e) const noexcept;

    std::map<std::string, ComponentWithLinks<State>> _states; ///< States dictionary of automation.
    std::map<std::string, ComponentWithLinks<Event>> _events; ///< Events dictionary of automation.

  }; // Automation class

} // namespace


#endif // AUTOMATION_HPP
