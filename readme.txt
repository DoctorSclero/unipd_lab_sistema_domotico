SCELTE PROGETTUALI:

Abbiamo deciso di mantenere un solo vector ordinato per la completa gestione della logica dei dispositivi:
Non abbiamo utilizzato lo std::stack e la std::priority_queue perche' non consente l'accesso casuale, 
questo non avrebbe permesso di implementare l'arresto/avvio da linea di comando dei dispositivi.
Abbiamo indagato la possibilita' di utilizzare la std::map (ordinata di default) perche' il funzionamento da noi
richiesto non sarebbe stato ideale per la struttura della mappa ordinata.

Per emulare la funzionalita' dello spegnimento in ordine di accensione dei dispositivi quando 
avviene un esubero ci siamo basati sull'utilizzo di una priorita' gestita da home stessa, ma presente in ogni singolo
dispositivo: infatti ogni qualvolta un dispositivo si accende richiede a home la priorita' che deve avere, e ogni qualvolta
un dispositivo si spegne, la home si occupa di rilasciare tale priorita' e decrementarla per tutti gli altri dispositivi
ancora accesi. 

L'unica eccezione fatta a queta logica avviene da parte dei dispositivi che si vuole restino accesi in fase di esubero
(es. "Impianto fotovoltaico" e "Frigorifero") detti "keep on"; questi infatti hanno priorita' costante -1 e vengono spenti in fase 
di esubero solo se tutti gli altri dispositivi che non hanno questa priorita' sono gia' spenti, 
ovviamente "Impianto fotovoltaico" non viene mai spento automaticamente perche' genera potenza.
La funzionalita' "keep on" e' impostata tramite un parametro nel costruttore dei dispositivi.

La priorita' era originariamente gestita dai soli device tramite una variabile membro statica. Questo pero' 
non permetteva l'attuale funzionamento di ritaratura della priorita' in base all'accensione/spegnimento. Infatti questa
non decrementava mai e avrebbe posto una limitazione nel massimo numero di accensioni disponibili nell'arco della giornata

Per fare comunicare i dispositivi e la home e' stato implementato il design pattern: Observer. Questo ci ha semplificato 
la gestione del logging e la gestione della priorita'.
Inoltre a causa di questa scelta il parametro home_ in Device non viene impostato in fase di creazione del dispositivo stesso, 
e' necessario quindi che home imposti questo parametro tramite la funzione Device::subscribe(), altrimenti l'eccezione 
"device_not_subscribed" verra' lanciata nelle funzioni della gerarchia Device dove questo parametro e' necessario per il funzionamento.

La funzione Device::stop() durante un esubero avrebbe creato dei loop all'interno di Home::update(),
e' stato necessario quindi creare la funzione Device::force_stop() per evitarlo.

Sono state create una serie di eccezioni personalizzate per andare a rappresentare al meglio i problemi riscontrati in fase di 
runtime, questo aumenta anche la semantica del codice.

La classe Logger e' stata incorporata all'interno di Home per evitare dei problemi di inclusione ricorsiva degli header:
infatti quando abbiamo utilizzato un file separato per l'header di Logger abbiamo riscontrato questo problema 
nonostante fossero state implementate le include guards; questo potrebbe essere dovuto ad un limite del compilatore.