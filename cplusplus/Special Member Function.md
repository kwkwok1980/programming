# Special Member Function

https://accu.org/index.php/journals/1896

The C++ standard [N1316] mandates that implicit versions will be created if a user doesn’t declare them explicitly:

§ 12.4 / 3 If a class has no user-declared destructor, a destructor is declared implicitly

§ 12.8 / 4 If the class definition does not explicitly declare a copy constructor, one is declared implicitly.

§ 12.8 / 10 If the class definition does not explicitly declare a copy assignment operator, one is declared implicitly

C++11 [N3242] added wording to the Standard, deprecating the previous behavior.

D.3 Implicit declaration of copy functions [depr.impldec]

The implicit definition of a copy constructor as defaulted is deprecated if the class has a user-declared copy assignment operator or a user-declared destructor. The implicit definition of a copy assignment operator as defaulted is deprecated if the class has a user-declared copy constructor or a user-declared destructor. In a future revision of this International Standard, these implicit definitions could become deleted

This means that compilers keep generating a defaulted copy constructor, assignment operator and destructor if no user-defined declaration is found, but at least now they might issue a warning.

C++11 [N3242] says that move operations are ONLY implicitly declared if the following set of conditions occur:

§ 12.8 / 9

If the definition of a class X does not explicitly declare a move constructor, one will be implicitly declared as defaulted if and only if

X does not have a user-declared copy constructor,
X does not have a user-declared copy assignment operator,
X does not have a user-declared move assignment operator,
X does not have a user-declared destructor, and
The move constructor would not be implicitly defined as deleted.
§ 12.8 / 20

If the definition of a class X does not explicitly declare a move assignment operator, one will be implicitly declared as defaulted if and only if

X does not have a user-declared copy constructor,
X does not have a user-declared move constructor,
X does not have a user-declared copy assignment operator,
X does not have a user-declared destructor, and
The move assignment operator would not be implicitly defined as deleted.
Unlike the Rule of three, the Rule of Five is partially enforced by the compiler. You get a default move constructor and move assignment operator if and only if none of the other four are defined/defaulted by the user.

C++14 expanded the wording, and now an explicit declaration of a move constructor or move assignment operator marks the defaulted copy constructor and assignment operator as deleted. This means that explicit move operations make your objects non-copyable/assignable by default. This is as close as you get to a real enforcement of the rule of five by a compiler.

§ 12.8 / 7

If the class definition does not explicitly declare a copy constructor, one is declared implicitly. If the class definition declares a move constructor or move assignment operator, the implicitly declared copy constructor is defined as deleted; otherwise, it is defined as defaulted (8.4). The latter case is deprecated if the class has a user-declared copy assignment operator or a user-declared destructor.

§ 12.8 / 18

If the class definition does not explicitly declare a copy assignment operator, one is declared implicitly. If the class definition declares a move constructor or move assignment operator, the implicitly declared copy assignment operator is defined as deleted; otherwise, it is defined as defaulted (8.4). The latter case is deprecated if the class has a user-declared copy constructor or a user-declared destructor.

http://www.enyo.de/fw/notes/cpp-auto-members.html 

- The parameter-less default constructor is generated if the class does not define any constructor and all base classes and members are default-constructible. This means that once you declare a copy constructor (perhaps to disable the automatically provided one, see below), the compiler will not supply a default construtor.

- A copy constructor (which takes a single argument of type const T &) is generated if all base classes and members are copy-constructible. Note that reference members are copy-constructible.

- The copy-assignment operator T &T::operator=(const T &) is generated if all base classes and members are copy-assignable. For this purpose, reference members are not considered copy-assignable.

- The destructor is always automatically supplied if possible, based on the members and the base classes.

- In C++11, A move constructor with an argument T && is supplied unless the class has any of the following: a user-defined copy constructor, copy-assignment operator, move-assignment operator, or destructor. If the move constructor cannot be implemented because not all bases or members are move-constructible, the supplied move constructor will be defined as deleted.

- Similarly, in C++11, a move-assignment operator T &T::operator=(T &&) is supplied under the same conditions.

https://en.wikipedia.org/wiki/Special_member_functions

- Default constructor if no other constructor is explicitly declared.

- Copy constructor if no move constructor and move assignment operator are explicitly declared. If a destructor is declared generation of a copy constructor is deprecated (C++11, proposal N3242 [2]).

- Copy assignment operator if no move constructor and move assignment operator are explicitly declared. If a destructor is declared generation of a copy assignment operator is deprecated.

- Move constructor if no copy constructor, copy assignment operator, move assignment operator and destructor are explicitly declared.

- Move assignment operator if no copy constructor, copy assignment operator, move constructor and destructor are explicitly declared.
