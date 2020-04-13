# KV-store
A simple KV-store system

reference:

[C++ Bloom Filter Library]:https://github.com/ArashPartow/bloom

​	

Test

- [x] read/write 10 int

  

- [x] read/write 1000 int

  page size = 512

  hash number = 100

  filter page size = 1024

  block page size = 16 * 1024
  
  

- [ ] read/write 100000 int

  w&r  ->8s

  read -> 8s

- [ ] read/write 200000 long long

  block page size = 4 * 1024

  filter page size = 1 * 1024

  hash number = 100

  page size = 512

  w&r -> 30s

- [ ] read/write 100000 long long

  block page size = 4 * 1024

  filter page size = 1 * 1024

  hash number = 100

  page size = 512

  w&r -> 12s

  --

  block page size = 16 * 1024 

  w&r -> 8s

  (用100w的参数时7s)

- [ ] read/write 1e6 long long

  block page size = 512 * 1024

  filter page size = 16 * 1024

  hash number = 100

  page size = 8 * 1024

  w&r=1m24s

  r      =1m24s