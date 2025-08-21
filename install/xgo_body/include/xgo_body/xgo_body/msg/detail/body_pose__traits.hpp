// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xgo_body:msg/BodyPose.idl
// generated code does not contain a copyright notice

#ifndef XGO_BODY__MSG__DETAIL__BODY_POSE__TRAITS_HPP_
#define XGO_BODY__MSG__DETAIL__BODY_POSE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xgo_body/msg/detail/body_pose__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xgo_body
{

namespace msg
{

inline void to_flow_style_yaml(
  const BodyPose & msg,
  std::ostream & out)
{
  out << "{";
  // member: pose
  {
    if (msg.pose.size() == 0) {
      out << "pose: []";
    } else {
      out << "pose: [";
      size_t pending_items = msg.pose.size();
      for (auto item : msg.pose) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const BodyPose & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.pose.size() == 0) {
      out << "pose: []\n";
    } else {
      out << "pose:\n";
      for (auto item : msg.pose) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const BodyPose & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace xgo_body

namespace rosidl_generator_traits
{

[[deprecated("use xgo_body::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const xgo_body::msg::BodyPose & msg,
  std::ostream & out, size_t indentation = 0)
{
  xgo_body::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xgo_body::msg::to_yaml() instead")]]
inline std::string to_yaml(const xgo_body::msg::BodyPose & msg)
{
  return xgo_body::msg::to_yaml(msg);
}

template<>
inline const char * data_type<xgo_body::msg::BodyPose>()
{
  return "xgo_body::msg::BodyPose";
}

template<>
inline const char * name<xgo_body::msg::BodyPose>()
{
  return "xgo_body/msg/BodyPose";
}

template<>
struct has_fixed_size<xgo_body::msg::BodyPose>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<xgo_body::msg::BodyPose>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<xgo_body::msg::BodyPose>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // XGO_BODY__MSG__DETAIL__BODY_POSE__TRAITS_HPP_
