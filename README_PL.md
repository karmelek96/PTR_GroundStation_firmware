# PTR Groundstation
Oprogramowanie do płytek ESP32 / LoRa / OLED z rodziny TTGO, które zmienia je w stacje odbiorcze do elektroniki budowanej przez [PTR-Projects](https://github.com/PTR-projects).
## Instalacja i pierwsze uruchomienie
1. Pobierz i rozpakuj najnowszą binarkę dla twojej płytki z sekcji [releases](https://github.com/PTR-projects/PTR_GroundStation_firmware/releases/latest).
2. Wgraj binarkę na płytkę, z adresem 0. [Możesz użyć tego narzędzia online](https://espressif.github.io/esptool-js/) które działa na większości przeglądarek.
Żeby to zrobić, podłącz płytkę do komputera, włącz ją, i wybierz z listy portów COM. Wybierz pobrany plik .bin, ustaw `flash address offset` na `0x0`, i kliknij `program`.
Uwaga: Jeśli masz z tym problemy, spróbuj użyć innej przeglądarki.
![image](https://github.com/user-attachments/assets/0a8ee731-6ea4-45b8-bbbe-b01a5d15f8f0)

   
To wszystko. Jeśli wyśwetlacz działa ale wydaje się zepsuty, wykonaj następne kroki.

3. Połącz się z siecią płytki. Powinna zaczynać się od `PTR-GS`. Jeśli łączysz się z telefonu, wyłącz dane komórkowe.
4. W przeglądarce otwórz adres `192.168.4.1`
5. Zjedź do `Change OLED driver`, zmień, i kliknij `Change driver`.

Wyświetlacz powinien byc naprawiony, i płytka jest gotowa do odbioru telemetrii.

## Użytkowanie
### Wyświetlacz
Na wyświetlaczu OLED wyświetlane są najważniejsze dane.
![image](https://github.com/user-attachments/assets/29279de8-3483-4a6a-8c66-add6b64d519e)
Uwaga: Żeby moć używać funkcji odczytu odległości i azymutu, musisz mieć płytkę wyposażoną w GPS.

Uwaga: W przypadku utraty sygnału od wybranego ID, wskaźnik częstotliwości odbierania ramek zamieni się na licznik sekund od ostatniej odebranej ramki.
![image](https://github.com/user-attachments/assets/2a844e23-0c5e-46e3-8cec-db363d5423bc)
### ID urządzenia
ID umożliwia na rozróżnienie urządzeń pracujących na tej samej częstotliwości. Dzięki temu stacja może odbierać wiele urządzeń jednocześnie (na przykład w rakiecie wielostopniowej, lub na imprezie gdzie mamy dużo startów do monitorowania).
Zmiana ID w ustawieniach pozwala na wybór urządzenia które chcesz odbierać, na zasadzie filtra. Jeśli chcesz odbierać wszystkie urządzenia na wybranej częstotliwości, ustaw ID na `0`.
### WebGUI
WebGUI pozwala na zmianę ustaweń urządzenia. Żeby otworzuć WebGUI, połącz isę z siecią wifi stacji naziemnej. Zaczyna się od `PTR-GS`, ale pełna nazwa wyświetlana jest na ekranie podczas uruchamiania.
Następnie, w przeglądarce otwórz adres `192.168.4.1`. Jeśli łączysz się z telefonu, wyłącz dane komórkowe.

Co możemy robić w WebGUI:
- Zmieniać ustawienia stacji naziemnej, stroić radio, zmieniać filtr ID.
- Widzieć wszystkie odbierane urządzenia pogrupowane po ID.
- Monitorować telemetrię konkretnego urządzenia, i otworzyć jego lokalizację w Google Maps, dla łatwego odnalezienia.
- Pobrać surowe logi w pliku CSV, które zawierają dodatkowe informacje jak apogeum czy ilosć satelitów GPS.
![image](https://github.com/user-attachments/assets/533af8c4-557c-4127-bcf6-4d2ef12ad76f)
