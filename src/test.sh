#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

# assert 0 "0;"
# assert 42 "42;"
# assert 21 "5+20-4;"
# assert 41 " 12 + 34 - 5 ;"
# assert 47 "5+6*7;"
# assert 15 "5*(9-6);"
# assert 4 "(3+5)/2;"

# assert 10 "-10+20;"

# assert 0 "0==1;"
# assert 1 '42==42;'
# assert 1 '0!=1;'
# assert 0 '42!=42;'

# assert 1 '0<1;'
# assert 0 '1<1;'
# assert 0 '2<1;'
# assert 1 '0<=1;'
# assert 1 '1<=1;'
# assert 0 '2<=1;'

# assert 1 '1>0;'
# assert 0 '1>1;'
# assert 0 '1>2;'
# assert 1 '1>=0;'
# assert 1 '1>=1;'
# assert 0 '1>=2;'
# assert 6 'aa=2+4;'
# assert 2 'a = 1; b = a + 1;'
# assert 4 "a = 2; b = 2; a + b;"
# assert 30 "return 30;"
# assert 30 "foo = 10; bar = 20; return foo + bar;"
# assert 0 'return 1>=2;'

# assert 1 "if(1) return 1==1;"
# assert 1 "if(3) return 2>=1;"
# assert 0 "if(0) return 2>=1; return 0;"

assert 1 "if(1) return 2>=1; else return 1>=1;"
assert 0 "if(0) return 2>=1; else return 0;"
assert 0 "if(0) return 2>=1; else return 0>2;"
assert 1 "if(1) return 1; else if(0) return 2; else return 3;"
assert 2 "if(0) return 1; else if(1) return 2; else return 3;"
assert 3 "if(0) return 1; else if(0) return 2; else return 3;"

echo OK