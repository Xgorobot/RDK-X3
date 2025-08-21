// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xgo_body:msg/JointAngle.idl
// generated code does not contain a copyright notice

#ifndef XGO_BODY__MSG__DETAIL__JOINT_ANGLE__STRUCT_H_
#define XGO_BODY__MSG__DETAIL__JOINT_ANGLE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/JointAngle in the package xgo_body.
typedef struct xgo_body__msg__JointAngle
{
  float angle[15];
} xgo_body__msg__JointAngle;

// Struct for a sequence of xgo_body__msg__JointAngle.
typedef struct xgo_body__msg__JointAngle__Sequence
{
  xgo_body__msg__JointAngle * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xgo_body__msg__JointAngle__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XGO_BODY__MSG__DETAIL__JOINT_ANGLE__STRUCT_H_
