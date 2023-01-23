/*! @file graph_translators.hpp
Translator functions header file.
@authors A. Kozov
@date 2021/08/29 */

#ifndef GRAPH_TRANSLATORS_HPP
#define GRAPH_TRANSLATORS_HPP

#include <string>
#include <sstream>

#include "automation.hpp"

// Namespace of DES model.
namespace des {


  /*! Translates the specified automation to DOT description.
  @param o Reference to std::ostream object.
  @param a Automation object.
  @return Reference to std::ostream object. */
  std::ostream& operator<<(std::ostream& o, const Automation& a);


  class Translator {
  public:

    Translator(const Automation& a) : _a(a) {};

    const Automation& a() const { return _a; };

  private:
    const Automation& _a;

  };

  inline std::ostream& operator<<(std::ostream& o, const Translator& t) {
    return o << t.a();
  }


  /*! Translates the specified automation to DOT description.
  @param a Automation object.
  @return Translator object to use stream output.*/
  inline Translator translateAutomationToDOT(const Automation& a) {
    return Translator(a);
  };




} // namespace


#endif // GRAPH_TRANSLATORS_HPP
