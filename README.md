# aecapital
broadcast summary order book data to clients test

### Steps to build
```
git clone https://github.com/chavanrc/aecapital.git
cd aecapital/
mkdir build
cd build/
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
### If want to perform benchmarks then build with -DBENCHMARK_ENABLED=ON
```
cmake -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLED=ON ..
make
```
### To Run:
You can find your lib and test exe in build/stage
```
./tests/bin/Tests -g ./data/practise.dat "aecapital file test"
```
### To Run Testcases:
```
./tests/bin/Tests "create order book test"
```
### To run benchmarks
```
./tests/bin/Tests "aecapital benchmark test"
```
### You can use docker as well
```
docker build -f ./docker/Dockerfile -t="ubuntu:aecapital" .
docker run -it -v /home/{user}/CLionProjects/aecapital/:/home/user/aecapital --cap-add=SYS_PTRACE ubuntu:aecapital bash
```
