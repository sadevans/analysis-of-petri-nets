/*! @file components_tests.cpp
DES components tests source file.
@authors A. Kozov
@date 2021/12/18 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ModelTests

#include <boost/test/unit_test.hpp>

#include "components.hpp"


// Test of ViewProperties constructors.
BOOST_AUTO_TEST_CASE(CreateViewProperties) {
  des::ViewProperties p;
  BOOST_CHECK(p.Hint == L"");
  BOOST_CHECK(p.X == 0.0);
  BOOST_CHECK(p.Y == 0.0);
  BOOST_CHECK(p.Angle == 0.0);
  BOOST_CHECK_NO_THROW(des::ViewProperties other(p));
  BOOST_CHECK_NO_THROW(des::ViewProperties other(std::move(des::ViewProperties{})));
  BOOST_CHECK_NO_THROW(des::ViewProperties other = p);
  BOOST_CHECK_NO_THROW(des::ViewProperties other = std::move(des::ViewProperties{}));
}

// Test of ViewProperties equivalence.
BOOST_AUTO_TEST_CASE(CompareViewProperties) {
  const des::ViewProperties properties = { L"Test hint", 5.0, 0.5, 1.0 };
  des::ViewProperties other_properties;
  BOOST_CHECK(!(properties == other_properties));
  other_properties.Hint = properties.Hint;
  BOOST_CHECK(!(properties == other_properties));
  other_properties.X = properties.X;
  BOOST_CHECK(!(properties == other_properties));
  other_properties.Y = properties.Y;
  BOOST_CHECK(!(properties == other_properties));
  other_properties.Angle = properties.Angle;
  BOOST_CHECK(properties == other_properties);
}

// Test of BehaviorAttributeRestriction constructors.
BOOST_AUTO_TEST_CASE(CreateBehaviorAttributeRestriction) {
  des::BehaviorAttributeRestriction r;
  BOOST_CHECK(r.Name == L"");
  BOOST_CHECK(r.Function == L"");
  BOOST_CHECK_NO_THROW(des::BehaviorAttributeRestriction other(r));
  BOOST_CHECK_NO_THROW(des::BehaviorAttributeRestriction other(std::move(des::BehaviorAttributeRestriction{})));
  BOOST_CHECK_NO_THROW(des::BehaviorAttributeRestriction other = r);
  BOOST_CHECK_NO_THROW(des::BehaviorAttributeRestriction other = std::move(des::BehaviorAttributeRestriction{}));
}

// Test of BehaviorAttributeRestriction equivalence.
BOOST_AUTO_TEST_CASE(CompareBehaviorAttributeRestrictions) {
  const des::BehaviorAttributeRestriction restriction = { L"Test name", L"Test function" };
  des::BehaviorAttributeRestriction other_restriction;
  BOOST_CHECK(!(restriction == other_restriction));
  other_restriction.Name = restriction.Name;
  BOOST_CHECK(!(restriction == other_restriction));
  other_restriction.Function = restriction.Function;
  BOOST_CHECK(restriction == other_restriction);
}

// Test of BehaviorAttribute constructors.
BOOST_AUTO_TEST_CASE(CreateBehaviorAttribute) {
  des::BehaviorAttribute a;
  BOOST_CHECK(a.Type == L"");
  BOOST_CHECK(a.Restrictions.empty());
  BOOST_CHECK_NO_THROW(des::BehaviorAttribute other(a));
  BOOST_CHECK_NO_THROW(des::BehaviorAttribute other(std::move(des::BehaviorAttribute{})));
  BOOST_CHECK_NO_THROW(des::BehaviorAttribute other = a);
  BOOST_CHECK_NO_THROW(des::BehaviorAttribute other = std::move(des::BehaviorAttribute{}));
}

// Test of BehaviorAttribute equivalence.
BOOST_AUTO_TEST_CASE(CompareBehaviorAttributes) {
  const des::BehaviorAttribute attribute = { L"Test type", { { L"Test name", L"Test function" } } };
  des::BehaviorAttribute other_attribute;
  BOOST_CHECK(!(attribute == other_attribute));
  other_attribute.Type = attribute.Type;
  BOOST_CHECK(!(attribute == other_attribute));
  other_attribute.Restrictions = attribute.Restrictions;
  BOOST_CHECK(attribute == other_attribute);
}

// Test of checkNameString function.
BOOST_AUTO_TEST_CASE(CheckNameString) {
  const std::string empty("");
  BOOST_CHECK(!des::checkNameString(empty));
  const std::string very_long(des::max_name_length, 'T');
  BOOST_CHECK(!des::checkNameString(very_long));
  const std::string first_non_letter("/test");
  BOOST_CHECK(!des::checkNameString(first_non_letter));
  const std::string invalid_symbols("Test\nTest*{}");
  BOOST_CHECK(!des::checkNameString(invalid_symbols));
  const std::string right("Test/Name_123");
  BOOST_CHECK(des::checkNameString(right));
}
