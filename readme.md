# Opdracht 4 Sockets

## Deel 1:

  Maak een server en een client.
  De client stuurt via een datagram een IO nummer en een toggle snelheid door.
  De server begint vervolgens de IO aan te sturen en stuurt bij iedere toggle het niveau van de IO naar de client.
  Datagram versie is volledige pakketen in een keer versturen. Werken met sendTo en ReceiveFrom in de plaats van read en write.

## Deel 2:

  Maak gebruik van sockets om een webserver te maken.
  Zorg dat op de webpagina met de toestand van enkele inputs en outputs wordt weergegeven.
  (niet dynamisch => refresh van pagina nodig om nieuwe toestand te zien)

  merk op dat index.html in de build folder staat. Dit komt doordat al er gebruik wordt gemaakt van Cmake de index.html file in dezelfde folder als
  de output file moet staan. ander krijg je segmentation fault.


