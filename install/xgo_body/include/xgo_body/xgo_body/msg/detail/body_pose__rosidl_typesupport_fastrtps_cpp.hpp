// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from xgo_body:msg/BodyPose.idl
// generated code does not contain a copyright notice

#ifndef XGO_BODY__MSG__DETAIL__BODY_POSE__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define XGO_BODY__MSG__DETAIL__BODY_POSE__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "xgo_body/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "xgo_body/msg/detail/body_pose__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace xgo_body
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_xgo_body
cdr_serialize(
  const xgo_body::msg::BodyPose & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_xgo_body
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  xgo_body::msg::BodyPose & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_xgo_body
get_serialized_size(
  const xgo_body::msg::BodyPose & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_xgo_body
max_serialized_size_BodyPose(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace xgo_body

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_xgo_body
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, xgo_body, msg, BodyPose)();

#ifdef __cplusplus
}
#endif

#endif  // XGO_BODY__MSG__DETAIL__BODY_POSE__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
