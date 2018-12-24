#include<openssl/sha.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<locale.h>

void H(unsigned char * data, unsigned char *hash){
  SHA1(data,10,hash);
}

void print(unsigned char *hashString){
  for(int i=0;i<10;i++){
    printf("%c",hashString[i]);
  }
}

//copy a to b
void copy(unsigned char *a,unsigned char *b){
  strcpy(b,a);
}

void ShaToString(unsigned char *hash,unsigned char *hashString)
{ 
  for(int i=0;i<SHA_DIGEST_LENGTH;i++){
    sprintf((unsigned char*)&(hashString[i*2]), "%02x", hash[i]);
  }
}

int compare(unsigned char *a,unsigned char *b){
  for(int i=0;i<10;i++){
    if(a[i] != b[i]){
      return 0;
    }
  }
  return 1;
}

void breakSha(unsigned char *hash,unsigned char *hashString){
  unsigned char *a = (unsigned char *) calloc((SHA_DIGEST_LENGTH * 2)+1,sizeof(unsigned char *));
  unsigned char *b = (unsigned char *) calloc((SHA_DIGEST_LENGTH * 2)+1,sizeof(unsigned char *));
  unsigned char *x0= (unsigned char *) calloc((SHA_DIGEST_LENGTH * 2)+1,sizeof(unsigned char *));
  copy(hashString,a);
  copy(hashString,b);
  copy(hashString,x0);
  int i = 0;
  while(1){
    if(i%1000000 == 0){
      setlocale(LC_NUMERIC, "");
      printf("number of itration : %'d\r",i);
    }
    // a = H(a)
    H(a,hash);
    ShaToString(hash,hashString);
    copy(hashString,a);
    //=================================================
    // b = H(H(b))
    H(b,hash);
    ShaToString(hash,hashString);
    H(hashString,hash);
    ShaToString(hash,hashString);
    copy(hashString,b);
      if(compare(a,b) == 1){
	printf("number of itration that we found the collision: %d\n",i);
	break;
      }
    i++;
  }
  if(compare(a,b) == 0){
    printf("a and b are not equal\n");
    return;
  }
  copy(a,b);
  copy(x0,a);
  unsigned char *ha = (unsigned char *) calloc((SHA_DIGEST_LENGTH * 2)+1,sizeof(unsigned char *));
  unsigned char *hb = (unsigned char *) calloc((SHA_DIGEST_LENGTH * 2)+1,sizeof(unsigned char *));
  while(1){
    H(a,hash);
    ShaToString(hash,hashString);
    copy(hashString,ha);
    H(b,hash);
    ShaToString(hash,hashString);
    copy(hashString,hb);
    if(compare(ha,hb) ==1){
      printf("find collision and strings are : \n");
      print(a);
      printf("\n");
      print(b);
      printf("\nand their hash is : ");
      print(ha);
      printf("\n");
      break;
    }else{
      H(a,hash);
      ShaToString(hash,hashString);
      copy(hashString,a);
      H(b,hash);
      ShaToString(hash,hashString);
      copy(hashString,b);
    }
  }
}

void main(){
  unsigned char * hash = (unsigned char *) calloc(SHA_DIGEST_LENGTH,sizeof(unsigned char *));
  //because we used %02x we need 2*Sha_size and because we close string with \0 i add +1 to the size 
  unsigned char * hashString = (unsigned char *) calloc((SHA_DIGEST_LENGTH * 2)+1,sizeof(unsigned char *));
  //
  H("3bab8ab772",hash);//initializing algorithm with mojtaba hash value
  ShaToString(hash,hashString);
  printf("program initialized with : ");
  print(hashString);
  printf("\n");
  breakSha(hash,hashString);
}
