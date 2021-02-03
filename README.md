## Specifiche
- Ciao [nome], allora potresti fare questa demo: 
l'Utente in registrazione imputa come Username 7 caratteri (fissi) alfanumerici e imputa altri 7 caratteri (massimo) alfanumerici come Nickname (il sistema controlla che non esistano altri Nickname uguali ed in caso ne richiede la modifica) 
L'Utente inserisce poi 2 stringhe di testo di max 140 caratteri.
A fronte di tale registrazione il sistema converte (utilizzando un algoritmo reversibile) sia lo Username che la prima stringa di testo in un codice alfanumerico reversibile. 
L'argoritmo verrà automaticamente modificato nel momento in cui un altro utente dovesse inserire uno Username già utilizzato da altro utente.
Sia i dati "criptati/convertiti" che quelli in chiaro vengono tramutati in un QRcode che viene esposto a video su device mobile assieme ad una stringa alfanumerica univoca di 7 caratteri (la schermata conterrà quindi il QRcode +la stringa univoca. Nel QRcode viene inoltre inserita la data e l'ora di generazione del QRcode medesimo.
Successicamente un apparato di lettura del QRcode deve poter convertire in chiaro il contenuto crittografato del QRcode: ció può avvenire tramite APP in cui siano presenti i relativi algoritmi di riconversione, oppure via web attraverso interazione con nostro server.
Anche la data di generazione del QRcode va criptata..
- Il programma deve essere compilabile (non interpretato) e deve essere raggiunto tramite interfaccia web.

## Utilizzo
Il committente non ha dichiarato il fine di questo progetto.

## Sviluppo
Si è dunque deciso di utilizzare il linguaggio C come cuore dell'applicativo web (in Common Gateway Interface). 

Essendo una demo se ne sconsiglia l'utilizzo in quanto l'algoritmo non è stato testato a dovere e l'output (criptato) contiene un pattern ripetuto. Sarebbe dovuto poi essere aggiornato utilizzando AES. Inoltre per lo stesso motivo non tutte le specifiche sono soddisfatte. 

Reso pubblico perché (presumibilmente) abbandonato dal 2017.
