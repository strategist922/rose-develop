/** 
 * \file MDCG/include/MDCG/model-type.hpp
 *
 * \author Tristan Vanderbruggen
 *
 */

#ifndef __MDCG_MODEL_TYPE_HPP__
#define __MDCG_MODEL_TYPE_HPP__

#include "MDCG/model-base.hpp"

class SgType;

namespace MDCG {

namespace Model {

/*!
 * \addtogroup grp_mdcg_model
 * @{
*/

template <>
struct node_t<e_model_type> {
  /// invalid when kind == e_native_type && kind == e_class_type (should be NULL)
  type_symbol_t * symbol;

  enum {
    e_typedef_type,
    e_enum_type,
    e_native_type,
    e_class_type /// in this case symbol is NULL.
  } kind;

  /// valid iff kind == e_typedef_type
  type_t * referenced_type;

  /// valid iff kind == e_native_type
  SgType * native_type;

  /// valid iff kind == e_class_type
  class_t * aliased_class;
};

template <>
struct scope_t<e_model_type> {
  union {
    namespace_t * a_namespace;
    class_t * a_class;
  } parent;
};

/** @} */

}

}

#endif /* __MDCG_MODEL_TYPE_HPP__ */

