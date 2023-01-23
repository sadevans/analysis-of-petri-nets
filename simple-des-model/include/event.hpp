/*! @file event.hpp
@ref des::Event class header file.
@authors A. Kozov
@date 2021/12/15 */

#ifndef EVENT_HPP
#define EVENT_HPP

#include <exception>
#include <list>
#include <string>

#include "components.hpp"

// Namespace of DES model.
namespace des {

  /*! Enumeration of event types.
  EventType::expected and EventType::macro are an uncontrollable event types of supervisory control theory. */
  enum class EventType {
    controllable,   ///< Controllable event.
    uncontrollable, ///< Uncontrollable event.
    expected,       ///< Expected event (for domino method).
    macro           ///< Macro event (for state space reducing).
  };

  /*! Class of a DES event corresponding a Petri net transition.
  @details This class contains event type, view properties, list of behavior attributes, list of masked event names,
  list of mapping rules for masked events. Only macro event can have non-empty lists of masked event names or mapping
  rules. */
  class Event {
  public:

    /*! Constructs a @ref Event object by copying of other @ref Event object. */
    Event(const Event&) = default;

    /*! Constructs a @ref Event object by moving of other @ref Event object. */
    Event(Event&&) = default;

    /*! Assigns new value of a @ref Event object by copying of other @ref Event object. */
    Event& operator=(const Event&) = default;

    /*! Assigns new value of a @ref Event object by moving of other @ref Event object. */
    Event& operator=(Event&&) = default;

    /*! Constructs a event object with specified type.
    @details Constructor creates @ref Event object with specified type, default view properties, empty list of behavior
    attributes, empty list of masked event names and empty list of mapping rules.
    @param t Type of event.*/
    explicit Event(const EventType& t);

    /*! Returns type of the event.
    @return Type of the event. */
    [[nodiscard]] inline EventType type() const noexcept {
      return _type;
    }

    /*! Returns view properties of event.
    @return Reference to the event view properties. */
    inline ViewProperties& viewProperties() noexcept {
      return _properties;
    }

    /*! Returns behavior attribute list of the event.
    @return Reference to the event behavior attribute list. */
    inline std::list<BehaviorAttribute>& behaviorAttributes() noexcept {
      return _attributes;
    }

    /*! Returns copy of masked event name list from the macro event.
    @return List of masked event names.
    @throw std::logic_error Invalid event type (type isn't @ref EventType::macro). */
    [[nodiscard]] std::list<std::string> getMaskedEvents() const;

    /*! Sets new masked event name list into the macro event.
    @details This method replaces current masked event name list in the macro event with specified list.
    @param l Event name list for setting.
    @throw std::logic_error Invalid event type (type isn't @ref EventType::macro). */
    void setMaskedEvents(const std::list<std::string>& l);

    /*! Adds one masked event name into the macro event.
    @param e Event name for addition.
    @throw std::logic_error Invalid event type (type isn't @ref EventType::macro). */
    void addMaskedEvent(const std::string& e);

    /*! Returns copy of mapping rule list from the macro event.
    @return List of mapping rules.
    @throw std::logic_error Invalid event type (type isn't @ref EventType::macro). */
    [[nodiscard]] std::list<BehaviorAttributeRestriction> getMappingRules() const;

    /*! Sets new mapping rule list into the macro event.
    @details This method replaces current mapping rule list in the macro event with specified list.
    @param l List of mapping rules for setting.
    @throw std::logic_error Invalid event type (type isn't @ref EventType::macro). */
    void setMappingRules(const std::list<BehaviorAttributeRestriction>& l);

    /*! Adds one mapping rule into the macro event.
    @param r Mapping rule for addition.
    @throw std::logic_error Invalid event type (type isn't @ref EventType::macro). */
    void addMappingRule(const BehaviorAttributeRestriction& r);

    /*! Returns true for equal @ref Event object.
    @details This method returns true if all fields of the compared objects are equal.
    @param e @ref Event object for comparison.
    @return Result of the comparison. */
    bool operator==(const Event& e) const noexcept;

  protected:

    EventType _type;                                        ///< Type of event.
    ViewProperties _properties;                             ///< View properties of event.
    std::list<BehaviorAttribute> _attributes;               ///< Behavior attribute list of event.
    std::list<std::string> _masked_events;                  ///< Masked event name list (for macro event).
    std::list<BehaviorAttributeRestriction> _mapping_rules; ///< Mapping rule list (for macro event).

  }; // Event class

  /*! Returns true for event with type EventType::controllable.
  @details This function checks type of specified event and returns false if it isn't controllable.
  @param e Event for the type check.
  @return Result of the event type check. */
  bool isControllable(const Event& e);

  /*! Returns true for event with types EventType::uncontrollable, EventType::expected or EventType::macro.
  @details This function checks type of specified event and returns false if it's controllable.
  @param e Event for the type check.
  @return Result of the event type check. */
  bool isUncontrollable(const Event& e);

  /*! Returns true for event with types EventType::macro.
  @details This function checks type of specified event and returns false if it isn't macro event.
  @param e Event for the type check.
  @return Result of the event type check. */
  bool isMacro(const Event& e);

} // namespace


#endif // EVENT_HPP
