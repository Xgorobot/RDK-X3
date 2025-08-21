// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xgo_body:msg/JointAngle.idl
// generated code does not contain a copyright notice

#ifndef XGO_BODY__MSG__DETAIL__JOINT_ANGLE__STRUCT_HPP_
#define XGO_BODY__MSG__DETAIL__JOINT_ANGLE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__xgo_body__msg__JointAngle __attribute__((deprecated))
#else
# define DEPRECATED__xgo_body__msg__JointAngle __declspec(deprecated)
#endif

namespace xgo_body
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct JointAngle_
{
  using Type = JointAngle_<ContainerAllocator>;

  explicit JointAngle_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<float, 15>::iterator, float>(this->angle.begin(), this->angle.end(), 0.0f);
    }
  }

  explicit JointAngle_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : angle(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<float, 15>::iterator, float>(this->angle.begin(), this->angle.end(), 0.0f);
    }
  }

  // field types and members
  using _angle_type =
    std::array<float, 15>;
  _angle_type angle;

  // setters for named parameter idiom
  Type & set__angle(
    const std::array<float, 15> & _arg)
  {
    this->angle = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xgo_body::msg::JointAngle_<ContainerAllocator> *;
  using ConstRawPtr =
    const xgo_body::msg::JointAngle_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xgo_body::msg::JointAngle_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xgo_body::msg::JointAngle_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xgo_body::msg::JointAngle_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xgo_body::msg::JointAngle_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xgo_body::msg::JointAngle_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xgo_body::msg::JointAngle_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xgo_body::msg::JointAngle_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xgo_body::msg::JointAngle_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xgo_body__msg__JointAngle
    std::shared_ptr<xgo_body::msg::JointAngle_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xgo_body__msg__JointAngle
    std::shared_ptr<xgo_body::msg::JointAngle_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const JointAngle_ & other) const
  {
    if (this->angle != other.angle) {
      return false;
    }
    return true;
  }
  bool operator!=(const JointAngle_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct JointAngle_

// alias to use template instance with default allocator
using JointAngle =
  xgo_body::msg::JointAngle_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace xgo_body

#endif  // XGO_BODY__MSG__DETAIL__JOINT_ANGLE__STRUCT_HPP_
