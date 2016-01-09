#ifndef RBX_BUILTIN_EXCEPTION_HPP
#define RBX_BUILTIN_EXCEPTION_HPP

#include "builtin/object.hpp"

namespace rubinius {
  class Class;
  class Array;
  class CompiledCode;

  class Exception : public Object {
  public:
    const static object_type type = ExceptionType;

  private:
    Object* reason_message_;  // slot
    Array* locations_; // slot
    Exception* parent_; // slot
    Object* backtrace_; // slot
    Object* custom_backtrace_; // slot

  public:
    /* accessors */

    attr_accessor(reason_message, Object);
    attr_accessor(locations, Array);
    attr_accessor(parent, Exception);
    attr_accessor(backtrace, Object);
    attr_accessor(custom_backtrace, Object);

    /* interface */

    static void init(STATE);
    static Exception* create(STATE);

    void print_locations(STATE);

    const char* message_c_str(STATE);

    static Exception* make_exception(STATE, Class* exc_class, const char* message);
    static Exception* make_type_error(STATE, object_type type, Object* object,
                                      const char* reason = NULL);
    static Exception* make_errno_exception(STATE, Class* exc_class, Object* reason, Object* loc);

    static Exception* make_argument_error(STATE, int expected, int given, Symbol* name=0);
    static Exception* make_encoding_compatibility_error(STATE, Object* a, Object* b);
    static Exception* make_frozen_exception(STATE, Object* obj);

    NORETURN(static void argument_error(STATE, int expected, int given));
    NORETURN(static void argument_error(STATE, const char* reason));
    NORETURN(static void regexp_error(STATE, const char* reason));
    NORETURN(static void type_error(STATE, const char* reason));
    NORETURN(static void type_error(STATE, object_type type, Object* object,
                           const char* reason = NULL));
    static void type_error(STATE, const char* reason, CallFrame* call_frame);
    NORETURN(static void float_domain_error(STATE, const char* reason = NULL));
    NORETURN(static void zero_division_error(STATE, const char* reason = NULL));
    NORETURN(static void io_error(STATE, const char* reason));
    NORETURN(static void range_error(STATE, const char* reason));
    NORETURN(static void runtime_error(STATE, const char* reason));

    NORETURN(static void assertion_error(STATE, const char* reason = NULL));
    NORETURN(static void object_bounds_exceeded_error(STATE, Object* obj, int index));
    NORETURN(static void object_bounds_exceeded_error(STATE, const char* reason));

    /** Raise a SystemCallError with given message. */
    NORETURN(static void system_call_error(STATE, const char* reason));
    NORETURN(static void system_call_error(STATE, const std::string& reason));

    NORETURN(static void thread_error(STATE, const char* reason));
    NORETURN(static void fiber_error(STATE, const char* reason));
    NORETURN(static void memory_error(STATE));

    static Exception* make_lje(STATE, CallFrame* frame);

    static void internal_error(STATE, CallFrame* frame, const char* reason);
    static void bytecode_error(STATE, CallFrame* frame, CompiledCode* code,
                               int ip, const char* reason);
    static void frozen_error(STATE, Object* obj);
    static void frozen_error(STATE, CallFrame* frame, Object* obj);

    static void encoding_compatibility_error(STATE, Object* a, Object* b);
    static void encoding_compatibility_error(STATE, Object* a, Object* b,
                                             CallFrame* frame);

    // Rubinius.primitive :exception_errno_error
    static Object* errno_error(STATE, Object* reason, Fixnum* ern, Object* loc);

    static void errno_error(STATE, const char* reason = NULL, int ern = 0,
                            const char* entity = 0);

    static void errno_wait_readable(STATE, int error);
    static void errno_wait_writable(STATE, int error);

    /**
     * Convenience predicates for checking the class of an
     * exception instance. These are provided as a way around
     * creating a bunch more C++ builtin classes and globals
     * and because the exception hierarchy (e.g. FloatDomainError)
     * isn't always consistent.
     */
    static bool argument_error_p(STATE, Exception* exc);
    static bool type_error_p(STATE, Exception* exc);
    static bool zero_division_error_p(STATE, Exception* exc);
    static bool float_domain_error_p(STATE, Exception* exc);
    static bool assertion_error_p(STATE, Exception* exc);
    static bool object_bounds_exceeded_error_p(STATE, Exception* exc);
    static bool errno_error_p(STATE, Exception* exc);
    static bool system_call_error_p(STATE, Exception* exc);
    static bool io_error_p(STATE, Exception* exc);
    static bool runtime_error_p(STATE, Exception* exc);

    static Class* get_argument_error(STATE);
    static Class* get_type_error(STATE);
    static Class* get_zero_division_error(STATE);
    static Class* get_float_domain_error(STATE);
    static Class* get_range_error(STATE);
    static Class* get_assertion_error(STATE);
    static Class* get_object_bounds_exceeded_error(STATE);
    static Class* get_io_error(STATE);
    static Class* get_system_call_error(STATE);
    static Class* get_thread_error(STATE);
    static Class* get_fiber_error(STATE);
    static Class* get_errno_error(STATE, Fixnum* ern);
    static Class* get_runtime_error(STATE);
    static Class* get_encoding_compatibility_error(STATE);

    class Info : public TypeInfo {
    public:
      BASIC_TYPEINFO(TypeInfo)
      virtual void show(STATE, Object* self, int level);
    };

    friend class Info;
  };
};

#endif
