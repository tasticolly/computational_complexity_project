# computational_complexity_project
## Theory
[Article](tex/complexity.pdf)

## Запуск программы
В директории src ужно выполнить следующую последовательность команд:  
```
mkdir build
cd build
cmake ..
make
```
Теперь в директории build готовы два исполняемых файла RuralPostmanProblem - сама программа, в которой вы можете получить ответ для своих примеров, и performance - тесты производительности
Тесты запускать надо следующим образом 
```
./performance --benchmark_out_format=json --benchmark_out=<filename>
```
<filename> - имя файла, в который записываются результаты.
