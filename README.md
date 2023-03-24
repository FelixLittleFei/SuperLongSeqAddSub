# Mini Project 1 (Jan. 2023): Two super long Integer Sequences Addition Subtraction

This program is implemented in C++. Your input parameters must contains an inputfile and could contain a 2nd parameter to denote the input base and a 3rd parameter to denote the output base. The length of the sequence could be much bigger than long long long int. You cannot use built-in types to store these 2 sequences.

### **This <300 lines of mini program includes** 

- Multiple input validity checks
- Character conversions
- Addition/subtraction handling
- Base conversions
- Leading zeros removal etc..

The base will be 2-36, which will be represented in your inputfile as 2-10 and A-Z(a-z). A represents 10, B represents 11, ... Z represents 36. The output will be digits and uppercase alphabets only, you could easily modified it to lowercase if you want.


### **Other notice:**
- The lengths of 2 Sequences could be different.
- Sign handling.

### **Usage (Program input parameters)**
"  --inputFile <filename>    inputfile \n"
  
"  --inputBase <number>     input decimal btw 2-36 (optional) \n"
  
"  --outputBase <number>       output decimal btw 2-36 (optional) \n";

### **Example (You may need your program name at the beginning as well)**
inputFile
  
inputFile 10 16
  
inputFile 2
  
inputFile 10 2

## Testing
#### (1) The contents of the input file base 10 could be:
98

-10000001

#### The output in radix 10 must be:
-9999903


#### (2) The contents of the input file base 36 could be:
  
-czvjzkldfjg000z0z0z0eopogs54867g0fgert6000045jhgrt07575fgbir0ew0465233kcw

15w34fdgvntoopl4k40000088846g468797908z0z000zzz0009hazz

#### The result:
  
If Base 36:
-CZVJZKLDFJG000Z0Z0XUILL93B9GEHIQFAWART5ZZRRW1D1CLKSXXY56HAJR0DW05651TM9CX

If Base 10:
-147098694550647835620679717570878378783899165875780434014120749235398411979380429160987449599626877947229738863073

If Base 16:
-F4AA088C573511C527C857A2C294D961E23AC06CD4DFDB59E5D38122C274B09BA46A09B982E652E02442A9AC0789E1

## Math Tutorial
An interesting reference for base conversion algorithm: https://www.cs.colostate.edu/~cs270/.Spring12/Notes/NumberSystems
Hope it helps!
