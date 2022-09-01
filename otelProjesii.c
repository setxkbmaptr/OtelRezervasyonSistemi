#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ODASAYISI 75
#define BUFFER_SIZE 1000

typedef struct{
	char *musteriAdi;
	char *musteriSoyadi;
}musteriBilgileri;

typedef struct{
	int gun;
	int ay;
	int yil;
}tarihBilgileri;	

typedef struct{
	double fiyatBilgisi;
	tarihBilgileri girisTarihi;
	tarihBilgileri cikisTarihi;
	int kisiSayisi;
	int odaDurumu; // 1 dolu 0 bos
	int odaNumarasi;	
	musteriBilgileri musteri;
}odaBilgileri;

typedef enum{
	musteri=0,personel=1
}yetkiSeviyeleri;
	
	
int  gunFarkiHesapla(tarihBilgileri girisTarihi,tarihBilgileri cikisTarihi){
	
	if(cikisTarihi.gun<girisTarihi.gun){
		cikisTarihi.gun+=30;
		cikisTarihi.ay-=1;
	}
	if(cikisTarihi.ay<girisTarihi.ay){
		cikisTarihi.ay+=12;
		cikisTarihi.yil-=1;
	}
	tarihBilgileri tarihFarki;
	tarihFarki.gun=cikisTarihi.gun-girisTarihi.gun;
	tarihFarki.ay=cikisTarihi.ay-girisTarihi.ay;
	tarihFarki.yil=cikisTarihi.yil-girisTarihi.yil;
	int kalmaFarki=tarihFarki.gun+tarihFarki.ay*30+tarihFarki.yil*365;
	
	return kalmaFarki;
}

double fiyatHesaplama(odaBilgileri *odalar, int odaNumarasi){
			
	int kalmaFarki=gunFarkiHesapla(odalar[odaNumarasi-1].girisTarihi,odalar[odaNumarasi-1].cikisTarihi);
	printf("********************************\n");
	printf("Konaklayacaginiz gun sayisi:%d\n",kalmaFarki);
	
	return 800*kalmaFarki*odalar[odaNumarasi-1].kisiSayisi;//kisi sayisi * 800 günlük fiyat * kac gün kaldigi
	
}

void rezervasyonTemizle(odaBilgileri *odalar, int odaNumarasi){
		
	
	odalar[odaNumarasi-1].odaDurumu=0; //0 bos 1 dolu demek.
	free(odalar[odaNumarasi-1].musteri.musteriAdi);
	free(odalar[odaNumarasi-1].musteri.musteriSoyadi);
	free(odalar[odaNumarasi-1].odaNumarasi);
	printf("********************************\n");
	printf("%d numarali oda bosaltilmistir.\n",odaNumarasi);
		
}

void doluOdaYazdirma(odaBilgileri *odalar){	////////?????????????*
		
	int i;
	for(i=0;i<ODASAYISI;i++){
		if(odalar[i].odaDurumu!=0){			//0 bos 1 dolu demek.
			printf("***************\n");
			printf("Oda numarasi:%d\n",odalar[i].odaNumarasi);
			printf("Musteri Adi ve Soyadi:%s %s\n",odalar[i].musteri.musteriAdi,odalar[i].musteri.musteriSoyadi);
			printf("Kisi Sayisi:%d\n",odalar[i].kisiSayisi);
			printf("Odenmesi Gereken Ucret:%.2lf\n",odalar[i].fiyatBilgisi);
			printf("Giris Tarihi:%d:%d:%d\n",odalar[i].girisTarihi.gun,odalar[i].girisTarihi.ay,odalar[i].girisTarihi.yil);
			printf("Cikis Tarihi:%d:%d:%d\n",odalar[i].cikisTarihi.gun,odalar[i].cikisTarihi.ay,odalar[i].cikisTarihi.yil);		
		}
	}		
} 		
	
void yeniMusteriEkleme(odaBilgileri *odalar){
		
	int sorgulanacakOda;
	char musteriAdi[20],musteriSoyadi[20];
	char ch;
	
	printf("Lutfen kayit yaptirmak istediginiz oda numarasini giriniz:");
	scanf("%d",&sorgulanacakOda);

	if(odalar[sorgulanacakOda-1].odaDurumu!=0){
		printf("Kayit yaptirmak istediginiz oda %d:%d:%d tarihine kadar doludur.\n",odalar[sorgulanacakOda-1].cikisTarihi.gun,odalar[sorgulanacakOda-1].cikisTarihi.ay,odalar[sorgulanacakOda-1].cikisTarihi.yil);
		return;
	}
	odalar[sorgulanacakOda-1].odaNumarasi=sorgulanacakOda;
	
	printf("Lutfen Adinizi ve Soyadinizi tuslayiniz:");
	scanf("%s%s",&musteriAdi,&musteriSoyadi);
	
	odalar[sorgulanacakOda-1].musteri.musteriAdi=(char*)malloc(sizeof(char)*strlen(musteriAdi)+1);
	odalar[sorgulanacakOda-1].musteri.musteriSoyadi=(char*)malloc(sizeof(char)*strlen(musteriSoyadi)+1);
	strcpy(odalar[sorgulanacakOda-1].musteri.musteriAdi,musteriAdi);
	strcpy(odalar[sorgulanacakOda-1].musteri.musteriSoyadi,musteriSoyadi);
	
	printf("Lutfen konaklayacak kisi sayisini giriniz:");
	scanf("%d",&odalar[sorgulanacakOda-1].kisiSayisi);	
	
	printf("Lutfen otele giris tarihinizi (gun:ay:yil) seklinde tuslayiniz.");
	scanf("%d:%d:%d",&odalar[sorgulanacakOda-1].girisTarihi.gun,&odalar[sorgulanacakOda-1].girisTarihi.ay,&odalar[sorgulanacakOda-1].girisTarihi.yil);
	
	printf("Lutfen otelden cikis tarihinizi (gun:ay:yil) seklinde tuslayiniz.");
	scanf("%d:%d:%d",&odalar[sorgulanacakOda-1].cikisTarihi.gun,&odalar[sorgulanacakOda-1].cikisTarihi.ay,&odalar[sorgulanacakOda-1].cikisTarihi.yil);

	odalar[sorgulanacakOda-1].fiyatBilgisi=fiyatHesaplama(odalar,sorgulanacakOda);
	printf("********************************\n");
	printf("Odemeniz gereken toplam tutar:%2.lfTL\n",odalar[sorgulanacakOda-1].fiyatBilgisi);
	printf(" Odemeyi kabul ediyor musunuz (Y-N):\n");
	printf("********************************\n");
	fflush(stdin);
	ch=getchar();
	if(ch=='Y'){
		printf("Odeme basariyla yapildi iyi tatiller.\n");							
		printf("********************************\n");
	}
	odalar[sorgulanacakOda-1].odaDurumu=0;// odamiz dolu
	if(ch=='N'){
		rezervasyonTemizle(odalar,sorgulanacakOda);					
		return;
	}
	else{
	odalar[sorgulanacakOda-1].odaDurumu=1;
	}
}

void odaBilgisiSorgulama(odaBilgileri *odalar, int odaNumarasi){
	
	if(odalar[odaNumarasi-1].odaDurumu!=0){			
		printf("********************************\n");
		printf("%d numarali oda %d:%d:%d tarihine kadar doludur.\n",odaNumarasi,odalar[odaNumarasi-1].cikisTarihi.gun,odalar[odaNumarasi-1].cikisTarihi.ay,odalar[odaNumarasi-1].cikisTarihi.yil);
		printf("********************************\n");
	}
	else{
		printf("********************************\n");
		printf("%d numarali oda suan musait durumdadir.\n",odaNumarasi);
		printf("********************************\n");
	}		
}

void fiyatBilgisiYazdirma(){
		
	double toplamFiyat=0.0;
	int kisiSayisi;
	tarihBilgileri girisTarihi,cikisTarihi;
				
	printf("Kac kisi kalmak istediginiz tuslayiniz:");
	scanf("%d",&kisiSayisi);
	printf("Lutfen giris tarihinizi (gun:ay:yil) olacak sekilde tuslayiniz:");
	scanf("%d:%d:%d",&girisTarihi.gun,&girisTarihi.ay,&girisTarihi.yil);
	printf("Lutfen cikis tarihinizi (gun:ay:yil) olacak sekilde tuslayiniz:");
	scanf("%d:%d:%d",&cikisTarihi.gun,&cikisTarihi.ay,&cikisTarihi.yil);
		
	toplamFiyat=gunFarkiHesapla(girisTarihi,cikisTarihi)*kisiSayisi*800;
	printf("********************************\n");
	printf("Toplam odenmesi gereken tutar:%.2lf\n",toplamFiyat);
	printf("********************************\n");		
}

void yedekKayitAlma(odaBilgileri *odalar){
		
	int i=0;
	FILE *otelKayitlar=fopen("otelKayitlar.txt","a+");
	if(otelKayitlar==NULL){
		fprintf(stderr,"Yedek kayit alinirken bir hata meydana geldi.");
		exit(1);
	}
	for(i=0;i<ODASAYISI;i++){
		if(odalar[i].odaDurumu!=0){
			fprintf(otelKayitlar,"Oda Numarasi:%d\n",odalar[i].odaNumarasi);
			fprintf(otelKayitlar,"Musterinin Adi-Soyadi:%s %s\n",odalar[i].musteri.musteriAdi,odalar[i].musteri.musteriSoyadi);
			fprintf(otelKayitlar,"Kisi Sayisi:%d\n",odalar[i].kisiSayisi);
			fprintf(otelKayitlar,"Toplam Tutar:%.2lfTL\n",odalar[i].fiyatBilgisi);
			fprintf(otelKayitlar,"Giris Tarihi:%d:%d:%d\n",odalar[i].girisTarihi.gun,odalar[i].girisTarihi.ay,odalar[i].girisTarihi.yil);
			fprintf(otelKayitlar,"Cikis Tarihi:%d:%d:%d\n",odalar[i].cikisTarihi.gun,odalar[i].cikisTarihi.ay,odalar[i].cikisTarihi.yil);
			fprintf(otelKayitlar,"\n");
		}
	}
	fclose(otelKayitlar);
}

int main(){
	
	int odaNumarasi,personelSifresi=121200,kullanicininGirdigiSifre;
	yetkiSeviyeleri kullaniciYetkisi;
	char ch;

	FILE *otelKayitlar=fopen("otelKayitlar.txt","r+");

	odaBilgileri *odaDizini,*odalar;// oda bilgileri burada saklidir.	
	odaDizini=(odaBilgileri*)calloc(ODASAYISI,sizeof(odaBilgileri));
		
	printf("************X Otel Rezervasyon Sistemine Hosgeldiniz:************");
	printf("\n");
	printf("Burada calisiyorsaniz sifrenizi giriniz Musteriyseniz Lutfen 0'i tuslayiniz':");
	scanf("%d",&kullanicininGirdigiSifre);
	if(personelSifresi==kullanicininGirdigiSifre){		//personel olmus oluyor.
		kullaniciYetkisi=personel;
	}
	else {												//musteri olmus oluyor.
		kullaniciYetkisi=musteri;
}
	do{	
		printf("********************************\n");
		printf("1.)Yeni Musteri girisi yapmak icin 'G'\n2.)Oda bilgilerini gormek istiyorsaniz'O'\n3.)Dolu odalari ogrenmek istiyorsaniz'D'\n");
	printf("4.)Fiyat bilgisi almak istiyorsaniz'F'\n5.)Musteri cikisi gerceklestirmek istiyorsaniz'M'\n6.)Sistemden cikis yapmak istiyorsaniz'Q' tuslayiniz ");
	fflush(stdin);
	ch=getchar();
	switch(ch){
			case 'G':
			case 'g':
					if(kullaniciYetkisi==personel){
						yeniMusteriEkleme(odaDizini);
					}
					else{
					printf("Burasi yetkili personel icindir.\n");
					}
					break;
			case 'O':
			case 'o':
					printf("Lutfen bilgilerini ogrenmek istediginiz oda numarasini giriniz:");
					scanf("%d",&odaNumarasi);
					odaBilgisiSorgulama(odaDizini,odaNumarasi);	
					break;			
			case 'D':
			case 'd':
					if(kullaniciYetkisi==personel){
						doluOdaYazdirma(odaDizini); 
					}
					else{
						printf("Burasi yetkili personel icindir.\n");
					}
					int ch;				//yeni.
					do{
						ch=fgetc(otelKayitlar);
					
						putchar(ch);
					}while(ch!=EOF);
					printf("\n");
					break;	
			case 'F':
			case 'f':
					fiyatBilgisiYazdirma();	
					break;
			case 'M':
			case 'm':
					if(kullaniciYetkisi==personel){
						printf("Cikis yapmak istediginiz odanin numarasini tuslayiniz:");
						scanf("%d",&odaNumarasi);
						rezervasyonTemizle(odaDizini,odaNumarasi);
						FILE *otelKayitlar=fopen("otelKayitlar.txt","w");
					}
					break;																				
			case 'Q':
			case 'q':
					yedekKayitAlma(odaDizini);	
					break;
			default:
					break;
			
			}
		}while(ch!='Q');
	
	return 0;
}
