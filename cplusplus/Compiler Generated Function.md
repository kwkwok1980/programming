
# Compiler Generated Functions

http://www.enyo.de/fw/notes/cpp-auto-members.html 

- A copy constructor (which takes a single argument of type const T &) is generated if all base classes and members are copy-constructible. Note that reference members are copy-constructible.

- The parameter-less default constructor is generated if the class does not define any constructor and all base classes and members are default-constructible. This means that once you declare a copy constructor (perhaps to disable the automatically provided one, see below), the compiler will not supply a default construtor.

- The copy-assignment operator T &T::operator=(const T &) is generated if all base classes and members are copy-assignable. For this purpose, reference members are not considered copy-assignable.

- The destructor is always automatically supplied if possible, based on the members and the base classes.

- In C++11, A move constructor with an argument T && is supplied unless the class has any of the following: a user-defined copy constructor, copy-assignment operator, move-assignment operator, or destructor. If the move constructor cannot be implemented because not all bases or members are move-constructible, the supplied move constructor will be defined as deleted.

- Similarly, in C++11, a move-assignment operator T &T::operator=(T &&) is supplied under the same conditions.

https://en.wikipedia.org/wiki/Special_member_functions
