// c11.cpp : features
//

#include "c11.h"
#include <mutex>

using namespace std;

namespace FINAL{
	struct A {
		virtual void foo();
	};

	struct B : A {
		void foo() final; // final override of foo, cat be overriden anymore
	};

	struct C final: B { // C cannot be inherited

	};
}

namespace OVERRIDE {
	struct A {
		virtual void foo();
	};

	struct B : A {
		void foo() override; // QoC and compile error if not overriding
	};
}

namespace TRAILING_RETURN_TYPE {
	struct A {
		template <class T, class U> 
		auto add(T t, U u) -> decltype(t + u) {
			return t + u;
		}
	};
	static int val = 3;
	auto fun = []() -> int& {return val; }; //can force lamda to return by reference
}

namespace RVALUE_REFERENCE {
	int a = 3;
	int&& b = a + a;
}

namespace MOVE_CONSTRUCTOR {
	struct A {
		int n;
		string s;
		A(A&& a) noexcept : 
			s(move(a.s)), 
			n(move(a.n)) {
		}
		A& operator=(A&& a) noexcept {
			return *this;
		}
	};
}

namespace SCOPED_ENUM {
	enum class A {
		b, c=90, japko
	};

	A getJapko(){
		return A::japko;
	}
}

namespace CONSTEXPR {
	constexpr int factorial(int n)
	{
		return n <= 1 ? 1 : (n * factorial(n - 1));
	}

	template<int n>
	struct constN
	{
		constN() { std::cout << n << '\n'; }
	};

	const int b = 3;

	constN<factorial(b)> c;
}

namespace LIST_INITIALISATION {
	std::map<int, std::string> m = { // nested list-initialization
	   {1, "a"},
	   {2, {'a', 'b', 'c'} },
	   {3, "b"}
	};
}

namespace DELEGATE_CONSTRUCTOR {
	class Foo {
	public:
		Foo(char x, int y) {}
		Foo(int y) : Foo('a', y) {} // Foo(int) delegates to Foo(char,int)
	};
}

namespace INHERITED_CONTRUCTOR {
	struct A {
		int c;
		A() = delete;
		A(int a) :c(a) {
			cout << "IC A constructor" << endl;
		}
	};
	struct B : A {
		using A::A;
		B() = delete;
	};

	B b(2);
}

namespace TYPE_ALIAS {
	using dupa = int;

	dupa a = 5;
}

namespace USER_LITERAL {
	
	int operator"" _w(char hs) {
		cout << "conversion user_literal" << endl;
		return (int)hs * 2.0;
	}

	int a = 'a'_w;
}

namespace ATTRIBUTES {
	[[noreturn]][[deprecated]]
	void printDupa() {
		cout << "dupa" << endl;
	}
}

namespace LAMBDAS {
	auto lubieLambdy = []() ->string {
		cout << "som dobre" << endl;
		return "lubie lambdy";
	};

	string sLubieLambdy = lubieLambdy();
}

namespace THREAD_LOCAL {
	thread_local unsigned int rage = 1;
	std::mutex cout_mutex;
	void increase_rage(const std::string& thread_name)
	{
		++rage; // modifying outside a lock is okay; this is a thread-local variable
		std::lock_guard<std::mutex> lock(cout_mutex);
		std::cout << "Rage counter for " << thread_name << ": " << rage << '\n';
	}
}

namespace NOEXCEPT {
	void may_throw();
	void no_throw() noexcept;
}

namespace ALIGNAS {
	struct alignas(8) A {};
	struct alignas(16) B { A a; };
	//struct alignas(8) C { B b; }; error
}

namespace PARAMETER_PACK {
	template<typename T>
	T adder(T first) {
		return first;
	}

	template<typename T, typename... Args>
	T adder(T first, Args... args) {
		return first + adder(args...);
	}

	template<typename... Args>
	int sumJaj(Args... args) { return (... + args); } // superior fold expression

	template<int N>
	int jaja() { return N; }
}

int main()
{
	// auto 
	auto a = 5;

	// decltype
	decltype(a)& b = a;

	{
		using namespace THREAD_LOCAL;
		std::thread a(increase_rage, "a"), b(increase_rage, "b");

		{
			std::lock_guard<std::mutex> lock(cout_mutex);
			std::cout << "Rage counter for main: " << rage << '\n';
		}

		a.join();
		b.join();
	}
	
	{
		using namespace NOEXCEPT;
		std::cout << std::boolalpha
			<< "Is may_throw() noexcept? " << noexcept(may_throw()) << '\n'
			<< "Is no_throw() noexcept? " << noexcept(no_throw()) << '\n';
	}

	{
		using namespace PARAMETER_PACK;
		cout << adder(1, 2, 3) << endl;
		cout << sumJaj(1, 2, 3) << endl;
		cout << jaja<12>() << endl;
	}

	return 0;
}
