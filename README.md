# metaprog

Почему DecompressorType(void* internal) лучше, чем decompress(void* internal)?

Первое предположение: потому что тогда это становится очень похоже на паттерн Command: все данные передаются конструктору, вызываемая функция void(void). Легко делегировать исполнение куда угодно.

Но лектор в письме ответил, что это не то, что хочется услышать. "Идея передавать данные в конструктор (а не аргументами в функцию) служит лишь для дополнительных оптимизаций"

Второе предположение:

Мы живём в x64, где действую calling convention Microsoft x64 calling convention на Windows и System V AMD64 ABI на других платформах. Суть их в том, что аргументы в функцию передаются через какие-то регистры, и только если параметров больше, чем это число регистров, они начинают передаваться через стек.

При вызове новой функции значения этих регистров сохраняются на стеке, потому что считается, что вызов функции ломает их значения, *если не доказано обратное*.

Для функции void(void) можно явно сказать, что её вызов не будет изменять значения этих переменных, и их можно не сохранять.

Да, у нас на самом деле void(DecompressorType* this), но возможно, что компиляторы умеют сохранять значения только одного этого регистра, который используется для передачи this.
