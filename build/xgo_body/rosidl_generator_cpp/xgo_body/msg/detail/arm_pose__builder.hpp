// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xgo_body:msg/ArmPose.idl
// generated code does not contain a copyright notice

#ifndef XGO_BODY__MSG__DETAIL__ARM_POSE__BUILDER_HPP_
#define XGO_BODY__MSG__DETAIL__ARM_POSE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xgo_body/msg/detail/arm_pose__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xgo_body
{

namespace msg
{

namespace builder
{

class Init_ArmPose_pose
{
public:
  Init_ArmPose_pose()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::xgo_body::msg::ArmPose pose(::xgo_body::msg::ArmPose::_pose_type arg)
  {
    msg_.pose = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xgo_body::msg::ArmPose msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::xgo_body::msg::ArmPose>()
{
  return xgo_body::msg::builder::Init_ArmPose_pose();
}

}  // namespace xgo_body

#endif  // XGO_BODY__MSG__DETAIL__ARM_POSE__BUILDER_HPP_
