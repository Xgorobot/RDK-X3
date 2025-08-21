// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from xgo_body:msg/BodyPose.idl
// generated code does not contain a copyright notice

#ifndef XGO_BODY__MSG__DETAIL__BODY_POSE__FUNCTIONS_H_
#define XGO_BODY__MSG__DETAIL__BODY_POSE__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "xgo_body/msg/rosidl_generator_c__visibility_control.h"

#include "xgo_body/msg/detail/body_pose__struct.h"

/// Initialize msg/BodyPose message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * xgo_body__msg__BodyPose
 * )) before or use
 * xgo_body__msg__BodyPose__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
bool
xgo_body__msg__BodyPose__init(xgo_body__msg__BodyPose * msg);

/// Finalize msg/BodyPose message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
void
xgo_body__msg__BodyPose__fini(xgo_body__msg__BodyPose * msg);

/// Create msg/BodyPose message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * xgo_body__msg__BodyPose__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
xgo_body__msg__BodyPose *
xgo_body__msg__BodyPose__create();

/// Destroy msg/BodyPose message.
/**
 * It calls
 * xgo_body__msg__BodyPose__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
void
xgo_body__msg__BodyPose__destroy(xgo_body__msg__BodyPose * msg);

/// Check for msg/BodyPose message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
bool
xgo_body__msg__BodyPose__are_equal(const xgo_body__msg__BodyPose * lhs, const xgo_body__msg__BodyPose * rhs);

/// Copy a msg/BodyPose message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
bool
xgo_body__msg__BodyPose__copy(
  const xgo_body__msg__BodyPose * input,
  xgo_body__msg__BodyPose * output);

/// Initialize array of msg/BodyPose messages.
/**
 * It allocates the memory for the number of elements and calls
 * xgo_body__msg__BodyPose__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
bool
xgo_body__msg__BodyPose__Sequence__init(xgo_body__msg__BodyPose__Sequence * array, size_t size);

/// Finalize array of msg/BodyPose messages.
/**
 * It calls
 * xgo_body__msg__BodyPose__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
void
xgo_body__msg__BodyPose__Sequence__fini(xgo_body__msg__BodyPose__Sequence * array);

/// Create array of msg/BodyPose messages.
/**
 * It allocates the memory for the array and calls
 * xgo_body__msg__BodyPose__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
xgo_body__msg__BodyPose__Sequence *
xgo_body__msg__BodyPose__Sequence__create(size_t size);

/// Destroy array of msg/BodyPose messages.
/**
 * It calls
 * xgo_body__msg__BodyPose__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
void
xgo_body__msg__BodyPose__Sequence__destroy(xgo_body__msg__BodyPose__Sequence * array);

/// Check for msg/BodyPose message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
bool
xgo_body__msg__BodyPose__Sequence__are_equal(const xgo_body__msg__BodyPose__Sequence * lhs, const xgo_body__msg__BodyPose__Sequence * rhs);

/// Copy an array of msg/BodyPose messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_xgo_body
bool
xgo_body__msg__BodyPose__Sequence__copy(
  const xgo_body__msg__BodyPose__Sequence * input,
  xgo_body__msg__BodyPose__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // XGO_BODY__MSG__DETAIL__BODY_POSE__FUNCTIONS_H_
