// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xgo_body:msg/ArmPose.idl
// generated code does not contain a copyright notice

#ifndef XGO_BODY__MSG__DETAIL__ARM_POSE__STRUCT_H_
#define XGO_BODY__MSG__DETAIL__ARM_POSE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/ArmPose in the package xgo_body.
typedef struct xgo_body__msg__ArmPose
{
  float pose[3];
} xgo_body__msg__ArmPose;

// Struct for a sequence of xgo_body__msg__ArmPose.
typedef struct xgo_body__msg__ArmPose__Sequence
{
  xgo_body__msg__ArmPose * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xgo_body__msg__ArmPose__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XGO_BODY__MSG__DETAIL__ARM_POSE__STRUCT_H_
