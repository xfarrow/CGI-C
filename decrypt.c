#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define USER_ASCII_KEY 4
#define STR_ASCII_KEY 9

char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";



int conta_caratteri(char* str);
void decodeblock(unsigned char in[], char *clrstr);
void b64_decode(char *b64src, char *clrdst);
void completa_stringa(char* str);
void cripta_user(char* str);
void cripta_str(char* str);
void concatena_totale(char* user, char* str, char* dest);
void ascii_key(char* str, int key);
void decrypt(void);
void unencode(char *src, char *dest, int len);
char* get_field(char *query, char *field);

int main(void){
	printf("Content-type: text/html\n\n");
	printf("<html><body>");
	decrypt();
	printf("</body></html>");
	return 0;
}

void decrypt(void){
	int i,j=96,k=0,b64_clear_user_char,b64_clear_str_char;
	char utente_bin[97];
	char str_bin[1505];
	char b64USER[13];
	char b64STR1[189];
	int buffer[8];
	char user[8];
	char str1[141];
	char clear_string[148];
	char nick[8];
	char str2[141];
	
	int len;
	char* query;
	char* str_bin_received; //stringa di bit ricevuta da GET
	
	if ((query=getenv("QUERY_STRING"))==NULL)  {
    printf ("<h3>Pagina richiamata in modo errato</h3>\n"
            "</body></html>\n");
    exit(1);
  }
  	len=strlen(query);
	str_bin_received = (char*)malloc((len*sizeof(char))+1);
	str_bin_received[len]='\0';
	str_bin_received=get_field(query,"bin");
	
	for(i=0;i<96;i++){
		utente_bin[i]=str_bin_received[i];
	}
	utente_bin[96]='\0';
	
	for(i=0;i<1504;i++){
		str_bin[i]=str_bin_received[j];
		j++;
	}
	str_bin[1504]='\0';
	
	i=0;
	while(str_bin_received[j]!='\0'){
		clear_string[i]=str_bin_received[j];
		j++;
		i++;
	}
	clear_string[i]='\0';
	i=0;
	k=0;
	
	while(clear_string[i]!='$'){
		nick[i]=clear_string[i];
		i++;
	}
	nick[i]='\0';
	i++;
	while(clear_string[i]!='\0'){
		str2[k]=clear_string[i];
		k++;
		i++;
	}
	str2[k]='\0';
	k=0;
	cripta_user(utente_bin); //cripta e decripta
	cripta_str(str_bin); //cripta e decripta


	// Trasforma sequenza bit in stringa USER b64
	for(i=0;i<96;i+=0){
		j=0;
		for(j=0;j<=7;j++){
			buffer[j]=(utente_bin[i])-48; //faccio -48 in modo che "0" (numero 48 in dec ascii) valga 0 in dec ascii 
			i++;
		}
		b64_clear_user_char = (buffer[0]*128) + (buffer[1]*64) + (buffer[2]*32) + (buffer[3]*16) + (buffer[4]*8) + (buffer[5]*4) + (buffer[6]*2) + (buffer[7]);
		b64USER[k] = (char)b64_clear_user_char;
		k++;
	}
	b64USER[k]='\0';
	k=0;

	// Trasforma sequenza bit in stringa STR b64
	for(i=0;i<1504;i+=0){
		for(j=0;j<=7;j++){
			buffer[j]=(str_bin[i])-48;
			i++;
		}
		b64_clear_str_char = (buffer[0]*128) + (buffer[1]*64) + (buffer[2]*32) + (buffer[3]*16) + (buffer[4]*8) + (buffer[5]*4) + (buffer[6]*2) + (buffer[7]);
		b64STR1[k] = (char)b64_clear_str_char;
		k++;
	}
	b64STR1[k]='\0';
	
	b64_decode(b64USER,user);
	b64_decode(b64STR1,str1);
	
	ascii_key(user,-1*USER_ASCII_KEY);
	ascii_key(str1,-1*STR_ASCII_KEY);
	printf("<h2><b>Username decrittografato: %s<br>Nick: %s<br>Stringa 1 decrittografata: %s<br>Stringa 2: %s<br></b></h2>",user,nick,str1,str2);
	
	
	return;
}

int conta_caratteri(char* str){
	int i=0;
	while(str[i]!='\0'){
		i++;
	}
	return i;
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
	}
}


void decodeblock(unsigned char in[], char *clrstr) {
  unsigned char out[4];
  out[0] = in[0] << 2 | in[1] >> 4;
  out[1] = in[1] << 4 | in[2] >> 2;
  out[2] = in[2] << 6 | in[3] >> 0;
  out[3] = '\0';
  strncat(clrstr, out, sizeof(out));
}
void b64_decode(char *b64src, char *clrdst) {
  int c, phase, i;
  unsigned char in[4];
  char *p;

  clrdst[0] = '\0';
  phase = 0; i=0;
  while(b64src[i]) {
    c = (int) b64src[i];
    if(c == '=') {
      decodeblock(in, clrdst); 
      break;
    }
    p = strchr(b64, c);
    if(p) {
      in[phase] = p - b64;
      phase = (phase + 1) % 4;
      if(phase == 0) {
        decodeblock(in, clrdst);
        in[0]=in[1]=in[2]=in[3]=0;
      }
    }
    i++;
  }
}
char* get_field(char *query, char *field)  {
     int i,j,len,pos;
     char *tmp,*input;
 
     // len è pari alla lunghezza della querry+1
     len = strlen(query)+1;
 
     /* tmp sarà il pattern di ricerca all'interno della query
        Nel nostro caso andrà a contenere la stringa 'nome=' */
     tmp   = (char*) malloc( (strlen(field)+1)*sizeof(char) );
 
     /* input è lunga quanto la query, e andrà a contenere
        il campo da noi ricercato */
     input = (char*) malloc(len*sizeof(char));
 
     // tmp <- nome=pippo
     sprintf (tmp, "%s=", field);
 
     // Se all'interno della query non c'è il campo richiesto, esco
     if (strstr(query,tmp)==NULL)
          return NULL;
 
     /* Cerco la posizione all'interno della query
        in cui è stato trovato il campo nome */
     pos = ( (int) strstr(query,tmp) - (int) query) + (strlen(field)+1);
 
     /* Controllo quanto è lungo il pattern nome=blablabla
        Questo   ciclo   termina   quando   viene   incontrato   un   '&' all'interno
        della query (ovvero quando comincia un nuovo campo) o quando la stringa è 
        terminata 
        Alla fine i conterrà il numero di caratteri totali nel pattern di ricerca */
     for (i=pos; ; i++)  {
          if (query[i]=='\0' || query[i]=='&') break;
     }
 
     // Salvo il contenuto della query che mi interessa in input
     for (j=pos; j<i; j++)
          input[j-pos]=query[j];
 
     //'unencodo' input,  rendendo  eventuali caratteri speciali umanamente leggibili
     unencode(input,input,len);
 
     // Ritorno input
     return input;
} 
void unencode(char *src, char *dest, int len)  {
     int i,code;
 
     // Ciclo finché non ho letto tutti i caratteri specificati
     for (i=0; i<len; i++, src++, dest++)  {
          // Se il carattere corrente di src è un '+', lo converto in uno spazio ' '
          if (*src=='+')  *dest=' ';
 
          // Se il carattere corrente è un '%'
          else if (*src=='%')  {
            /* Se il carattere successivo non è un carattere valido,
               il carattere di destinazione sarà un '?',
               altrimenti sarà il carattere ASCII corrispondente */
               if (sscanf(src+1, "%2x", &code) != 1) code='?';
               *dest = (char) code;
 
               // Leggo il prossimo carattere
               src += 2;
          }
 
       /* Se è un carattere alfanumerico standard e non un carattere speciale,
          allora il carattere di destinazione è uguale a quello sorgente */
          else  *dest=*src;
     }
 
     // Termino la stringa di destinazione
     dest[len]='\0';
}
