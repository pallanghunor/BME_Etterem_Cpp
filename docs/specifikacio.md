# Feladatspecifikáció: Nagy házi feladat - BME Étterem Kezelő

**Készítette:** Pallang Hunor  
**Tárgy:** Programozás alapjai 2.  
**Feladat:** Éttermi adminisztrációs rendszer  

---

## 1. A feladat általános leírása
A jelen **nagy házi feladat** célja egy étterem napi működését támogató digitális adminisztrációs rendszer kidolgozása konzolos felületen. A szoftver lehetővé teszi az étterem asztalainak, az étlap különböző típusú tételeinek és a vendégek rendeléseinek nyilvántartását. A program nem korlátozza a tárolható adatok mennyiségét, azokat a rendelkezésre álló memória erejéig dinamikusan kezeli.

## 2. Funkciók és elvárt működés (Felhasználói esetek)

### 2.1. Asztalkezelés
* **Felvétel**: A felhasználó rögzíthet új asztalt a férőhelyek számának (minimum 2), egy rövid szöveges leírásnak, valamint az étterem alaprajzán elfoglalt X és Y koordinátáknak a megadásával.
* **Módosítás**: Lehetőség van a férőhelyek, a leírás és a pozíció utólagos korrekciójára.
* **Törlés**: Egy asztal eltávolítható a rendszerből, feltéve, hogy nem foglalt és nem tartozik hozzá aktív rendelés.
* **Asztal lezárása (Számlázás)**: A fogyasztás végén a program összesíti az asztalhoz tartozó rendeléseket, kiírja a fizetendő végösszeget a képernyőre, és egy formázott számlát generál külső szöveges fájlba. Ezt követően az asztal automatikusan felszabadul.

### 2.2. Étlap (Menü) kezelése
A program két különböző típusú fogyasztási cikket tud nyilvántartani, eltérő tulajdonságokkal:
* **Ételek**: Név, egységár, elérhetőség (készleten van-e) és allergén információk.
* **Italok**: Név, egységár, elérhetőség, űrtartalom (pl. 0.5l) és alkoholtartalom jelzése.

A felhasználó ezeket a tételeket felveheti, adataikat módosíthatja, törölheti, illetve az egész étlapot kilistázhatja, ahol a program a típusnak megfelelő extra adatokat is megjeleníti.

### 2.3. Rendelések menedzselése
* **Felvétel**: Egy kiválasztott asztalhoz tételek rendelhetők az étlapról, darabszám megadásával. Ha az asztal korábban szabad volt, a státusza automatikusan "foglalt"-ra változik.
* **Módosítás**: A már felvett rendelések darabszáma módosítható. Ha egy tétel mennyisége 0-ra csökken, az adott elem kikerül a rendelésből.
* **Lekérdezés**: Asztalonként megtekinthető az eddigi fogyasztás részletes listája és a pillanatnyi részösszeg.

### 2.4. Foglaltsági térkép
A program karakteres vizuális felületen megjeleníti az étterem alaprajzát az asztalok koordinátái alapján. A térkép jelzi az asztalok azonosítóját és azok aktuális foglaltsági állapotát.

## 3. Bemenetek és kimenetek (Adatkezelés)
A program a futások közötti állapotmegőrzést egyszerű szöveges fájlok segítségével biztosítja. Induláskor beolvassa, kilépéskor felülírja őket. 

**Bemeneti és kimeneti adatfájlok formátuma:**
* **asztalok.txt**: ID;férőhely;leírás;X;Y;foglaltság
* **etelek.txt**: Típus(E/I);ID;név;ár;elérhetőség;[Extra: allergén VAGY űrtartalom;alkoholos]
* **orders.txt**: RendelésID;AsztalID;Végösszeg;TételekSzáma;[ÉtelID;Mennyiség]...

**Generált kimenet:**
* **Számlák**: Az asztal lezárásakor a szamlak/ mappába kerülnek generálásra szamla_YYYYMMDD_ID.txt néven. A fájl formázottan, ember számára olvashatóan tartalmazza az étterem adatait, a dátumot, a tételek listáját részösszegekkel, és a végösszeget.

## 4. Működési peremfeltételek és körülmények
* **Interaktivitás és Batch mód**: A program alapértelmezetten interaktív konzolos menürendszerrel kommunikál a standard bemeneten (stdin) és kimeneten (stdout). A menüvezérlés azonban kialakításából fakadóan támogatja a fájlból történő bemenet-átirányítást, így automatizált tesztfájlokkal is működtethető.
* **Bemenet-validálás**: A rendszer ellenőrzi a felhasználói adatokat. Rossz adattípus (pl. szám helyett betű) vagy érvénytelen logikai érték (pl. negatív ár, pályán kívüli koordináta) esetén a program nem omlik össze, hanem hibaüzenetet ad és újrakéri az adatot.
* **Ütközésvédelem**: Két asztal térben nem fedheti egymást, ugyanazokra az X-Y koordinátákra asztal nem rögzíthető.
* **Adatintegritás**: Ha a program indításakor a bemeneti adatfájlok nem léteznek, a program hiba nélkül, üres adatbázissal indul, és azokat a futás végén hozza létre.