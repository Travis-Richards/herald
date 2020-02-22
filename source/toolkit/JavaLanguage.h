#pragma once

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class Language;

/// Makes an instance of @ref Language
/// using a Java derived class.
/// @returns A new language instance.
ScopedPtr<Language> make_java_language();

} // namespace tk

} // namespace herald
