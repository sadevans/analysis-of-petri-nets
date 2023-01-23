/*! @file state.hpp
@ref des::State class header file.
@authors A. Kozov
@date 2021/12/15 */

#ifndef STATE_HPP
#define STATE_HPP

#include <exception>
#include <list>
#include <string>

#include "components.hpp"


// Namespace of DES model.
namespace des {

  /*! Class of a DES state corresponding a Petri net place.
  @details This class contains activity token quantity (quantity of tokens in corresponding Petri net place) and view
  properties. */
  class State {
  public:

    /*! Constructs a @ref State object by copying of other @ref State object. */
    State(const State&) = default;

    /*! Constructs a @ref State object by moving of other @ref State object. */
    State(State&&) = default;

    /*! Assigns new value of a @ref State object by copying of other @ref State object. */
    State& operator=(const State&) = default;

    /*! Assigns new value of a @ref State object by moving of other @ref State object. */
    State& operator=(State&&) = default;

    /*! Constructs a @ref State object with specified activity tokens quantity.
    @details Constructor creates @ref State object with specified quantity of activity tokens and default view
    properties.
    @param a Quantity of activity tokens. */
    explicit State(const unsigned& a = 0);

    /*! Sets new activity tokens quantity in the state.
    @details This function replaces current value of activity token quantity in the state with specified quantity.
    @param a Quantity of activity tokens. */
    void setActivity(const unsigned& a) noexcept;

    /*! Returns quantity of activity tokens in the state.
    @return Quantity of activity tokens. */
    [[nodiscard]] inline unsigned activity() const noexcept {
      return _activity;
    }

    /*! Returns view properties of the state.
    @return Reference to the state view properties. */
    inline ViewProperties& viewProperties() noexcept {
      return _properties;
    }

    /*! Returns true for equal @ref State object.
    @details This function returns true if all fields of the compared objects are equal.
    @param s @ref State object for comparison.
    @return Result of the comparison. */
    bool operator==(const State& s) const noexcept;

  protected:

    unsigned _activity;                       ///< Quantity of activity tokens in state.
    ViewProperties _properties;               ///< View properties of state.

  }; // State class

} // namespace


#endif // STATE_HPP
