#pragma once

namespace herald {

/// Used to keep dynamically allocated
/// objects or memory until they go out
/// of scope.
/// @tparam Object The type to be contained
/// by the pointer.
template <typename Object>
class ScopedPtr final {
  /// A pointer to the object being managed.
  /// This will get deleted by the deconstructor.
  Object* object;
public:
  /// Constructs the scoped pointer.
  /// @param o The object to contain.
  constexpr ScopedPtr(Object* o = nullptr) noexcept : object(o) {}
  /// No copy constructor allowed
  /// because the object can't always be copied.
  ScopedPtr(const ScopedPtr<Object>&) = delete;
  /// Constructs a scoped pointer with move semantics.
  ScopedPtr(ScopedPtr<Object>&& other) noexcept
    : object(other.release()) {}
  /// Constructs the scoped pointer from one with
  /// another type, using move semantics.
  /// @tparam Other The type of the other object.
  template <typename Other>
  ScopedPtr(ScopedPtr<Other>& other) noexcept
    : object(other.release()) {}
  /// Releases the object, if there is one.
  ~ScopedPtr() {
    if (object) {
      delete object;
    }
  }
  /// Accesses a normal object pointer.
  Object* get() noexcept {
    return object;
  }
  /// Accesses a normal object pointer, using const-access.
  const Object* get() const noexcept {
    return object;
  }
  /// Removes the object from the pointer.
  /// This does not free any memory, it's
  /// mainly used for move constructors.
  /// @returns A pointer to the object
  /// that was released from the pointer wrapper.
  Object* release() noexcept {
    auto tmp = object;
    object = nullptr;
    return tmp;
  }
  /// Accesses the object, with write permission.
  Object& operator * () noexcept {
    return *object;
  }
  /// Accesses the object, read-only.
  const Object& operator * () const noexcept {
    return *object;
  }
  /// Accesses the object, with write permission.
  Object* operator -> () noexcept {
    return object;
  }
  /// Accesses the object, read-only.
  const Object* operator -> () const noexcept {
    return object;
  }
  /// Assigns a different scoped pointer to this one, via move semantics.
  /// @tparam Other The type of the other object being assigned.
  /// @param other The pointer to assign to "this" one.
  /// @returns A reference to "this" pointer.
  template <typename Other>
  ScopedPtr<Object>& operator = (ScopedPtr<Other>&& other) {

    if (object) {
      delete object;
    }

    object = other.release();

    return *this;
  }
  /// Indicates whether or not the pointer
  /// has an object or not.
  /// @returns True if there's a valid object,
  /// false if there's not.
  operator bool () const noexcept {
    return object;
  }
};

} // namespace herald
