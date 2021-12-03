# Dokumentation/Zusammenfassung von Kommunikation mit MCU

## SPI RFC-MCU Übersicht:
- MCU Master
- RFC Slave
- In welchen Intervallen sendet die MCU am Anfang? Wie wird anfängliche Frequenz 
- Am Anfang Frequenzen für Module senden

## Datenpakete:
### Standard:
1. LEN
2. NP (4 bit) + Protokoll (3 bit) + ADC-Flag (1 bit)
3. PS (4 bit) + ComEn (4 bit)
4. Data0
   ...
   DataN
   (CRC)
        
### Sonderpakete:
1 Null
2 LEN
3 Metainfo (ASCII Char.) (1)
4 String (ASCII String)
  ...
  (CRC)
        
        
(1) - R = Restart (kein String)
    - E = Error (String optional)
    - I = Intervallmessung setzen (String: Tabellenplatz | Periode (1-255 bzw. 100ms - 25,5s) | String (Standardpaket))
    - S = Start (kein String)
       
      
