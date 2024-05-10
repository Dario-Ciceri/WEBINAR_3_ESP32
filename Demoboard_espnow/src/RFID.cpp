/*
   Sensore RFID/NFC
   Futura Academy: https://academy.futuranet.it/
   Webinar 2: Esp32 Futura Demoboard
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#include "Arduino.h"
#include "RFID.h"
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

void RFID::init() {
  Serial.println("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");
  Serial.println("Inizializzazione RFID!");
  nfc.begin();
}

void RFID::formatToMifareClassic() {
  Serial.println();
  Serial.println("Posiziona un tag sopra il sensore per formattarlo da NDEF a Mifare Classic:");
  if (nfc.tagPresent()) {
    bool success = nfc.clean();
    if (success) {
      Serial.println("Successo! Tag riportato alle impostazioni di fabbrica!");
    } else {
      Serial.println("Errore! Impossibile formattare il tag!");
    }
  } else {
    Serial.println("Tag non trovato!!! Annullo l'operazione!");
  }
}

void RFID::erase() {
  Serial.println();
  Serial.println("Posiziona un tag sopra il sensore per cancellare i suoi dati:");
  if (nfc.tagPresent()) {
    bool success = nfc.erase();
    if (success) {
      Serial.println("Successo! Ora il tag contiene un messaggio vuoto!");
    } else {
      Serial.println("Errore! Impossibile cancellare i dati dek tag!");
    }
  } else {
    Serial.println("Tag non trovato!!! Annullo l'operazione!");
  }
}

void RFID::formatToNDEF() {
  Serial.println();
  Serial.println("Posiziona un tag sopra il sensore per formattarlo da Mifare Classic a NDEF:");
  if (nfc.tagPresent()) {
    bool success = nfc.format();
    if (success) {
      Serial.println("Successo! Tag formattato come NDEF");
    } else {
      Serial.println("Errore! Impossibile formattare il tag!");
    }
  } else {
    Serial.println("Tag non trovato!!! Annullo l'operazione!");
  }
}

void RFID::read() {
  Serial.println();
  Serial.println("Posiziona un tag sopra il sensore per leggerlo:");
  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    tag.print();
  } else {
    Serial.println("Tag non trovato!!! Annullo l'operazione!");
  }
}

String RFID::extendedRead() {
  Serial.println();
  Serial.println("Posiziona un tag sopra il sensore per leggerlo:");

  String result = "";

  if (nfc.tagPresent(5))
  {
    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: "); Serial.println(tag.getUidString());

    if (tag.hasNdefMessage()) // every tag won't have a message
    {

      NdefMessage message = tag.getNdefMessage();
      Serial.print("Questo tag contiene un messaggio NDEF: ");
      Serial.print(message.getRecordCount());
      Serial.print(" NDEF Record");
      if (message.getRecordCount() != 1) {
        Serial.print("s");
      }
      Serial.println(".");

      // cycle through the records, printing some info from each
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        Serial.print("\nNDEF Record "); Serial.println(i + 1);
        NdefRecord record = message.getRecord(i);
        // NdefRecord record = message[i]; // alternate syntax

        Serial.print("  TNF: "); Serial.println(record.getTnf());
        Serial.print("  Type: "); Serial.println(record.getType()); // will be "" for TNF_EMPTY

        // The TNF and Type should be used to determine how your application processes the payload
        // There's no generic processing for the payload, it's returned as a byte[]
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);

        // Print the Hex and Printable Characters
        Serial.print("  Payload (HEX): ");
        PrintHexChar(payload, payloadLength);

        // Force the data into a String (might work depending on the content)
        // Real code should use smarter processing
        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        Serial.print("  Payload (Stringa di testo): ");
        Serial.println(payloadAsString);
        result = payloadAsString;

        // id is probably blank and will return ""
        String uid = record.getId();
        if (uid != "") {
          Serial.print("  ID: "); Serial.println(uid);
        }
      }
    }
  } else {
    Serial.println("Tag non trovato!!! Annullo l'operazione!");
  }
  return result;
}

void RFID::write() {
  Serial.println();
  Serial.println("Posiziona un tag sopra il sensore per scriverci dei dati:");
  if (nfc.tagPresent())
  {
    NdefMessage message = NdefMessage();
    message.addUriRecord("https://www.instagram.com/_dario.ciceri_/");

    bool success = nfc.write(message);
    if (success) {
      Serial.println("Successo! Prova a leggere questo tag con il tuo cellulare (basta avvicinarlo avendo l'NFC accesso!)");
    } else {
      Serial.println("Errore in scrittura!");
    }
  } else {
    Serial.println("Tag non trovato!!! Annullo l'operazione!");
  }
}

void RFID::writeMultipleRecords() {
  Serial.println();
  Serial.println("Posiziona un tag sopra il sensore per scriverci molteplici record di dati:");
  if (nfc.tagPresent())
  {
    NdefMessage message = NdefMessage();
    message.addTextRecord("profilo instagram:");
    message.addUriRecord("https://www.instagram.com/_dario.ciceri_/");
    message.addTextRecord("canale YouTube:");
    message.addUriRecord("https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA");

    boolean success = nfc.write(message);
    if (success) {
      Serial.println("Successo! Prova a leggere questo tag con il tuo cellulare (basta avvicinarlo avendo l'NFC accesso!)");
    } else {
      Serial.println("Errore in scrittura!");
    }
  } else {
    Serial.println("Tag non trovato!!! Annullo l'operazione!");
  }
}
