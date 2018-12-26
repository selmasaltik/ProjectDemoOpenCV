// ProjectDemoOpenCV.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp> 
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>

using namespace cv;
using namespace std;

// Farklı fonksiyonlarda kullanmak üzere global değişkenlerimizi belirliyoruz:
int mod; // Yapacağımız farklı işlemler arasında dolaşırken hangi işlemde olduğumuzu tutacağız.
int xx1, xx2, yy1, yy2; // Fare imleci ile seçtiğimiz alanın koordinatlarını tutacağız.
bool cizim; // Fare imleciyle çizim yapılıp yapılmayacağını tutacağız.
ofstream pozitif_txt, negatif_txt; // pozitif.txt ve negatif.txt dosyalarını belirteceğiz.

// main metodunun altında tanımlayacağımız metotları önce burada belirtiyoruz.
// Çünkü C++ dilinde, bir metot kullanıldığında, üst satırlarda o metot tanımlanmamışsa hata verir.
void TurkceKarakter(); // Konsol ekranında Türkçe karakterleri kullanabilmek için metot.
void ModYonlendir(); // Farklı işlemlerimiz (ben mod diyorum) arasında geçiş yapacak metot.
void AnaMenu(); // Konsol ekranında ana menü görünümünü oluşturacak metot.
void Mod1(); // Sadece kamera görüntüsünü alıp göstereceğimiz basit bir mod olacak.
void Mod2(); // Pozitif örneği nasıl oluşturmak istediğimizi soran bir alt menü göstereceğiz.
void Mod21(); // Fare imleci yardımıyla arayacağımız objeyi işaretleyebileceğiz ve kaydedeceğiz.
void Mod22(); // Yüz bulma amacımız varsa, görüntüde yüz göründüğü her kareyi kaydedebileceğiz.
void Mod3(); // Negatif örnekler oluşturmak için kullanacağız.
void Mod4(); // Pozitif görüntülerden bir *.vec dosyası oluşturacağız.
void Mod5(); // Oluşturduğumuz vec dosyasındaki görüntüleri tek tek kontrol edebileceğiz.
void Mod6(); // Her şey hazırsa artık eğitimi gerçekleştireceğiz.
void Mod7(); //Eğittiğimiz Cascade sınıflandırıcımızı test edebileceğiz.

int main()
{
	if (mod == 0) AnaMenu();
	else if (mod == 1) Mod1();  // Kamera modu
	else if (mod == 2) Mod2();  // Pozitif örnek oluşturma menüsü
	else if (mod == 21) Mod21(); //  > İmleç yardımıyla işaretleyerek
	else if (mod == 22) Mod22(); //  > Otomatik yüz tespiti yardımıyla
	else if (mod == 3) Mod3();  // Negatif örnek oluşturma modu
	else if (mod == 4) Mod4();  // Vec dosyası oluşturma modu
	else if (mod == 5) Mod5();  // Vec dosyası kontrolü modu
	else if (mod == 6) Mod6();  // CascadeClassifier eğitim modu
	else if (mod == 7) Mod7();  // CascadeClassifier test modu
	return 0;
}


void TurkceKarakter() { setlocale(LC_ALL, "Turkish"); }
void ModYonlendir() {  }
void AnaMenu() { 
	system("CLS");
	cout << "                                                    " << endl;
	cout << "   OpenCV Görüntü İşleme - Duygu Analizi Yazılımı   " << endl;
	cout << "   *-----*------------------------------------*     " << endl;
	cout << "   |  1  | Kamera modu                        |     " << endl;
	cout << "   |  2  | Pozitif örnek oluşturma modu       |     " << endl;
	cout << "   |  3  | Negatif örnek oluşturma modu       |     " << endl;
	cout << "   |  4  | VEC dosyası oluşturma modu         |     " << endl;
	cout << "   |  5  | VEC dosyası kontrolü modu          |     " << endl;
	cout << "   |  6  | CascadeClassifier eğitim modu      |     " << endl;
	cout << "   *-----*------------------------------------*     " << endl;
	cout << "   |  0  | Çıkış                              |     " << endl;
	cout << "   *-----*------------------------------------*     " << endl;
	cout << "   Çalıştırmak istediğiniz mod numarası: ";
	cin >> mod;
	// Ana menüdeyken mod 0 yapılırsa program kapatılır:
	if (mod != 0) ModYonlendir();
}
void Mod1() { 
	system("CLS");
	cout << "   " << endl;
	cout << "   << KAMERA MODU ETKİNLEŞTİRİLDİ >>" << endl;
	cout << "   Ana menüye dönmek için \"Kamera\" penceresindeyken ESC tuşuna basınız.";

	VideoCapture capture(0);
	Mat frame;
	namedWindow("Kamera");
	while (true) {
		capture.read(frame);
		flip(frame, frame, 1);
		imshow("Kamera", frame);
		if (waitKey(50) == 27) break;
	}
	system("CLS");
	cout << " " << endl;
	cout << "   << KAMERA MODU KAPATILIYOR -> Ana Menüye Dönülüyor... >>";
	destroyWindow("Kamera");
	capture.release();
	frame.release();
	mod = 0;
	ModYonlendir();
}
void Mod2() { 
	system("CLS");
	cout << "                                                  " << endl;
	cout << "   OpenCV Görüntü İşleme - Pozitif Örnek Menüsü   " << endl;
	cout << "   *-----*------------------------------------*   " << endl;
	cout << "   |  1  | İmleç yardımıyla işaretleyerek     |   " << endl;
	cout << "   |  2  | Otomatik yüz tespiti yardımıyla    |   " << endl;
	cout << "   *-----*------------------------------------*   " << endl;
	cout << "   |  0  | Ana menü                           |   " << endl;
	cout << "   *-----*------------------------------------*   " << endl;
	cout << "   Çalıştırmak istediğiniz mod numarası: ";
	cin >> mod;
	if (mod == 1) mod = 21;
	else if (mod == 2) mod = 22;
	else mod = 0;
	ModYonlendir();
}
void ImlecKoordinatKontrolu(int event, int x, int y, int flags, void *ek) { 
	if (event == EVENT_LBUTTONDOWN) { cizim = true; xx1 = x; yy1 = y; xx2 = x; yy2 = y; }
	if (event == EVENT_MOUSEMOVE && cizim) { xx2 = x; yy2 = y; }
	if (event == EVENT_LBUTTONUP) { cizim = false; }
}
void Mod21() { 
	pozitif_txt.open("pozitif.txt", ios::app);
	int pozitifGoruntuNo = 0;
	system("CLS");
	cout << "   " << endl;
	cout << "   Kaçıncı pozitif resimde kaldığınızı giriniz: ";
	cin >> pozitifGoruntuNo;
	pozitifGoruntuNo++;
	if (pozitifGoruntuNo != 1) pozitif_txt << "\r" << endl;
	system("CLS");
	cout << "   " << endl;
	cout << "   << POZİTİF ÖRNEK OLUŞTURUCU MOD-1 ETKİNLEŞTİRİLDİ >>" << endl;
	cout << "   S harfi tuşuna basarak oluşturucu modunu açıp kapatabilirsiniz." << endl;
	cout << "   İmleç yardımıyla, objenin görüntüsünü dikdörtgen içine alınız." << endl;
	cout << "   İşaretlenmiş görüntünün kaydı için fareyi bırakmadan boşluk tuşuna basınız." << endl;
	cout << "   Ana menüye dönmek için, kamera akarken ESC tuşuna basınız.";

	VideoCapture capture(0);
	Mat frame, yanFrame;
	namedWindow("Kamera");
	bool cizimModu = false;
	int buton = 0;
	char goruntuAdi[100];
	while (true) {
		buton = waitKey(50);
		if (buton == 83 || buton == 115) cizimModu = (cizimModu) ? false : true;
		capture.read(frame);
		flip(frame, frame, 1);
		cvtColor(frame, yanFrame, CV_BGR2GRAY);
		equalizeHist(yanFrame, yanFrame);
		if (cizimModu) { // S tuşuna basıldığında cizimModu açılır ve fare tıklamaları dikkate alınır.
			circle(frame, Point(30, 30), 12, Scalar(0, 0, 255), -1);
			setMouseCallback("Kamera", ImlecKoordinatKontrolu, NULL);
			if (cizim) { // Fareye tıklandığında bu blok işler
				rectangle(frame, Point(xx1, yy1), Point(xx2, yy2), Scalar(0, 0, 255));
				if (buton == 32) { // cizimModu açıkken boşluk tuşuna basılınca pozitif örnek kaydedilir.
					_snprintf_s(goruntuAdi, 100, "pozitif/img%d.jpg", pozitifGoruntuNo);
					// Pencerede normal görüntü gösteriliyor olsa da 
					// gri skalada histogram eşitlenmiş görüntü kaydediliyor.
					// Orijinal görüntüyü de kaydedebilirdik. Mecburi değil:
					imwrite(goruntuAdi, yanFrame, { CV_IMWRITE_JPEG_QUALITY ,100 });
					_snprintf_s(goruntuAdi, 250, "pozitif\\img%d.jpg", pozitifGoruntuNo);
					pozitif_txt
						<< goruntuAdi << " 1 "
						<< xx1 << " "
						<< yy1 << " "
						<< xx2 - xx1 << " "
						<< yy2 - yy1 << endl;
					pozitifGoruntuNo++;
					cizim = false;
				}
			}
		}
		putText(
			frame,
			to_string(pozitifGoruntuNo - 1),
			Point(70, 40),
			HersheyFonts::FONT_HERSHEY_COMPLEX,
			1,
			Scalar(255, 0, 0));
		imshow("Kamera", frame);
		if (buton == 27) break;
	}
	destroyWindow("Kamera");
	pozitif_txt.close();
	capture.release();
	frame.release();
	yanFrame.release();
	mod = 0;
	ModYonlendir();
}
void Mod22() { 
	pozitif_txt.open("pozitif.txt", ios::app);
	int pozitifGoruntuNo = 0;
	system("CLS");
	cout << "   " << endl;
	cout << "   Kaçıncı pozitif resimde kaldığınızı giriniz: ";
	cin >> pozitifGoruntuNo;
	pozitifGoruntuNo++;
	if (pozitifGoruntuNo != 1) pozitif_txt << "\r" << endl;
	system("CLS");
	cout << "   " << endl;
	cout << "   << POZİTİF ÖRNEK OLUŞTURUCU MOD-2 ETKİNLEŞTİRİLDİ >>" << endl;
	cout << "   Boşluk tuşuna basarak kayıt modunu başlatıp durdurabilirsiniz." << endl;
	cout << "   Kayıt modu açıksa pencerenin sol üst köşesinde kırmızı bir daire belirecektir." << endl;
	cout << "   Yazılım, ekranda yüz bulduğu her an otomatik olarak kayıt yapacaktır." << endl;
	cout << "   Ana menüye dönmek için, kamera akarken ESC tuşuna basınız.";

	VideoCapture capture(0);
	Mat frame, yanFrame;
	bool kayitModu = false;
	int buton = 0;
	namedWindow("Kamera");
	char goruntuAdi[100];
	CascadeClassifier dedektor_frontalFace("haarcascade_frontalface_default.xml");
	int minKomsu = 10;
	double scaleStep = 1.1;
	Size minNesne(100, 100);
	Size maxNesne(250, 250);
	vector<Rect> bulunan_frontalFace;

	while (true) {
		buton = waitKey(50);
		if (buton == 32) kayitModu = (kayitModu) ? false : true;
		capture.read(frame);
		flip(frame, frame, 1);
		cvtColor(frame, yanFrame, CV_BGR2GRAY);
		equalizeHist(yanFrame, yanFrame);
		bulunan_frontalFace.clear();
		dedektor_frontalFace.detectMultiScale(yanFrame, bulunan_frontalFace, scaleStep, minKomsu, 2, minNesne, maxNesne);

		if (bulunan_frontalFace.size() > 0) {
			if (kayitModu) {
				_snprintf_s(goruntuAdi, 100, "pozitif/img%d.jpg", pozitifGoruntuNo);
				imwrite(goruntuAdi, yanFrame, { CV_IMWRITE_JPEG_QUALITY ,100 });
				_snprintf_s(goruntuAdi, 100, "pozitif\\img%d.jpg", pozitifGoruntuNo);
				pozitif_txt
					<< goruntuAdi << " 1 "
					<< bulunan_frontalFace[0].x << " "
					<< bulunan_frontalFace[0].y << " "
					<< bulunan_frontalFace[0].width << " "
					<< bulunan_frontalFace[0].height << endl;
				pozitifGoruntuNo++;
				circle(frame, Point(30, 30), 12, Scalar(0, 0, 255), -1);
				rectangle(frame, bulunan_frontalFace[0].br(), bulunan_frontalFace[0].tl(), Scalar(0, 0, 255));
			}
			else rectangle(frame, bulunan_frontalFace[0].br(), bulunan_frontalFace[0].tl(), Scalar(0, 0, 0));
		}
		putText(
			frame,
			to_string(pozitifGoruntuNo - 1),
			Point(70, 40),
			HersheyFonts::FONT_HERSHEY_COMPLEX,
			1,
			Scalar(255, 0, 0));
		imshow("Kamera", frame);
		if (buton == 27) break;
	}
	destroyWindow("Kamera");
	capture.release();
	frame.release();
	yanFrame.release();
	pozitif_txt.close();
	mod = 0;
	ModYonlendir();
}
void Mod3() { 
	negatif_txt.open("negatif.txt", ios::app);
	int negatifGoruntuNo;
	system("CLS");
	cout << "   " << endl;
	cout << "   Kaçıncı negatif örnekte kaldığınızı giriniz: ";
	cin >> negatifGoruntuNo;
	negatifGoruntuNo++;
	if (negatifGoruntuNo != 1) negatif_txt << "\r" << endl;

	system("CLS");
	cout << "   " << endl;
	cout << "   << NEGATİF ÖRNEK OLUŞTURUCU MOD ETKİNLEŞTİRİLDİ >>" << endl;
	cout << "   Boşluk tuşuna basarak negatif örneği kaydedebilirsiniz" << endl;
	cout << "   Ana menüye dönmek için \"Kamera\" penceresindeyken ESC tuşuna basınız.";

	int buton = 0;
	VideoCapture capture(0);
	Mat frame;
	namedWindow("Kamera");
	char goruntuAdi[100];
	bool kayitModu = false;
	while (true) {
		buton = waitKey(50);
		capture.read(frame);
		flip(frame, frame, 1);
		if (buton == 32) kayitModu = (kayitModu) ? false : true;
		if (kayitModu) {
			_snprintf_s(goruntuAdi, 200, "negatif/img%d.jpg", negatifGoruntuNo);
			imwrite(goruntuAdi, frame, { CV_IMWRITE_JPEG_QUALITY ,100 });
			_snprintf_s(goruntuAdi, 250, "negatif\\img%d.jpg", negatifGoruntuNo);
			negatif_txt << goruntuAdi << endl;
			negatifGoruntuNo++;
			circle(frame, Point(30, 30), 10, Scalar(0, 0, 255), -1);
		}
		putText(
			frame,
			to_string(negatifGoruntuNo - 1),
			Point(70, 40),
			HersheyFonts::FONT_HERSHEY_COMPLEX,
			1,
			Scalar(255, 0, 0));
		imshow("Kamera", frame);
		if (buton == 27) break;
	}
	destroyWindow("Kamera");
	capture.release();
	frame.release();
	negatif_txt.close();
	mod = 0;
	ModYonlendir();
}
void Mod4() { 
	int sayi = 0;
	char komut[200];
	system("CLS");
	cout << "   " << endl;
	cout << "   << EĞİTİM VERİSİ OLUŞTURMA MODU >>" << endl;
	cout << "   İşlem yapmadan ana menüye dönmek için 0 giriniz." << endl;
	cout << "   " << endl;
	cout << "   Pozitif görüntülerden VEC dosyası oluşturmak için," << endl;
	cout << "   Kaydedilmiş pozitif görüntü sayısını giriniz: ";
	cin >> sayi;

	system("CLS");
	cout << "   " << endl;
	cout << "   << EĞİTİM VERİSİ OLUŞTURMA MODU ETKİNLEŞTİRİLDİ >>" << endl;
	_snprintf_s(komut, 200, "opencv_createsamples.exe -info pozitif.txt -num %d -vec egitimseti.vec -w 24 -h 24", sayi);
	cout << "   " << sayi << " sayıda pozitif görüntüden vektorel egitim seti olusturuluyor..." << endl;
	system(komut);
	cout << "   Vektorel egitim seti olusturuldu." << endl;
	cout << "   Ana menüye dönmek için 0 girin." << endl;
	cout << "   Eğitim verisi elemanlarını tek tek kontrol etmek için 1 girin." << endl;
	cout << "   Yapmak istediğiniz işlem: "; cin >> sayi;
	if (sayi == 1) mod = 5;
	else mod = 0;
	ModYonlendir();
}
void Mod5() { 
	system("CLS");
	cout << "   " << endl;
	cout << "   << EĞİTİM VERİSİ KONTROL MODU >>" << endl;
	cout << "   Kontrol modu etkinleştirilsin mi? (e/h) ";
	char cevap; cin >> cevap;
	if (cevap == 'E' || cevap == 'e' || cevap == 'Y' || cevap == 'y' || cevap == '1') {
		system("CLS");
		cout << "   " << endl;
		cout << "   << EĞİTİM VERİSİ KONTROL MODU ETKİNLEŞTİRİLDİ >>" << endl;
		cout << "   Sıradaki görüntüye geçmek için boşluk tuşuna basınız." << endl;
		cout << "   Görüntüler bittiğinde otomatik olarak ana sayfaya yönlendirileceksiniz." << endl;
		system("opencv_createsamples.exe -vec egitimseti.vec PAUSE");
	}
	mod = 0;
	ModYonlendir();
}
void Mod6() { 
	system("CLS");
	cout << "   " << endl;
	cout << "   << EĞİTİM VERİSİ KONTROL MODU >>" << endl;
	cout << "   Bu modu etkinleştirdiğinizde uzun sürebilecek olan eğitim gerçekleştirilir." << endl;
	cout << "   Eğitim sonucunda XML dosyası oluşturulur." << endl;
	cout << "   Eğitim modunu etkinleştirmek istiyor musunuz? (e/h) ";
	char cevap; cin >> cevap;
	if (cevap == 'E' || cevap == 'e' || cevap == 'Y' || cevap == 'y' || cevap == '1') {
		int pozGor;
		int negGor;
		float minHitRate = 0.995;
		int asamaSayisi;
		cout << "   Pozitif görüntü sayınızı giriniz: "; cin >> pozGor;
		cout << "   Negatif görüntü sayınızı giriniz: "; cin >> negGor;
		cout << "   Eğitim aşama sayısını giriniz: "; cin >> asamaSayisi;
		int numPos = floor((pozGor - negGor) / (1 + (asamaSayisi - 1)*(1 - minHitRate)));
		char komut[500];
		_snprintf_s(komut, 500, "opencv_traincascade.exe -data cascade/ -vec egitimseti.vec -bg negatif.txt -numPos %d -numNeg %d -numStages %d -minHitRate 0.995 -maxFalseAlarmRate 0.25 -mem 1024 -w 24 -h 24", numPos, negGor, asamaSayisi);
		system(komut);
		system("PAUSE");
	}
	mod = 0;
	ModYonlendir();
}

void Mod7() {
VideoCapture capture(0);
Mat frame, yanFrame;
bool kayitModu = false;
int buton = 0;
namedWindow("Kamera");
CascadeClassifier dedektor_frontalFace("cascade/cascade.xml");
int minKomsu = 10;
double scaleStep = 1.1;
Size minNesne(50, 50);
Size maxNesne(300, 300);
vector<Rect> bulunan_obje;
Scalar kareRengi = Scalar(0, 0, 0);

while (true) {
	buton = waitKey(50);
	capture.read(frame);
	flip(frame, frame, 1);
	cvtColor(frame, yanFrame, CV_BGR2GRAY);
	equalizeHist(yanFrame, yanFrame);
	bulunan_obje.clear();
	dedektor_frontalFace.detectMultiScale(yanFrame, bulunan_obje, scaleStep, minKomsu, 2, minNesne, maxNesne);

	if (bulunan_obje.size() > 0) {
		for (int i = 0; i < bulunan_obje.size(); i++) {
			switch (i) {
			case 0: kareRengi = Scalar(0, 0, 0); break;
			case 1: kareRengi = Scalar(0, 0, 255); break;
			case 2: kareRengi = Scalar(0, 255, 0); break;
			case 3: kareRengi = Scalar(0, 255, 255); break;
			case 4: kareRengi = Scalar(255, 0, 0); break;
			case 5: kareRengi = Scalar(255, 0, 255); break;
			case 6: kareRengi = Scalar(255, 255, 0); break;
			case 7: kareRengi = Scalar(255, 255, 255); break;
			}
			rectangle(frame, bulunan_obje[i].br(), bulunan_obje[i].tl(), kareRengi, 2);
		}
	}
	imshow("Kamera", frame);
	if (buton == 27) break;
}
destroyWindow("Kamera");
capture.release();
frame.release();
yanFrame.release();
mod = 0;
ModYonlendir();
}





