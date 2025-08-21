// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xgo_body:msg/BodyPose.idl
// generated code does not contain a copyright notice

#ifndef XGO_BODY__MSG__DETAIL__BODY_POSE__STRUCT_H_
#define XGO_BODY__MSG__DETAIL__BODY_POSE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/BodyPose in the package xgo_body.
typedef struct xgo_body__msg__BodyPose
{
  float pose[6];
} xgo_body__msg__BodyPose;

// Struct for a sequence of xgo_body__msg__BodyPose.
typedef struct xgo_body__msg__BodyPose__Sequence
{
  xgo_body__msg__BodyPose * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xgo_body__msg__BodyPose__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XGO_BODY__MSG__DETAIL__BODY_POSE__STRUCT_H_
