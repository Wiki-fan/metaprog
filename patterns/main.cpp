#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <set>

class IB {
 public:
    virtual void Do() = 0;
};
class B : public IB {
 public:
    void Do() override {
        std::cout << "Do" <<std::endl;
    }
};

class A {
 public:
    void DoSomethingWithB(IB& b) {
        std::cout << "A does ";
        b.Do();
    }

    void Update(B& b) {
        std::cout << "A observed B and noticed change" <<std::endl;
    }
};

enum class Role {
    Proxy, Mediator, Observer
};

template<Role, typename A, typename B>
class C {

};

template <typename A, typename B>
class C<Role::Proxy, A, B> : public IB {
 private:
    std::shared_ptr<IB> b;
 public:
    C() :b(new B()) {

    }

    void Do() override {
        std::cout << "(proxy)";
        b->Do();
    }
};

template <typename A, typename B>
class C<Role::Mediator, A, B> : public B {
 private:
    B b;
 public:
    C(A a) {

    }

    std::vector<B&> objects;

    void addObject(const B& b) {
        objects.push_back(b);
    }
};

template <typename A, typename B>
class C<Role::Observer, A, B> : public B {
 private:
    B b;
 public:
    // A is Observer
    // B is Subject
    C() {}

    std::map<B&, std::set<A&>> pairs;

    void attachAtoB(A& a, B& b) {
        if (pairs[b]) {
            pairs.at(b).insert(a);
        } else {
            pairs[b] = std::set<A&>({a});
        }
    }

    void detachAfromB(A& a, B& b) {
        pairs[b].remove(a);
    }

    void notify(B& b) {
        for (auto& a: pairs[b]) {
            a.Update(b);
        }
    }

    std::vector<B&> objects;
};

int main() {
    C<Role::Proxy, A, B> cProxy;

    B b;

    A a;
    a.DoSomethingWithB(b);
    a.DoSomethingWithB(cProxy);

    C<Role::Observer, A, B> cObserver;
    cObserver.attachAtoB(a, b);
    cObserver.notify(b);

}

// класс exception, получает указатель на std::thread потока где возник и где должен быть обработан. И надо механизм, который автоматизирует обработку. Шаблонная реализация.