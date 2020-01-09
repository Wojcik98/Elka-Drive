# Elka Drive Desktop
Projekt ma na celu stworzenie aplikacji umożliwiającej grupową współpracę.
Repozytorium zawiera aplikację frontendową służącą do komunikacji z serwerem, którego kod znajduje się na [Github/Elka-Drive-Backend](https://github.com/breader124/Elka-Drive-Backend).

## Budowanie
Przed zbudowaniem należy stworzyć plik `Elka-Drive-Desktop/rabbitmqsecrets.h` na podstawie pliku `Elka-Drive-Desktop/rabbitmqsecrets.h.template` i umieścić w nim dane do RabbitMQ.

Aplikacja korzysta z Qt w wersji 5.12, do zbudowania wymagana jest wcześniejsza instalacja tego toolkitu.

### Linux
Należy uruchomić w głównym folderze:
```
qmake
make
```
### Windows
Należy uruchomić w głównym folderze:
```
qmake.exe
mingw32-make.exe
```

## Testowanie
Po zbudowaniu aplikacji należy uruchomić plik wykonywalny z podprojektu ProcessResponse.
W systemie Linux jest to `ProcessResponse/test`, w systemie Windows `ProcessResponse/release/test.exe`.
Kod jest automatycznie testowany na platformie Github po każdej akcji `push` do repozytorium.
Wyniki testów można zobaczyć wchodząc w zakładkę `Actions`.

