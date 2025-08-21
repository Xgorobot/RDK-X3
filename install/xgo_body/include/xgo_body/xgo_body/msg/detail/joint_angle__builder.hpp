// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xgo_body:msg/JointAngle.idl
// generated code does not contain a copyright notice

#ifndef XGO_BODY__MSG__DETAIL__JOINT_ANGLE__BUILDER_HPP_
#define XGO_BODY__MSG__DETAIL__JOINT_ANGLE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xgo_body/msg/detail/joint_angle__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xgo_body
{

namespace msg
{

namespace builder
{

class Init_JointAngle_angle
{
public:
  Init_JointAngle_angle()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::xgo_body::msg::JointAngle angle(::xgo_body::msg::JointAngle::_angle_type arg)
  {
    msg_.angle = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xgo_body::msg::JointAngle msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::xgo_body::msg::JointAngle>()
{
  return xgo_body::msg::builder::Init_JointAngle_angle();
}

}  // namespace xgo_body

#endif  // XGO_BODY__MSG__DETAIL__JOINT_ANGLE__BUILDER_HPP_
