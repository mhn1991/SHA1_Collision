#include<openssl/sha.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<locale.h>
#include<time.h>

void H(unsigned char * data, unsigned char *hash,int nibbles){
  SHA1(data,nibbles,hash);
}

void print(unsigned char *hashString,int nibbles){
  for(int i=0;i<nibbles;i++){
    printf("%c",hashString[i]);
  }
}

//copy a to b
void copy(unsigned char *a,unsigned char *b){
  strcpy(b,a);
}

// sprintf is about 8 times slower than this 
void ShaToString(unsigned char *hash,unsigned char *hashString)
{
  const char * hex = "0123456789abcdef";
  for(int i=0;i<SHA_DIGEST_LENGTH;i++){
    //sprintf((unsigned char*)&(hashString[i*2]), "%02x", hash[i]);
    hashString [2*i]   = hex[(hash[i] & 0xF0) >> 4];
    hashString [2*i+1] = hex[hash[i] & 0x0F];
  }
}

int compare(unsigned char *a,unsigned char *b,int nibbles){
  for(int i=0;i<nibbles;i++){
    if(a[i] != b[i]){
      return 0;
    }
  }
  return 1;
}

void breakSha(unsigned char *hash,unsigned char *hashString,int nibbles){
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
    H(a,hash,nibbles);
    ShaToString(hash,hashString);
    copy(hashString,a);
    //=================================================
    // b = H(H(b))
    H(b,hash,nibbles);
    ShaToString(hash,hashString);
    H(hashString,hash,nibbles);
    ShaToString(hash,hashString);
    copy(hashString,b);
    if(compare(a,b,nibbles) == 1){
	printf("number of itration that we found the collision: %d\n",i);
	break;
      }
    i++;
  }
  if(compare(a,b,nibbles) == 0){
    printf("a and b are not equal\n");
    return;
  }
  copy(a,b);
  copy(x0,a);
  unsigned char *ha = (unsigned char *) calloc((SHA_DIGEST_LENGTH * 2)+1,sizeof(unsigned char *));
  unsigned char *hb = (unsigned char *) calloc((SHA_DIGEST_LENGTH * 2)+1,sizeof(unsigned char *));
  while(1){
    H(a,hash,nibbles);
    ShaToString(hash,hashString);
    copy(hashString,ha);
    H(b,hash,nibbles);
    ShaToString(hash,hashString);
    copy(hashString,hb);
    if(compare(ha,hb,nibbles) ==1){
      printf("find collision and strings are : \n");
      print(a,nibbles);
      printf("\n");
      print(b,nibbles);
      printf("\nand their hash is : ");
      print(ha,nibbles);
      printf("\n");
      break;
    }else{
      H(a,hash,nibbles);
      ShaToString(hash,hashString);
      copy(hashString,a);
      H(b,hash,nibbles);
      ShaToString(hash,hashString);
      copy(hashString,b);
    }
  }
}

void main(int argc,char *argv[]){
  //int nibbles;
  if(argc == 1){
    clock_t start = clock();
    printf("program will run to find collision until 10 nibles\n");
    int nibbles = 10;
    unsigned char * hash = (unsigned char *) calloc(SHA_DIGEST_LENGTH,sizeof(unsigned char *));
  //because we used %02x we need 2*Sha_size and because we close string with \0 i add +1 to the size                                                                                                                                                                               
  unsigned char * hashString = (unsigned char *) calloc((SHA_DIGEST_LENGTH * 2)+1,sizeof(unsigned char *));
  //you can init algorithm with any input                                                                                                                                                                                                                                          
  //This program init with string that has length 10 if you want more you have to initialize it yourself                                                                                                                                                                           
  H("3bab8ab772",hash,nibbles);//initializing algorithm with mojtaba hash value                                                                                                                                                                                                    
  ShaToString(hash,hashString);
  printf("program initialized with : ");
  print(hashString,nibbles);
  printf("\n");
  breakSha(hash,hashString,nibbles);
  clock_t finish = clock();
  printf("time take to find collision: %f\n",(float)(finish - start) / CLOCKS_PER_SEC);
  }
  else if(argc == 3){
    clock_t start = clock();
    int nibbles = atoi(argv[2]);
    char *str = argv[1];
    printf("you initilized algorithm with string:%s and number of nibbles you want is: %d\n",str,nibbles);
    unsigned char * hash = (unsigned char *) calloc(SHA_DIGEST_LENGTH,sizeof(unsigned char *));
  //because we used %02x we need 2*Sha_size and because we close string with \0 i add +1 to the size                                                                                                                                                                              
  unsigned char * hashString = (unsigned char *) calloc((SHA_DIGEST_LENGTH * 2)+1,sizeof(unsigned char *));
  //you can init algorithm with any input 
  H(str,hash,nibbles);//initializing algorithm with mojtaba hash value                                                                                                                                                                                                   
  ShaToString(hash,hashString);
  printf("program initialized with : ");
  print(hashString,nibbles);
  printf("\n");
  breakSha(hash,hashString,nibbles);
  clock_t finish = clock();
  printf("time take to find collision: %f\n",(float)(finish - start) / CLOCKS_PER_SEC);
  }else{
    printf("usage is :\n 1-just name of program to run with default values\n2- <program name> <string> <size of string>\n");
  }
}
