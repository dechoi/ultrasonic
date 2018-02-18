# ultrasonic
C Program sensor Ultrasonic HCSR-04 dengan AVR Microcontroller ATMEGA32
Pengukur jarak Digital menggunakan sensor ultrasonic  HC-SR04 dan AVR Microcontroller
Dalam tutorial ini kita akan membahas dan merancang rangkaian untuk mengukur jarak. Sirkuit ini dikembangkan dengan menghubungkan sensor ultrasonik "HC-SR04" dengan mikrokontroler AVR ATMEGA32 . Sensor ini menggunakan teknik yang disebut "ECHO" yang merupakan sesuatu yang Anda dapatkan saat suara dipantulkan kembali setelah dipantulkan dari permukaan.
Kita tahu getaran suara tidak bisa menembus benda padat / solid. Jadi apa yang terjadi adalah, ketika sebuah sumber suara menghasilkan getaran yang sinyal suara tempuh melalui udara pada kecepatan 220 meter per detik. Getaran ini saat sinyal suara bertemu telinga kita, kita menggambarkannya sebagai suara. Seperti yang dikatakan tadi getaran ini tidak bisa menembus solid, jadi saat sinyal suara memancar dengan permukaan seperti dinding, sinyal suara dipantulkan kembali pada kecepatan yang sama dengan sumbernya, yang disebut ECHO.

Seperti yang ditunjukkan pada gambar, pada awalnya kita perlu melakukan sensor untuk mengukur jarak, yaitu sinyal logika HIGH / TINGGI pada pin pemicu sensor lebih dari 10uS, setelah itu getaran suara dikirim oleh sensor, setelah gema, sensor memberikan sebuah sinyal pada pin output yang proporsinya sebanding dengan jarak antara sumber dan rintangan.
Rumus jarak ( FISIKA ) adalah S = 340 .t / s
Jarak ini dihitung sebagai, jarak (dalam cm) = lebar output pulsa (dalam uS) / 58.
Di sini lebar sinyal harus diambil dalam kelipatan uS (mikro detik atau 10 ^ -6).
Komponen yang dibutuhkan
Perangkat keras: ATMEGA32, Power supply (12v), AVR-ISP PROGRAMMER, DISPLAY LCD (16x2LCD), kapasitor 100uF, resistor 10KΩ (2 buah), sensor HC-SR04.
Perangkat lunak: AVR GCC,Notepad Programmer, dan Progis.  Disini kita menggunakan PORTC untuk terhubung ke port data LCD (D0-D7). Siapa pun yang tidak ingin bekerja dengan FUSE BITS dari ATMEGA32A tidak dapat menggunakan PORTC, karena PORTC berisi jenis komunikasi khusus yang hanya dapat dinonaktifkan dengan mengubah FUSEBITS. ( dalam ujicoba ini kita abaikan saja paragraf ini, namun perlu dipelajari cara setting Fuse Bit )
Pada rangkaian, Anda amati hanya digunakan dua pin kontrol, ini memberi fleksibilitas pemahaman yang lebih baik. Bit kontras dan READ / WRITE tidak sering digunakan sehingga bisa disingkat menjadi ground. Ini menempatkan LCD dalam mode kontras dan baca tertinggi. Kita hanya perlu mengendalikan pin ENABLE dan RS untuk mengirim karakter dan data yang sesuai.
PERHATIKAN GAMBAR untuk Sensor Ultrasonic PIN TRIGGER pada PD0  dan PIN ECHO pada PD2

Sambungan yang dilakukan untuk LCD diberikan di bawah ini:
•	PIN1 atau VSS ke ground
•	PIN2 atau Vdd atau VCC ke 5 v
•	PIN3 atau VEE ke ground (memberikan kontras maksimal)
•	PIN4 atau RS (Register Selection) ke PD6 dari UC
•	PIN5 atau RW (Baca / Tulis) ke ground (letakkan LCD dalam mode baca)
•	PIN6 atau E (Enable) ke PD5 dari UC
•	PIN7 atau D0 sampai PC0 dari UC
•	PIN8 atau D1 ke PC1 dari UC
•	PIN9 atau D2 ke PC2 dari UC
•	PIN10 atau D3 ke PC3 dari UC
•	PIN11 atau D4 sampai PC4 dari UC
•	PIN12 atau D5 sampai PC5 dari UC
•	PIN13 atau D6 sampai PC6 dari UC
•	PIN14 atau D7 sampai PC7 dari UC
Di sirkuit Anda dapat melihat bahwa kita telah menggunakan komunikasi 8bit (D0-D7) namun ini bukan keharusan dan kita dapat menggunakan komunikasi 4bit (D4-D7) namun dengan program komunikasi 4 bit menjadi agak rumit. Jadi seperti yang ditunjukkan pada tabel di atas kita menghubungkan 10 pin LCD ke controller dimana 8 pin adalah pin data dan 2 pin untuk kontrol.
 Sensor ultrasonik adalah perangkat empat pin, PIN1- VCC atau + 5V; PIN2-TRIGGER; PIN3- ECHO; PIN4- GROUND. Trigger pin adalah tempat kita memberi trigger untuk memberi tahu sensor untuk mengukur jarak. Echo adalah pin output dimana kita mendapatkan jarak dalam bentuk lebar pulsa. Pin gema disini terhubung ke controller sebagai sumber interupsi eksternal. Jadi untuk mendapatkan lebar keluaran sinyal, pin gema sensor terhubung ke INT0 (interrupt 0) atau PD2.

Tiga register di atas harus diatur agar pengaturannya berjalan dan kita akan membahasnya secara singkat,
BLUE (INT0): bit ini harus disetel tinggi untuk mengaktifkan interrupt0 eksternal, setelah pin ini diset, kita bisa merasakan perubahan logika pada pin PIND2.
BROWN (ISC00, ISC01): kedua bit ini disesuaikan dengan perubahan logika yang sesuai pada PD2, yang dianggap sebagai interrupt.
 
Jadi seperti yang dikatakan sebelumnya kita perlu interupsi untuk memulai penghitungan dan untuk menghentikannya. Jadi kita tetapkan ISC00 sebagai satu dan kita mendapatkan interupsi bila ada logika LOW/ RENDAH ke HIGH di INT0;  Interupsi lain bila ada logika HIGH to LOW.
RED (CS10): Bit ini hanya untuk mengaktifkan dan menonaktifkan counter. Meski bekerja sama dengan bit CS10 lainnya, CS12. Kami tidak melakukan prescaling apapun di sini, jadi kami tidak perlu khawatir tentang sinyal suara.
Beberapa hal penting yang perlu diingat disini adalah:
Kami menggunakan jam internal ATMEGA32A yang 1MHz. Tidak ada prescaling disini, kita tidak melakukan compare match interrupt menghasilkan rutinitas, jadi tidak ada setting register yang rumit.
Nilai penghitungan setelah penghitungan disimpan dalam register TCNT1 16bit,
Paham ...? mari kita mulai Coding...

Kami menggunakan AVRGCC untuk kopilasi kode C, jika menggunakan OS Windows 7, win 8, win 10 tambahkan library sysmin.dll pada folder UTIL dalam aplikasi AVRGCC, karena akan ditemui ERROR jika tidak disertakan,

Ayoo berkreasi...
