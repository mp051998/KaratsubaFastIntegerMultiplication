#include <iostream>
#include <cstring>  
using namespace std;

const int MAX_LENGTH = 10000;  // MAX_LENGTH is the length of the integer to be multiplied. Change this is you wish to multiply numbers having a greater length.
 
//This function is used by the make_equal_length function to append leading zeroes to the muliplicand having lesser number of digits.
void add_zeros(char* a, int n){
  int length = strlen(a);
  int i;
  for (i = length - 1 + n; i >= n; i--)
    a[i] = a[i - n];
  a[length + n] = 0;
  for (i = 0; i < n; i++)
    a[i] = '0';
}

//This function is used to remove the leading zeroes once we have obtained the final result.
void remove_zeros(char* a){
  int lena = strlen(a);
  int ind = 0;
  while (ind < lena && a[ind] == '0')
    ++ind;
  for (int i = ind; i < lena; ++i)
    a[i - ind] = a[i];
  a[lena - ind] = 0;
}

void sum(char* a, char* b, char* res){
  int lena = strlen(a);
  int lenb = strlen(b);

  if (lena < lenb){
    swap(a, b);
    swap(lena, lenb);
  }
 
  int toAdd = 0;
  for (int inda = lena - 1, indb = lenb - 1; inda >= 0; --inda, --indb)
    {
        int x = a[inda] - '0';
        int y = indb >= 0 ? b[indb] - '0' : 0;
 
        int curr = x + y + toAdd;
 
        if (curr >= 10){
          toAdd = 1;
          curr -= 10;
        }
        else
          toAdd = 0;
 
        res[inda] = curr + '0';
    }
 
  if (toAdd == 1){
    add_zeros(res, 1);
    res[0] = '1';
  }
}
 
// assume that a > b
void sub(char* a, char* b, char* res){
  int lena = strlen(a);
  int lenb = strlen(b);

  int toSub = 0;
  for (int inda = lena - 1, indb = lenb - 1; inda >= 0; --inda, --indb){
    int x = a[inda] - '0';
    int y = indb >= 0 ? b[indb] - '0' : 0;
 
    if (toSub == 1)
      x--;

    int curr;
    if (x < y){
      curr = x + 10 - y;
      toSub = 1;
    }
   else{
      curr = x - y;
      toSub = 0;
   }
 
    res[inda] = curr + '0';
  }
}
 
// returns a * 10^n
void mult10(char* a, int n){
  int lena = strlen(a);
  if (lena == 1 && a[0] == '0')
    return;

  for (int i = lena; i < lena + n; ++i)
    a[i] = '0';

  a[lena + n] = 0;
}
 
char* CreateArray(int len){
  char* res = new char[len];
  memset(res, 0, len);
  return res;
}
 
// add leading zeros if needed
void make_equal_length(char* a, char* b){
  int lena = strlen(a);
  int lenb = strlen(b);
  int n = max(lena, lenb);
  add_zeros(a, n - lena);
  add_zeros(b, n - lenb);
}
 
void karatsuba(char* x, char* y, char* res){
  make_equal_length(x, y);
  int len = strlen(x);
  if (len == 1){
    //char value - '0' returns the integer value of the character which in our case is the digit itself.
    int val = (x[0] - '0') * (y[0] - '0');
    if (val < 10)
      res[0] = val + '0';
    else{
      res[0] = (val / 10) + '0';
      res[1] = (val % 10) + '0';
    }
  }
  else{
    char* xl = CreateArray(len);
    char* xr = CreateArray(len);
    char* yl = CreateArray(len);
    char* yr = CreateArray(len);
 
    int rightSize = len / 2;
    int leftSize = len - rightSize;
    //we decalre leftSize as len - rightSize to account for odd lengths as well.

    //strcpy( ) function copies whole content of one string into another string. Whereas, strncpy( ) function copies portion of contents of one string into another string.
    //If destination string length is less than source string, entire/specified source string value won’t be copied into destination string in both cases.
    strncpy(xl, x, leftSize);
    strcpy(xr, x + leftSize);
    strncpy(yl, y, leftSize);
    strcpy(yr, y + leftSize);
    
    int maxl = 3 * len;
    char* P1 = CreateArray(maxl);
    char* P2 = CreateArray(maxl);
    char* P3 = CreateArray(maxl);
 
    karatsuba(xl, yl, P1);
    karatsuba(xr, yr, P2);
 
    char* tmp1 = CreateArray(maxl);
    char* tmp2 = CreateArray(maxl);
    sum(xl, xr, tmp1);
    sum(yl, yr, tmp2);
    karatsuba(tmp1, tmp2, P3);
    sub(P3, P1, P3);
    sub(P3, P2, P3);
    mult10(P3, rightSize);
    mult10(P1, 2 * rightSize);
    sum(P1, P2, res);
    sum(res, P3, res);
    remove_zeros(res);
 
  //The following operations are done for the sake of freeing up memory.
    delete[] xl;
    delete[] xr;
    delete[] yl;
    delete[] yr;
    delete[] tmp1;
    delete[] tmp2;
    delete[] P1;
    delete[] P2;
    delete[] P3;
  }
}
 
int main(){
  char a[MAX_LENGTH], b[MAX_LENGTH];
  cout<<"Enter the two numbers to be multiplied: \n";
  cin>>a>>b;
 
  char* res = CreateArray(MAX_LENGTH);
  
  karatsuba(a, b, res);
  cout<<"The product of the entered nos. is: "<<res<<"\n";
  return 0;
}
