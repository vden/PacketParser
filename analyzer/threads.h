#ifndef _EXT_THREAD_H
#define _EXT_THREAD_H

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <signal.h>
#endif

#ifdef WIN32
typedef HANDLE ThreadType;
#else
typedef pthread_t ThreadType;
#endif

class Thread {
public:
  Thread() {}
  virtual ~Thread();

  // Функция запуска потока. Ее нельзя совместить с конструктором
  // класса, так как может случиться, что поток запустится до того,
  // как объект будет полностью сформирован. А это может спокойно
  // произойти, если вызвать pthread_create или CreateThread в
  // в конструкторе. А вызов виртуальной функции в конструкторе,
  // да еще и в конструкторе недосформированного объекта,
  // в лучшем случае приведет к фатальной ошибке вызова чисто
  // виртуальной функции, либо в худшем случае падению программы
  // с нарушением защиты памяти. Запуск же потока после работы
  // конструктора избавляет от этих проблем.
  void Start();

  // Главная функция потока, реализующая работу потока.
  // Поток завершается, когда эта функция заканчивает работу.
  // Крайне рекомендуется ловить ВСЕ исключения в данной функции
  // через try-catch(...). Возникновение неловимого никем
  // исключения приведет к молчаливому падению программы без
  // возможности объяснить причину.
  virtual void Execute() = 0;

  // Присоединение к потоку.
  // Данная функция вернет управление только когда поток
  // завершит работу. Применяется при синхронизации потоков,
  // если надо отследить завершение потока.
  void Join();

  // Уничтожение потока.
  // Принудительно уничтожает поток извне. Данный способ
  // завершения потока является крайне нерекомендуемым.
  // Правильнее завершать поток логически, предусмотрев
  // в функции Execute() условие для выхода, так самым
  // обеспечив потоку нормальное завершение.
  void Kill();

private:
  ThreadType __handle;

  // Защита от случайного копирования объекта в C++
  Thread(const Thread&);
  void operator=(const Thread&);
};


#endif
