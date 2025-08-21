// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from xgo_body:msg/LegPose.idl
// generated code does not contain a copyright notice
#include "xgo_body/msg/detail/leg_pose__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
xgo_body__msg__LegPose__init(xgo_body__msg__LegPose * msg)
{
  if (!msg) {
    return false;
  }
  // pose
  return true;
}

void
xgo_body__msg__LegPose__fini(xgo_body__msg__LegPose * msg)
{
  if (!msg) {
    return;
  }
  // pose
}

bool
xgo_body__msg__LegPose__are_equal(const xgo_body__msg__LegPose * lhs, const xgo_body__msg__LegPose * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // pose
  for (size_t i = 0; i < 12; ++i) {
    if (lhs->pose[i] != rhs->pose[i]) {
      return false;
    }
  }
  return true;
}

bool
xgo_body__msg__LegPose__copy(
  const xgo_body__msg__LegPose * input,
  xgo_body__msg__LegPose * output)
{
  if (!input || !output) {
    return false;
  }
  // pose
  for (size_t i = 0; i < 12; ++i) {
    output->pose[i] = input->pose[i];
  }
  return true;
}

xgo_body__msg__LegPose *
xgo_body__msg__LegPose__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xgo_body__msg__LegPose * msg = (xgo_body__msg__LegPose *)allocator.allocate(sizeof(xgo_body__msg__LegPose), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(xgo_body__msg__LegPose));
  bool success = xgo_body__msg__LegPose__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
xgo_body__msg__LegPose__destroy(xgo_body__msg__LegPose * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    xgo_body__msg__LegPose__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
xgo_body__msg__LegPose__Sequence__init(xgo_body__msg__LegPose__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xgo_body__msg__LegPose * data = NULL;

  if (size) {
    data = (xgo_body__msg__LegPose *)allocator.zero_allocate(size, sizeof(xgo_body__msg__LegPose), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = xgo_body__msg__LegPose__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        xgo_body__msg__LegPose__fini(&data[i - 1]);
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
xgo_body__msg__LegPose__Sequence__fini(xgo_body__msg__LegPose__Sequence * array)
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
      xgo_body__msg__LegPose__fini(&array->data[i]);
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

xgo_body__msg__LegPose__Sequence *
xgo_body__msg__LegPose__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  xgo_body__msg__LegPose__Sequence * array = (xgo_body__msg__LegPose__Sequence *)allocator.allocate(sizeof(xgo_body__msg__LegPose__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = xgo_body__msg__LegPose__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
xgo_body__msg__LegPose__Sequence__destroy(xgo_body__msg__LegPose__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    xgo_body__msg__LegPose__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
xgo_body__msg__LegPose__Sequence__are_equal(const xgo_body__msg__LegPose__Sequence * lhs, const xgo_body__msg__LegPose__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!xgo_body__msg__LegPose__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
xgo_body__msg__LegPose__Sequence__copy(
  const xgo_body__msg__LegPose__Sequence * input,
  xgo_body__msg__LegPose__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(xgo_body__msg__LegPose);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    xgo_body__msg__LegPose * data =
      (xgo_body__msg__LegPose *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!xgo_body__msg__LegPose__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          xgo_body__msg__LegPose__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!xgo_body__msg__LegPose__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
