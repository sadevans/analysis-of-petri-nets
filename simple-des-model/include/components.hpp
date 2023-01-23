/*! @file components.hpp
DES model components classes header file.
@authors A. Kozov
@date 2021/12/15 */

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <exception>
#include <list>
#include <regex>
#include <string>


// Namespace of DES model.
namespace des {

  /*! Max length of name string for states, events and models. */
  const size_t max_name_length = 256;


  /*! Structure of base view properties for DES model components.
  @details This structure contains Unicode description, Cartesian coordinates and rotation angle for visualization of a
  DES component. The coordinates are zero in the top left screen corner, X-axis is from left to right, Y-axis is from
  top to down and the positive rotation angle in degrees is counted counterclockwise from the Y-axis. Default value:
  empty description, X, Y and Angle are zero. */
  struct ViewProperties {
    std::wstring Hint = L""; ///< Unicode description.
    double X = 0.0;          ///< X coordinate.
    double Y = 0.0;          ///< Y coordinate.
    double Angle = 0.0;      ///< Rotation angle.
  };

  /*! Returns true for two equal @ref ViewProperties objects.
  @details This function compares two objects and returns true if all fields of the compared objects are equal.
  @param l Left @ref ViewProperties object.
  @param r Right @ref ViewProperties object.
  @return Result of the comparison. */
  bool operator==(const ViewProperties& l, const ViewProperties& r) noexcept;


  /*! Structure of a restriction to behavior attribute.
  @details This structure contains Unicode strings with restriction name and restriction implementation code. */
  struct BehaviorAttributeRestriction {
    std::wstring Name = L"";     ///< Name of restriction.
    std::wstring Function = L""; ///< Implementation of restriction.
  };

  /*! Returns true for two equal @ref BehaviorAttributeRestriction objects.
  @details This function compares two objects and returns true if all fields of the compared objects are equal.
  @param l Left @ref BehaviorAttributeRestriction object.
  @param r Right @ref BehaviorAttributeRestriction object.
  @return Result of the comparison. */
  bool operator==(const BehaviorAttributeRestriction& l, const BehaviorAttributeRestriction& r) noexcept;


  /*! Structure of a behavior attribute.
  @details This structure contains Unicode string with type and list of restrictions for the behavior attribute. */
  struct BehaviorAttribute {
    std::wstring Type = L"";                                   ///< Type of behavior attribute.
    std::list<BehaviorAttributeRestriction> Restrictions = {}; ///< List of restrictions to behavior attribute.
  };

  /*! Returns true for two equal @ref BehaviorAttribute objects.
  @details This function compares two objects and returns true if all fields of the compared objects are equal.
  @param l Left @ref BehaviorAttribute object.
  @param r Right @ref BehaviorAttribute object.
  @return Result of the comparison. */
  bool operator==(const BehaviorAttribute& l, const BehaviorAttribute& r) noexcept;

  /*! Returns true for correct name string.
  @details This function checks correctness of specified name string and returns its result. Correct name string has
  length less then @ref max_name_length, starts with Latin letter and contains only Latin letters, digits, underscores
  or forward slashes.
  @param s String for check.
  @return Result of the string check. */
  bool checkNameString(const std::string& s);

} // namespace


#endif // COMPONENTS_HPP
