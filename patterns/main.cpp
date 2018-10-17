#include "A.h"
#include "B.h"
#include "C.h"

// Структура классов для Mediator:
// B - самолёты, number - бортовой номер (уникален)
// A - главный штаб, говорит самолётам, что делать
// C<Role::Mediator> - диспетчерская вышка.
// Общение штаба и вышки однонаправленное, самолётов и вышки - двунаправленное.
// Медиатор координирует использование общего ресурса - взлётной полосы
int main() {
    C<Role::Mediator> cMediator;
    C<Role::Observer> cObserver;

    B b0(0, &cMediator);
    B b1(1, &cMediator);
    C<Role::Proxy> cProxy(2, &cMediator);

    A a;
    a.DoSomethingWithB(b0);
    a.DoSomethingWithB(cProxy);

    a.OrderLanding(&cMediator, 1);
    b1.ReportLandedSuccessfully();

    cObserver.attachAtoB(&a, &b0);
    cObserver.notify(&b0);
}

// класс exception, получает указатель на std::thread потока где возник и где должен быть обработан. И надо механизм, который автоматизирует обработку. Шаблонная реализация.