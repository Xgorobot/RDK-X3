// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from xgo_body:msg/LegPose.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "xgo_body/msg/detail/leg_pose__rosidl_typesupport_introspection_c.h"
#include "xgo_body/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "xgo_body/msg/detail/leg_pose__functions.h"
#include "xgo_body/msg/detail/leg_pose__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  xgo_body__msg__LegPose__init(message_memory);
}

void xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_fini_function(void * message_memory)
{
  xgo_body__msg__LegPose__fini(message_memory);
}

size_t xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__size_function__LegPose__pose(
  const void * untyped_member)
{
  (void)untyped_member;
  return 12;
}

const void * xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__get_const_function__LegPose__pose(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__get_function__LegPose__pose(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__fetch_function__LegPose__pose(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__get_const_function__LegPose__pose(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__assign_function__LegPose__pose(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__get_function__LegPose__pose(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_message_member_array[1] = {
  {
    "pose",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    12,  // array size
    false,  // is upper bound
    offsetof(xgo_body__msg__LegPose, pose),  // bytes offset in struct
    NULL,  // default value
    xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__size_function__LegPose__pose,  // size() function pointer
    xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__get_const_function__LegPose__pose,  // get_const(index) function pointer
    xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__get_function__LegPose__pose,  // get(index) function pointer
    xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__fetch_function__LegPose__pose,  // fetch(index, &value) function pointer
    xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__assign_function__LegPose__pose,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_message_members = {
  "xgo_body__msg",  // message namespace
  "LegPose",  // message name
  1,  // number of fields
  sizeof(xgo_body__msg__LegPose),
  xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_message_member_array,  // message members
  xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_init_function,  // function to initialize message memory (memory has to be allocated)
  xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_message_type_support_handle = {
  0,
  &xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_xgo_body
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, xgo_body, msg, LegPose)() {
  if (!xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_message_type_support_handle.typesupport_identifier) {
    xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &xgo_body__msg__LegPose__rosidl_typesupport_introspection_c__LegPose_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
