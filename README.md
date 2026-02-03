# Proxy
## Обзор
Прокси-сервер для СУБД Postgresql для логирования SQL запросов без создания дополнительных потоков (thread).
## Шаги
1. Чтобы в прокси были видны SQL запросы в незашифрованном виде, необходимо отключить SSL на клиенте и сервере
2. Выполнить следующие команды:
```bash
make
cd build/
./proxy.out -port=<proxy port> -pg_addr=<PostgreSQL address> -pg_port=<PostgreSQL port> -log_file=<log file>
```
Пропущенные аргументы будут заменены на аргументы по умолчанию:
```bash
./proxy.out -port=5555 -pg_addr=127.0.0.1 -pg_port=5432 -log_file=log.txt
```
3. Со стороны клинета:
```bash
psql -U <username> -d <database name> -h <proxy address> -p <proxy port> --set=sslmode=disable
```
4. Для корректного завершения процесса обработаны сигналы SIGINT (^C) и SIGTERM (kill <pid>)
   Добавил просто для завершения ПОСЛЕ всех запросов. Не тестировал случаи, когда завершаем во время общения клиентов и сервера
5. Для очистки:
```bash
cd ..
make clean
```
## Пример
База данных:
```
testdb

 id |    name    |     email     |        created_at
----+------------+---------------+----------------------------
 1  | Иван       | ivan@test.ru  | 2026-02-01 20:33:24.285971
 2  | Мария      | maria@test.ru | 2026-02-01 20:33:24.285971
 3  | Петр       | petr@test.ru  | 2026-02-01 20:33:24.285971
```
Запсукаем прокси:
```bash
make
cd build/
./proxy.out
```
Подключаем клиента (вводим пароль):
```bash
psql -U postgres -d testdb -h 127.0.0.1 -p 5555 --set=sslmode=disable
```
Отправляем запросы:
```sql
SELECT * FROM test_users WHERE email = 'ivan@test.ru';
```
В файле логов появятся запросы.
