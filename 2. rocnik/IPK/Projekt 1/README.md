# Klient pro OpenWeatherMap API
Řešení projektu slouží pouze k inspiraci.<br/>
Hodnocení projektu: 18/20

## Zadání
Úkolem je vytvořit program - klienta rozhraní OpenWeatherMap, který bude schopen prostřednictvím HTTP dotazů získávat vybrané informace z datového zdroje. Pro přístup je nutná bezplatná registrace a získání klíče, který se používá v dotazech pro autentizaci. Způsob používání OpenWatherMap API si nastudujte z on-line zdroje.

Klient se bude spouštět pomocí souboru `Makefile` následujícím způsobem:

```
make run api_key=<API klíč> city=<Město>
```

Výsledkem bude informace o aktuálním počasí a následující informace: popis počasí, teplota, vlhkost, tlak, rychlost a směr větru. Používejte metrické jednotky při uvádění všech hodnot.

Pro správnou komunikaci je nutné vytvořit korektní HTTP dotaz. Jestliže si nejste jisti podívejte se na správnou komunikaci jejím odchycením a zobrazením ve Wiresharku.

Příklad:

```
make run api_key=b498767252de12f92574d288a9c4fdc2 city=Prague
```

```
Prague
overcast clouds
temp:28.5°C
humidity:89%
preassure:1013 hPa
wind-speed: 7.31km/h
wind-deg: 187
```

Pro implementaci klienta je nutné využít knihovny soketů. Není přípustné využívat knihovny pro zpracování HTTP a podobně - cílem je lightweight klient, který má minimum závislostí. 

Odevzdáváte zdrojové soubory projektu - projekt může být napsán v jazyce C, Python nebo Java.
