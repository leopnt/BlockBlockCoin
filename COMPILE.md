## Compile
```
gcc lib/*.c src/*.c main.c -o main -I./
```

## Execute
```
./main
```

## Leak test on MacOS
```
leaks -atExit -- ./main | grep LEAK:
```

## All in
```
gcc lib/*.c src/*.c main.c -o main -I./ && ./main && leaks -atExit -- ./main | grep LEAK:
