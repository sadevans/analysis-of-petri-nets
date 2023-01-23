/*! @file state_tests.cpp
@ref des::State class tests source file.
@authors A. Kozov
@date 2021/12/19 */

#include <boost/test/unit_test.hpp>

#include "state.hpp"


// Test of State default constructor.
BOOST_AUTO_TEST_CASE(CreateState) {
  des::State s;
  BOOST_CHECK(s.activity() == 0);
  des::ViewProperties p; // default view properties values
  BOOST_CHECK(s.viewProperties() == p);
}

// Test of State constructor by activity tokens quantity.
BOOST_AUTO_TEST_CASE(CreateStateByActivityTokensQuantity) {
  const unsigned activity_quantity = 10;
  des::State s(activity_quantity);
  BOOST_CHECK(s.activity() == activity_quantity);
  des::ViewProperties p; // default view properties values
  BOOST_CHECK(s.viewProperties() == p);
}

// Test of State copy and move constructors.
BOOST_AUTO_TEST_CASE(CreateStateByOtherState) {
  const unsigned activity_quantity = 10;
  const des::ViewProperties properties = { L"Test hint", 15.0, 5.0, 0.5 };
  des::State other(activity_quantity);
  other.viewProperties() = properties;
  des::State copy_result(other); // create by copying other state
  BOOST_CHECK(copy_result.activity() == activity_quantity);
  BOOST_CHECK(copy_result.viewProperties() == properties);
  des::State move_result(std::move(other)); // create by moving other state
  BOOST_CHECK(move_result.activity() == activity_quantity);
  BOOST_CHECK(move_result.viewProperties() == properties);
}

// Test of State copy and move assignments.
BOOST_AUTO_TEST_CASE(AssignStateByOtherState) {
  const unsigned activity_quantity = 10;
  const des::ViewProperties properties = { L"Test hint", 15.0, 5.0, 0.5 };
  des::State other(activity_quantity);
  other.viewProperties() = properties;
  des::State result;
  BOOST_CHECK_NO_THROW(result = other); // assign by copying other state
  BOOST_CHECK(result.activity() == activity_quantity);
  BOOST_CHECK(result.viewProperties() == properties);
  BOOST_CHECK_NO_THROW(result = std::move(other)); // assign by moving other state
  BOOST_CHECK(result.activity() == activity_quantity);
  BOOST_CHECK(result.viewProperties() == properties);
}

// Test of State setActivity method.
BOOST_AUTO_TEST_CASE(StateSetActivity) {
  unsigned result = 10;
  des::State s;
  BOOST_CHECK_NO_THROW(s.setActivity(result));
  BOOST_CHECK(s.activity() == result);
  for (result = 0; result < 5; result++) {
    BOOST_CHECK_NO_THROW(s.setActivity(result));
    BOOST_CHECK(s.activity() == result);
  }
}

// Test of State equivalence.
BOOST_AUTO_TEST_CASE(CompareStates) {
  des::State state(10);
  state.viewProperties() = { L"Test hint", 5.0, 0.5, 1.0 };
  BOOST_CHECK(!(state == des::State(1)));
  des::State other_state(state.activity());
  BOOST_CHECK(!(state == other_state));
  other_state.viewProperties() = state.viewProperties();
  BOOST_CHECK(state == other_state);
}
