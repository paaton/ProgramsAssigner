# Program Assig
- Program pro přiřazení programů jednotlivým uživatelům dle kapacit jednotlivých programů. Zároveň umí pracovat s programovými bloky, aby se přiřazené programy nepřekrývaly.
- pozor na pořadí známek, ve formuláři programů počítáme s tím, že 1 je nejlepší, do tohoto programu jde ale transposed input, tj 5<->1; 2<->4; 3<->3
### Co je funkční
- constraint pro nepřekrývání se jednotlivých přiřazených programů  - každý user může mít přiřazen pouze jeden program v daný programový blok
- respektuje kapacitu každého jednoho programu
- celkové funguje optimalizace dle maximalizace průměru přiřazených programů
- importuje csv programů a uživatelů
- exportuje pro každý programový blok vlastní seznam uživatelů s přiřazenými programy
### Co chybí
- "degradace odpovědi 1", resp. upscaling/mapping odpovědí tak, aby se co nejméně stávalo, že někdo dostane program, kde měl známku "1"
### nice to have věci
- transpose, nebo spíše standardizace vstupních dat, aby skrz účastníka vždy byl průměr blízký 3.
  - některé odpovědi neopustí interval 3-5, jiní uživatelé zase nemají ani jednou 5 - zkusím to trochu srovnat do latě, uvidíme jak to bude fungovat.

