#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define USER_ASCII_KEY 4
#define STR_ASCII_KEY 9

struct dati{
	char user[8];
	char str1[141];
	char b64USER[13];
	char b64STR1[189];
	char binUSER[97];
	char binSTR[1505];
	char total_encrypted[1601];
	char da_inserire_in_QR[1748];
};
char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *stringToBinary(char *s);
int conta_caratteri(char* str);
void encodeblock( unsigned char in[], char b64str[], int len );
void b64_encode(char *clrstr, char *b64dst);
void completa_stringa(char* str);
void cripta_user(char* str);
void cripta_str(char* str);
void concatena_totale(char* user, char* str, char* dest);
void ascii_key(char* str, int key);
struct dati input();



int main(void){
	printf("Content-type: text/html\n\n<body>");
	struct dati d;
	d=input();
	//printf("<br><br>%s<br><br>",d.total_encrypted);
	printf("<br><a href=\"/piero/QR.php?bin=%s\">Conferma</a></body></html>",d.da_inserire_in_QR);
	return 0;
}

struct dati input(){
	int i;
	struct dati d;
	int len;
	char* query;
	char user[8];
	char str1[141];
	char str2[141]; 
	char nick[8];
	char* p;
	 
	len = strlen(getenv("QUERY_STRING"));
	query=(char*)malloc((len*sizeof(char))+1);
	 
	if ((query=getenv("QUERY_STRING"))==NULL)  {
		printf ("<h3>Pagina richiamata in modo errato</h3>\n""</body></html>\n");
    	exit(1);
  	}
	query[len]='\0';

	p=(char*)strstr(query,"user=");
	p+=5;
	while(*(p)!='&' && *(p)!='\0'){
		user[i]=*(p);
		p++;
		i++;
	}
	
	i=0;
	p=NULL;
	p=(char*)strstr(query,"str1=");
	p+=5;
	
	while(*p!='&' && *p!='\0'){
		str1[i]=*p;
		p++;
		i++;
	}
	str1[i]='\0';
	
	i=0;
	p=NULL;
	p=(char*)strstr(query,"nick=");
	p+=5;
	while(*p!='&' && *p!='\0'){
		nick[i]=*p;
		p++;
		i++;
	}
	nick[i]='\0';
	
	i=0;
	p=NULL;
	p=(char*)strstr(query,"str2=");
	p+=5;
	
	while(*p!='&' && *p!='\0'){
		str2[i]=*p;
		p++;
		i++;
	}
	str2[i]='\0';
	
	printf("<b><h2>Conferma dati</h2></b><br>");
	printf("Username: %s<br>Nickname: %s<br>Stringa 1: %s<br>Stringa2: %s<br>",user,nick,str1,str2);

	strcpy(d.user,user);
	strcpy(d.str1,str1);
  	  
  	if(conta_caratteri(d.user)!=7 || conta_caratteri(d.str1)>140) {
  		printf("<b><h1>ERRORE! Meno di 7 caratteri per l'username</h1></b>'");
		exit(1); 
	}
  	
	
	ascii_key(d.user,USER_ASCII_KEY);
	completa_stringa(d.str1);
	ascii_key(d.str1,STR_ASCII_KEY);
	b64_encode(d.user, d.b64USER);
	b64_encode(d.str1, d.b64STR1);

	strcpy(d.binUSER,stringToBinary(d.b64USER));
	strcpy(d.binSTR,stringToBinary(d.b64STR1));

	cripta_user(d.binUSER);
	cripta_str(d.binSTR);
	concatena_totale(d.binUSER,d.binSTR,d.total_encrypted);
	strcpy(d.da_inserire_in_QR,d.total_encrypted);
	strcat(nick,"$");
	strcat(d.da_inserire_in_QR,nick);
	strcat(d.da_inserire_in_QR,str2);
	
	//printf("<br><br>%s",d.da_inserire_in_QR);
	
	return d;
}

int conta_caratteri(char* str){
	int i=0;
	while(str[i]!='\0'){
		i++;
	}
	return i;
}
void completa_stringa(char* str){
	int i;
	for(i=conta_caratteri(str);i<140;i++){
		str[i]= ' ' - STR_ASCII_KEY; //Questa istruzione fa sì che all'output non compaiano caratteri non stampabili come completamento della stringa
	}
	str[140]='\0';
	return;
}
void cripta_user(char* str){
	int left=0,right=95;
	char temp;
	for(left=0;left<48;left++){
		temp=str[left];
		str[left]=str[right];
		str[right]=temp;
		right--;
	}
}
void cripta_str(char* str){
	int i=0;
	char tmp;
	for(i=0;i<1504;i+=2){
		tmp=str[i];
		str[i]=str[i+1];
		str[i+1]=tmp;
	}
}
void concatena_totale(char* user, char* str, char* dest){
	int i,j=0;
	for(i=0;i<96;i++){
		dest[i]=user[i];
	}
	for(i=96;i<1601;i++){
		dest[i]=str[j];
		j++;
	}
	dest[1601]='\0';
}
void ascii_key(char* str, int key){
	int i=0;
	if(key>=0){ //Se key>0 vuol dire che sto criptando 
		while(str[i]!='\0'){
			str[i] = (str[i]+key)%128;
			i++;
		}
	}else{ //se key<0 sto decriptando
		while(str[i]!='\0'){
			if(str[i]!=32){ //Carattere di stringa completa, non va decrittato
				str[i] = (str[i]+key)%128;
				if(str[i]<0) str[i]=128+str[i];	
			}
			i++;
		}
		str[i]='\0';	
	}
}
char *stringToBinary(char *s){
  if (s == NULL) {
    // NULL might be 0 but you cannot be sure about it
    return NULL;
  }
  // get length of string without NUL
  size_t slen = strlen(s);

  // we cannot do that here, why?
  // if(slen == 0){ return s;}

  errno = 0;
  // allocate "slen" (number of characters in string without NUL)
  // times the number of bits in a "char" plus one byte for the NUL
  // at the end of the return value
  char *binary = malloc(slen * CHAR_BIT + 1);
  if(binary == NULL){
     fprintf(stderr,"malloc has failed in stringToBinary(%s): %s\n",s, strerror(errno));
     return NULL;
  }
  // finally we can put our shortcut from above here
  if (slen == 0) {
    *binary = '\0';
    return binary;
  }
  char *ptr;
  // keep an eye on the beginning
  char *start = binary;
  int i;

  // loop over the input-characters
  for (ptr = s; *ptr != '\0'; ptr++) {
    /* perform bitwise AND for every bit of the character */
    // loop over the input-character bits
    for (i = CHAR_BIT - 1; i >= 0; i--, binary++) {
      *binary = (*ptr & 1 << i) ? '1' : '0';
    }
  }
  // finalize return value
  *binary = '\0';
  // reset pointer to beginning
  binary = start;
  return binary;
}

void encodeblock( unsigned char in[], char b64str[], int len ) {
    unsigned char out[5];
    out[0] = b64[ in[0] >> 2 ];
    out[1] = b64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? b64[ ((in[1] & 0x0f) << 2) |
             ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? b64[ in[2] & 0x3f ] : '=');
    out[4] = '\0';
    strncat(b64str, out, sizeof(out));
}
void b64_encode(char *clrstr, char *b64dst) {
  unsigned char in[3];
  int i, len = 0;
  int j = 0;

  b64dst[0] = '\0';
  while(clrstr[j]) {
    len = 0;
    for(i=0; i<3; i++) {
     in[i] = (unsigned char) clrstr[j];
     if(clrstr[j]) {
        len++; j++;
      }
      else in[i] = 0;
    }
    if( len ) {
      encodeblock( in, b64dst, len );
    }
  }
}


