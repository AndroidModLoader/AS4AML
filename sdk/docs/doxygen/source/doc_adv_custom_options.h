/**

\page doc_adv_custom_options Custom options

Most applications have different needs and AngelScript is meant to be as flexible 
as possible to suit everybody. For this reason there are several options for 
customizing the script engine for the best fit.







\section doc_adv_custom_options_reg_types Registerable types

Even types that one might normally expect to be built-in to a script language, e.g.
string, array, and other container classes, are \ref doc_register_type "registered by the application". 
This is to allow the application to provide the exact implementation desired. 

Of course, should the application developer want to use a premade implementation the 
\ref doc_addon_script "add-ons provide just that".










\section doc_adv_custom_options_lang_mod Language modifications

The engine method \ref asIScriptEngine::SetEngineProperty "SetEngineProperty" permits making 
runtime choices about several different behaviours in the engine.

The following modify the script language in one way or the other:

\ref asEP_DISALLOW_EMPTY_LIST_ELEMENTS

By turning on this option the compiler will no longer accept empty list elements in initialization lists.
The following will for example not be supported:

<pre>
  array<int> arr = {1,2,,4,5,};
</pre>

When not turned on, the compiler will leave the empty list elements with the uninitialized value, just as an
uninitialized variable of the same type.

\ref asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE

By turning on this the compiler will no longer allow the use of value assignment operators on reference types.
While it may seem drastic, it can help reduce risk for bugs in the scripts, as the script writer will no longer
be able to do a value assignment by mistake when he meant to a handle assignment. Reference types should usually
not be copied, so the imposed restriction will likely not cause problems anyway.

\ref asEP_ALLOW_UNSAFE_REFERENCES

By turning on unsafe references you allow in-out references to be used for primitives and value types too.
Normally this will work fine, just as it does in ordinary languages such as C++, but know that it is 
quite possible to write scripts that will cause memory invasions or crashes if the references are not properly
guarded. With this option turned on you cannot consider the scripts to be sand-boxed any longer.

Turning this option on also makes temporary objects created within expressions to be destroyed only at the 
end of expressions, rather than as soon as possible. This makes it possible to pass references to temporary objects
to function calls within the expression, without the references becoming invalid too early.

\ref asEP_USE_CHARACTER_LITERALS, \ref asEP_ALLOW_MULTILINE_STRINGS, \ref asEP_SCRIPT_SCANNER, \ref asEP_STRING_ENCODING

These options are used to determine how strings are treated by the compiler. The details are described in \ref doc_strings.

\ref asEP_HEREDOC_TRIM_MODE

With this option the compiler can be set to always trim \ref doc_datatypes_strings "heredoc strings",
only trim if it is multiple lines, or never trim them.

\ref asEP_ALLOW_IMPLICIT_HANDLE_TYPES

This option is experimental. By turning it on script classes can be declared to always be treated as handles by declaring the 
class with @ before the name of the class. When this is done all variables of that type will be handles, and the assignment
operator will always perform a handle assignment.

\ref asEP_REQUIRE_ENUM_SCOPE

With this option all enum values must be prefixed with the enum type using the scope operator to qualify.

\ref asEP_PROPERTY_ACCESSOR_MODE

By default \ref doc_script_class_prop "virtual property accessors" when declared as individual functions need to be 
marked as such with the keyword 'property', or else the compiler won't consider the functions as virtual properties.
This behaviour was introduced in version 2.33.1.

Before this version, the compiler would automatically identify functions with the pre-fix 'get_' or 'set_' as virtual
property accessors if the function signature was appropriate. This led to undesired behaviour when developers would
declare such functions without the intention of them being used as virtual properties and yet the compiler used them as such.

For backwards compatibility it is still possible to configure the engine to use this behaviour by setting the engine
property asEP_PROPERTY_ACCESSOR_MODE to 2.

It is also possible to disable virtual property accessors all together by setting the engine property to 0.

Setting the option to 1 only allows virtual property accessors for registered functions, but still without requiring the keyword 'property'.

\ref asEP_DISALLOW_GLOBAL_VARS

The application can disable the ability to declare global variables in scripts completely. This can be useful in such cases
when the application wish to have full control of what can and cannot be stored by a script.  

\ref asEP_ALWAYS_IMPL_DEFAULT_CONSTRUCT

This property determines if the compiler will automatically provide a default constructor for classes. When set to 0 (default) 
it will be as per \ref doc_script_class_construct_auto "language specification". When set to 1 it will always be generated, and 
when set to 2 it will never be generated.

\ref asEP_ALWAYS_IMPL_DEFAULT_COPY_CONSTRUCT

This property determines if the compiler will automatically provide a copy constructor for classes. When set to 0 (default) 
it will be as per \ref doc_script_class_construct_auto "language specification". When set to 1 it will always be generated, and 
when set to 2 it will never be generated.

In versions before 2.37.0 the copy constructor was not generated automatically, so if backwards compatibility is needed, set this to 0.

\ref asEP_ALWAYS_IMPL_DEFAULT_COPY

This property determines if the compiler will automatically provide a copy operator for classes. When set to 0 (default) 
it will be as per \ref doc_script_class_assign_ops_auto "language specification". When set to 1 it will always be generated, and 
when set to 2 it will never be generated.

In versions before 2.37.0 the copy operator was always generated automatically, so if backwards compatibility is needed, set this to 1.

\ref asEP_ALTER_SYNTAX_NAMED_ARGS

If this flag is set to 1 or 2 the compiler will accept the '=' instead of ':' for \ref function "naming arguments in function calls". 
When option 1 is used the compiler will warn about it, so the script writer can update the script to the correct ':' token. When 
option 2 is used both '=' and ':' are accepted silently.

Observe that the use of the '=' token for naming arguments may cause a bit of a confusion. Especially if there is a variable
with the same name as the function argument as the script writer may think he's updating the value of the variable, but is 
just naming the function argument.

\ref asEP_DISABLE_INTEGER_DIVISION

This option changes the default behaviour of the / and /= operators. When true, this option replaces integer division
with floating-point division, e.g. 1/2 == 0.5 instead of 1/2 == 0

\ref asEP_PRIVATE_PROP_AS_PROTECTED

When this option is set to true the compiler will not prevent the access to private properties inherited from the parent class.
It will only write a warning.

This option is provided to give backwards compatibility for scripts written between versions 2.19.1, when support for private 
properties was added, albeit working more like protected properties, and version 2.30.0 when the behaviour was changed so that private
properties work like in other languages, i.e. they can't be accessed by derived classes.

\ref asEP_ALLOW_UNICODE_IDENTIFIERS

When this option is set to true the compiler will accept identifiers that contain characters with byte 
value higher than 127. This permits the use of international characters in the identifiers as the script 
can be encoded in UTF-8 format and compiler normally.

\ref asEP_IGNORE_DUPLICATE_SHARED_INTF

By turning on this option the compiler will silently accept duplicate declarations of shared interfaces in modules. 
This option was added to provide backwards compatibility with 2.35.0.

\ref asEP_BOOL_CONVERSION_MODE

By default boolean conversions is only done implicitly for value types with opImplConv conversion operator. If set to 1, then 
the compiler will also use the explicit opConv conversion operator even for reference types, in contexts where a boolean is 
expected, e.g. in conditions.

\ref asEP_FOREACH_SUPPORT

By turning off this property the support for \ref while "foreach loops" is disabled. This option was added to provide 
backwards compatibility for existing scripts before 2.38.0 that may be using the reserved keyword <tt>foreach</tt>.

\ref asEP_MEMBER_INIT_MODE

When this property to 0, the class members with an initialization expression in the declaration will always be initialized 
after the call to super(). It is also not possible to explicitly initialize members within the body of the constructor. When 
set to 1 (default), the class members will be initialized as described in \ref doc_script_class_memberinit. This mode was 
added to provide backwards compatibility with versions before 2.38.0.




\section doc_adv_custom_options_engine Engine behaviours

There are also several options in \ref asIScriptEngine::SetEngineProperty "SetEngineProperty" for modifying the way the engine behaves.

\ref asEP_MAX_NESTED_CALLS

This property defines how many recursive nested calls the engine should accept. In this context a nested call is when a script calls into 
an application registered function that in turn calls another script function. The second call is then said to be a nested call.

By default the engine will accept 100 nested calls.

\ref asEP_OPTIMIZE_BYTECODE

Normally this option is only used for testing the library, but should you find that the compilation time takes too long, then
it may be of interest to turn off the bytecode optimization pass by setting this option to false. 
 
\ref asEP_COPY_SCRIPT_SECTIONS
 
If you want to spare some dynamic memory and the script sections passed to the engine is already stored somewhere in memory then you
can turn off this options. If you do you'll need to be careful not to modify or deallocate the script sections until module has been built.
 
\ref asEP_MAX_STACK_SIZE, \ref asEP_INIT_STACK_SIZE, \ref asEP_MAX_CALL_STACK_SIZE, \ref asEP_INIT_CALL_STACK_SIZE

The script context's data stack grows dynamically as needed during executions. By default there is no limit to how large it may grow, but
if you wish to set this limit you can do with the asEP_MAX_STACK_SIZE option. The limit is given in bytes, but it is only an approximate limit. Setting the 
limit to 1MB doesn't mean the stack will grow exactly to 1MB, but you can be certain that it will not grow much beyond that.

Whenever a context attempts to grow the stack more than it is allowed, it will abort the execution and return a 'stack overflow' script 
exception.

Similarly the call stack also grows dynamically as needed during executions. The default here is also no limit, with the ability to limit
maximum size by setting the asEP_MAX_CALL_STACK_SIZE. This limit is set in number of function calls. 

In some cases it may be useful to set the initial stack size too, e.g. if you know beforehand that a large stack is needed, or 
if you wish to avoid any runtime memory allocations during an execution. In this case you can use the asEP_INIT_STACK_SIZE for the 
data stack, and asEP_INIT_CALL_STACK_SIZE for the call stack.
 
\ref asEP_BUILD_WITHOUT_LINE_CUES
 
This option can be used to speed up the execution of scripts a bit. It is best described in \ref doc_finetuning_3.
 
\ref asEP_INIT_GLOBAL_VARS_AFTER_BUILD
 
By default the global variables in the script modules are initialized immediately after building the scripts, but sometimes this is not 
desired. For example, if you debug the initialization of the variables, or if you're building an offline compiler, or even if you want
to add extra validation of the variables to make sure the script doesn't declare any variables you don't want it to.

\see \ref doc_adv_precompile
 
\ref asEP_INCLUDE_JIT_INSTRUCTIONS

In order for JIT compilation to work properly it needs to have some hints in the bytecode so it can know where a switch from 
bytecode execution to JIT compiled execution shall be done. By default this option is turned off as it would just add an overhead
to the bytecode, but when using JIT compilation this overhead will be compensated by the faster execution with direct machine code.

\see \ref doc_adv_jit

\ref asEP_EXPAND_DEF_ARRAY_TO_TMPL
 
This option is to determine how the \ref asIScriptEngine::RegisterDefaultArrayType "default array type" is displayed in compiler 
messages and function declarations, e.g. int[] or array<int>. 
 
\ref asEP_AUTO_GARBAGE_COLLECT

By default AngelScript's incremental garbage collector is executed as scripts are invoked by the application. This does add 
a slight overhead to the execution, which is normally tolerable but in some applications the CPU cycles may be needed for other
things. In this case the automatic execution of the garbage collector can be turned off to hold off on the execution until the
CPU can be spared.

\see \ref doc_gc

\ref asEP_DISABLE_SCRIPT_CLASS_GC

By default the script compiler will detect script classes that can potentially form circular references and will flag them for
garbage detection. If it is known that no circular references are formed, or if they are manually resolved in the script, it is
possible to disable this to increase performance.

\see \ref doc_gc
 
\ref asEP_COMPILER_WARNINGS

Compiler warnings can be turned off or treated as errors by setting this engine property.

\ref asEP_GENERIC_CALL_MODE

By default the \ref doc_generic "generic calling convention" treats the ref counts in the handles the same way that the native calling convention, i.e.
releases any handles passed to a function if they have been marked as auto handles, and increments the returned handle if it has 
been marked as auto handle.

If the behaviour used before 2.33.0 is desired for backwards compatibility, then set this property to 0. In this case the 
generic calling convention will always release references for handles received in arguments, and never increment references 
for returned handles. 

\ref asEP_NO_DEBUG_OUTPUT

When the library is built with AS_DEBUG it will write debug output to the folder AS_DEBUG by default. By turning on this engine property this debug output is disabled.



*/
