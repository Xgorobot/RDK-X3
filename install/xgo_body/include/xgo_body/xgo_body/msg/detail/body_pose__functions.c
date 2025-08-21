// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from xgo_body:msg/BodyPose.idl
// generated code does not contain a copyright notice
#include "xgo_body/msg/detail/body_pose__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
xgo_body__msg__BodyPose__init(xgo_body__msg__BodyPose * msg)
{
  if (!msg) {
    return false;
  }
  // pose
  return true;
}

void
xgo_body__msg__BodyPose__fini(xgo_body__msg__BodyPose * msg)
{
  if (!msg) {
    return;
  }
  // pose
}

bool
xgo_body__msg__BodyPose__are_equal(const xgo_body__msg__BodyPose * lhs, const xgo_body__msg__BodyPose * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // pose
  for (size_t i = 0; i < 6; ++i) {
    if (lhs->pose[i] != rhs->pose[i]) {
      return false;
    }
  }
  return true;
}

bool
xgo_body__msg__BodyPose__copy(
  const xgo_body__msg__BodyPose * input,
  xgo_body__msg__BodyPose * output)
{
  if (!input || !output) {
    return false;
  }
  // pose
  for (size_t i = 0; i < 6; ++i) {
    output->pose[i] = input->pose[i];
  }
  return true;
}

xgo_body__msg__BodyPose *
xgo_body__msg__BodyPose__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xgo_body__msg__BodyPose * msg = (xgo_body__msg__BodyPose *)allocator.allocate(sizeof(xgo_body__msg__BodyPose), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(xgo_body__msg__BodyPose));
  bool success = xgo_body__msg__BodyPose__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
xgo_body__msg__BodyPose__destroy(xgo_body__msg__BodyPose * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    xgo_body__msg__BodyPose__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
xgo_body__msg__BodyPose__Sequence__init(xgo_body__msg__BodyPose__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xgo_body__msg__BodyPose * data = NULL;

  if (size) {
    data = (xgo_body__msg__BodyPose *)allocator.zero_allocate(size, sizeof(xgo_body__msg__BodyPose), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = xgo_body__msg__BodyPose__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        xgo_body__msg__BodyPose__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
xgo_body__msg__BodyPose__Sequence__fini(xgo_body__msg__BodyPose__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      xgo_body__msg__BodyPose__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

xgo_body__msg__BodyPose__Sequence *
xgo_body__msg__BodyPose__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xgo_body__msg__BodyPose__Sequence * array = (xgo_body__msg__BodyPose__Sequence *)allocator.allocate(sizeof(xgo_body__msg__BodyPose__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = xgo_body__msg__BodyPose__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
xgo_body__msg__BodyPose__Sequence__destroy(xgo_body__msg__BodyPose__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    xgo_body__msg__BodyPose__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
xgo_body__msg__BodyPose__Sequence__are_equal(const xgo_body__msg__BodyPose__Sequence * lhs, const xgo_body__msg__BodyPose__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!xgo_body__msg__BodyPose__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
xgo_body__msg__BodyPose__Sequence__copy(
  const xgo_body__msg__BodyPose__Sequence * input,
  xgo_body__msg__BodyPose__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(xgo_body__msg__BodyPose);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    xgo_body__msg__BodyPose * data =
      (xgo_body__msg__BodyPose *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!xgo_body__msg__BodyPose__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          xgo_body__msg__BodyPose__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!xgo_body__msg__BodyPose__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
