//入力端子
int APPS1 = A1;  //アクセルセンサ1信号
int Ready_sig = A2;  //ReadyON信号入力
int APPS2 = A3;  //アクセルセンサ2信号
int Seizyo_sig = A4;//異常信号入力
int BSE  = A5;  //ブレーキセンサ信号
int Mot_sig_kanshi = A6; //モータ出力監視
//出力端子
int Mot_analog_sig = A0;  //モーター出力信号
int izyo_sig = 2; //アクセル系統異常信号出力
int BSE_zyoutai_sig = 3;//ブレーキ状態信号出力
int APPS_zyoutai_sig = 5;//アクセル状態信号出力

//閾値
int APPS1_zyogen; //アクセルセンサ1上限
int APPS1_kagen; //アクセルセンサ1下限
int APPS2_zyogen; //アクセルセンサ2上限
int APPS2_kagen; //アクセルセンサ2下限
int BSE_zyogen; //ブレーキセンサ上限
int BSE_kagen; //ブレーキセンサ下限
int APPS_anzeniki , BSE_anzeniki;//アクセル、ブレーキセンサの閾値範囲
//復帰判定用
int APPS1_zyogen_100; //アクセルセンサ1上限-100
int APPS1_kagen_100; //アクセルセンサ1下限+100
int APPS2_zyogen_100; //アクセルセンサ2上限-100
int APPS2_kagen_100; //アクセルセンサ2下限+100
int BSE_zyogen_100; //ブレーキセンサ上限-100
int BSE_kagen_100; //ブレーキセンサ下限+100
//異常値
int APPS_izyo; //アクセルセンサ異常
int BSE_izyo; //ブレーキセンサ異常
int Douzifumi; //アクセル、ブレーキ同時踏み
//その他
int APPS_on; //アクセルon状態
int APPS_off;
int BSE_on; //ブレーキ作動中
int BSE_off;
int volt; //アクセルセンサ1の電圧(×10)
int volt2;//モータ出力の電圧(×10)
int Mot_power ; //モータ出力値
int Ready_on; //Ready_on状態
int Ready_off; //Ready_off状態
int APPS_izyo2 , Bra_izyo2;
int APPS_25;//アクセル開度25%
int APPS_sa;
int APPS_sa_fukki;
int APPS_izyo_fukki;
int BSE_izyo_fukki;
int APPS_5;
int APBS,A;
int AP2,re,b,z,D,gg;
unsigned long time , seizyo_time , douzi_time , APPS_sa_seizyo_time , MEGA_izyo_time,APPS_ON_time;
unsigned long fukki_time,go_time , BSE_time;

void setup() {
  Serial.begin(9600);
  pinMode(0, OUTPUT);
}

void loop() {
  time = millis(); 
  analogReadResolution(12); // read the input on A0 at default resolution (12 bits)
  analogWriteResolution(12);

  //入力端子
  APPS1 = analogRead(A1);
  Ready_sig = analogRead (A2);
  APPS2 = analogRead(A3);
 Seizyo_sig = analogRead(A4);
  BSE = analogRead(A5);
  Mot_sig_kanshi = analogRead (A6);

  //置き換え
  volt = map(APPS1, 0, 4095, 0, 33);
  volt2 = map(Mot_power, 0, 4095, 0, 33);

  //シリアルモニタ
   Serial.print("Ready信号");
  Serial.print(Ready_sig);
  Serial.print("正常信号");
  Serial.print(Seizyo_sig);
  Serial.print("メイン時間");
  Serial.print(time);
  Serial.print("正常時の時間");
  Serial.print(seizyo_time);
  Serial.print("同時踏み時間");
  Serial.print(douzi_time);
  Serial.print("アクセルセンサの差に問題なし時間");
  Serial.print(APPS_sa_seizyo_time);
  Serial.print("アクセル開度①");
  Serial.print(APPS1);
  Serial.print("電圧");
  Serial.print(volt);
  Serial.print("アクセル開度②");
  Serial.print(APPS2);
  Serial.print("ブレーキ開度");
  Serial.print(BSE);
  Serial.print("モーター出力監視");
  Serial.print(Mot_sig_kanshi);
  Serial.print("モーター電圧");
  Serial.println(volt2);
  APBS= APPS_5 && BSE_off;
  //閾値設定
  APPS1_zyogen = (APPS1 >= 3000);
  APPS1_kagen = (APPS1 <= 500);
  APPS2_zyogen = (APPS2 >= 3000);
  APPS2_kagen = (APPS2 <= 500);
  BSE_zyogen = (BSE >= 3000);
  BSE_kagen = (BSE <= 500);
  APPS_anzeniki = ( APPS1<=3000 && APPS1>=500 && APPS2<=3000 && APPS2>=500);
  BSE_anzeniki = ( BSE<=3000 && BSE>=400);

  //復帰判定設定
  APPS1_zyogen_100 = (APPS1 <=2900);
  APPS1_kagen_100 = (APPS1 >=600);
  APPS2_zyogen_100 = (APPS2 <=2900);
  APPS2_kagen_100 = (APPS2 >=600);
  BSE_zyogen_100 = (BSE <=2900);
  BSE_kagen_100 = (BSE >=600);
  APPS_sa_fukki = (APPS_sa<=125 || APPS_sa>=-125);
  APPS_izyo_fukki = (APPS1_zyogen_100 && APPS1_kagen_100 && APPS2_zyogen_100 && APPS2_kagen_100 );
  BSE_izyo_fukki =  (BSE_zyogen_100 && BSE_kagen_100);
  //APPS_5 = (APPS1<=963 && APPS2<=963);
  
  //作動設定
  Ready_on = (Ready_sig >= 400 && Seizyo_sig>=350);
  Ready_off = (Ready_sig <= 400 ||  Seizyo_sig<=350);
  APPS_on = (APPS1 >= 850);
  APPS_off = (APPS1 <=840);
  BSE_on = (BSE >= 1000);
  BSE_off = (BSE <= 990);
 
  //異常設定
  APPS_izyo = ( APPS1_zyogen || APPS1_kagen || APPS2_zyogen || APPS2_kagen);
  BSE_izyo = ( BSE_zyogen || BSE_kagen );
  Douzifumi = ( APPS_25 && BSE_on );
  APPS_sa = (APPS1 - APPS2);
  
  //その他
  Mot_power = (APPS1-850)*2.5;
  APPS_25 = (APPS1 >= 1263 ); //アクセル開度25%以上
  z=1;
  //BSE_val = (BSE);

    digitalWrite(izyo_sig, HIGH);
  //モータ出力固定
  if (Mot_power >= 3800) {
    Mot_power = 3800; 
    }
    if(APPS1_zyogen || APPS1_kagen || APPS2_zyogen || APPS2_kagen){
       Mot_power = 0;
   }
  if (Mot_power <= 0) {
    Mot_power = 0;
  }
  //時間
   if(APPS1 < 413 && BSE_off){
      douzi_time = time;
    }
    if(APPS_sa<=165 && APPS_sa>=-165){
      APPS_sa_seizyo_time =time;
    }
    if(APPS_anzeniki&&BSE_anzeniki){  
      seizyo_time = time; 
    }
  //Ready on判定用
  if(APPS_on){
    digitalWrite(APPS_zyoutai_sig,LOW);
  }
  if(APPS_off){
    digitalWrite(APPS_zyoutai_sig,HIGH);
  }
  if(BSE_on){
    digitalWrite(BSE_zyoutai_sig,HIGH);
  }
  if(BSE_off){
    digitalWrite(BSE_zyoutai_sig,LOW);
  }
  /*if ( BSE_on && APPS_off){
    digitalWrite(Ready_on_hanteiyo,HIGH);
  }*/
  //if(Seizyo_sig>=800){

      
     if (APPS_sa>=165 || APPS_sa<=-165) {                     //アクセルセンサ1と2の差が10%以上で作動
     if(re>=1) {
     analogWrite(Mot_analog_sig,Mot_power);
     }
     if(time-APPS_sa_seizyo_time>=500){
      while ( z>=1 ) {
        //閾値設定
  APPS1_zyogen = (APPS1 >= 3000);
  APPS1_kagen = (APPS1 <= 500);
  APPS2_zyogen = (APPS2 >= 3000);
  APPS2_kagen = (APPS2 <= 500);
  BSE_zyogen = (BSE >= 3000);
  BSE_kagen = (BSE <= 500);
  APPS_anzeniki = ( APPS1<=3000 && APPS1>=500 && APPS2<=3000 && APPS2>=500);
  BSE_anzeniki = ( BSE<=3000 && BSE>=400);
        //作動設定
  Ready_on = (Ready_sig >= 400 && Seizyo_sig>=350);
  Ready_off = (Ready_sig <= 400 ||  Seizyo_sig<=350);
  APPS_on = (APPS1 >= 850);
  APPS_off = (APPS1 <=840);
  BSE_on = (BSE >= 1000);
  BSE_off = (BSE <= 990);

  //異常設定
  APPS_izyo = ( APPS1_zyogen || APPS1_kagen || APPS2_zyogen || APPS2_kagen);
  BSE_izyo = ( BSE_zyogen || BSE_kagen );
  Douzifumi = ( APPS_25 && BSE_on );
  APPS_sa = (APPS1 - APPS2);
        //復帰判定設定
  APPS1_zyogen_100 = (APPS1 <=2900);
  APPS1_kagen_100 = (APPS1 >=600);
  APPS2_zyogen_100 = (APPS2 <=2900);
  APPS2_kagen_100 = (APPS2 >=600);
  BSE_zyogen_100 = (BSE <=2900);
  BSE_kagen_100 = (BSE >=600);
  APPS_sa_fukki = (APPS_sa<=125 || APPS_sa>=-125);
  APPS_izyo_fukki = (APPS1_zyogen_100 && APPS1_kagen_100 && APPS2_zyogen_100 && APPS2_kagen_100 );
  BSE_izyo_fukki =  (BSE_zyogen_100 && BSE_kagen_100);
  //APPS_5 = (APPS1<=963 && APPS2<=963);
        analogWrite(Mot_analog_sig,0);
        digitalWrite(izyo_sig, LOW);
        Serial.print(Ready_sig);
        APPS1 = analogRead(A1);
        Ready_sig = analogRead (A2);
        APPS2 = analogRead(A3);
       Seizyo_sig = analogRead(A4);
        BSE = analogRead(A5);
        Mot_sig_kanshi = analogRead (A6);
        volt = map(APPS1, 0, 4095, 0, 33);
        volt2 = map(Mot_sig_kanshi, 0, 4095, 0, 33);
        time = millis();
        Serial.print("アクセルセンサ差異常");
        Serial.print("Ready信号");
        Serial.print(Ready_sig);
        Serial.print("正常信号");
        Serial.print(Seizyo_sig);
        Serial.print("メイン時間");
        Serial.print(time);
        Serial.print("正常時の時間");
        Serial.print(seizyo_time);
        Serial.print("同時踏み時間");
        Serial.print(douzi_time);
        Serial.print("アクセルセンサの差に問題なし時間");
        Serial.print(APPS_sa_seizyo_time);
        Serial.print("アクセル開度①");
        Serial.print(APPS1);
        Serial.print("電圧");
        Serial.print(volt);
        Serial.print("アクセル開度②");
        Serial.print(APPS2);
        Serial.print("ブレーキ開度");
        Serial.print(BSE);
        Serial.print("モーター出力監視");
        Serial.print(Mot_sig_kanshi);
        Serial.print("モーター電圧");+
        Serial.println(volt2);
        if(BSE_on && b<=0){ 
          b=1;
          BSE_time=time; 
          }
          if(time-BSE_time<=100){
            b=0;
        if(BSE_on){
    digitalWrite(BSE_zyoutai_sig,HIGH);
  }
          }
  if(BSE_off){
    digitalWrite(BSE_zyoutai_sig,LOW);
  }
              if( APPS_sa>=165 || APPS_sa<=-165){ 
          fukki_time=time;
        }
        if( APPS_sa_fukki&& time-fukki_time>=50){
          digitalWrite(izyo_sig, HIGH);
          return;
        }
       
      }
     }
    }
     if( APPS_izyo || BSE_izyo ){                                 //アクセル・ブレーキセンサ異常で作動
     if(re>=1) {
     analogWrite(Mot_analog_sig,Mot_power);
      if(BSE_izyo){
        digitalWrite(BSE_zyoutai_sig,LOW);
       }
     }
     if( time-seizyo_time>=500){
      while (z>=1 ) {
      
        //閾値設定
  APPS1_zyogen = (APPS1 >= 3000);
  APPS1_kagen = (APPS1 <= 500);
  APPS2_zyogen = (APPS2 >= 3000);
  APPS2_kagen = (APPS2 <= 500);
  BSE_zyogen = (BSE >= 3000);
  BSE_kagen = (BSE <= 500);
  APPS_anzeniki = ( APPS1<=3000 && APPS1>=500 && APPS2<=3000 && APPS2>=500);
  BSE_anzeniki = ( BSE<=3000 && BSE>=500);
        //作動設定
  Ready_on = (Ready_sig >= 400 && Seizyo_sig>=350);
  Ready_off = (Ready_sig <= 400 ||  Seizyo_sig<=350);
  APPS_on = (APPS1 >= 850);
  APPS_off = (APPS1 <=840);
  BSE_on = (BSE >= 1000);
  BSE_off = (BSE <= 990);

  //異常設定
  APPS_izyo = ( APPS1_zyogen || APPS1_kagen || APPS2_zyogen || APPS2_kagen);
  BSE_izyo = ( BSE_zyogen || BSE_kagen );
  Douzifumi = ( APPS_25 && BSE_on );
  APPS_sa = (APPS1 - APPS2);
        //復帰判定設定
  APPS1_zyogen_100 = (APPS1 <=2900);
  APPS1_kagen_100 = (APPS1 >=600);
  APPS2_zyogen_100 = (APPS2 <=2900);
  APPS2_kagen_100 = (APPS2 >=600);
  BSE_zyogen_100 = (BSE <=2900);
  BSE_kagen_100 = (BSE >=600);
  APPS_sa_fukki = (APPS_sa<=125 || APPS_sa>=-125);
  APPS_izyo_fukki = (APPS1_zyogen_100 && APPS1_kagen_100 && APPS2_zyogen_100 && APPS2_kagen_100 );
  BSE_izyo_fukki =  (BSE_zyogen_100 && BSE_kagen_100);
 // APPS_5 = (APPS1<=963 && APPS2<=963);
        analogWrite(Mot_analog_sig,0);
        digitalWrite(izyo_sig, LOW);
        Serial.print(Ready_sig);
        APPS1 = analogRead(A1);
        Ready_sig = analogRead (A2);
        APPS2 = analogRead(A3);
       Seizyo_sig = analogRead(A4);
        BSE = analogRead(A5);
        Mot_sig_kanshi = analogRead (A6);
        volt = map(APPS1, 0, 4095, 0, 33);
        volt2 = map(Mot_sig_kanshi, 0, 4095, 0, 33);
        time = millis();
        Serial.print("アクセル系統異常");
        Serial.print("Ready信号");
  Serial.print(Ready_sig);
  Serial.print("正常信号");
  Serial.print(Seizyo_sig);
        Serial.print("メイン時間");
        Serial.print(time);
        Serial.print("正常時の時間");
        Serial.print(seizyo_time);
        Serial.print("同時踏み時間");
        Serial.print(douzi_time);
        Serial.print("アクセルセンサの差に問題なし時間");
        Serial.print(APPS_sa_seizyo_time);
        Serial.print("アクセル開度①");
        Serial.print(APPS1);
        Serial.print("電圧");
        Serial.print(volt);
        Serial.print("アクセル開度②");
        Serial.print(APPS2);
        Serial.print("ブレーキ開度");
        Serial.print(BSE);
        Serial.print("モーター出力監視");
        Serial.print(Mot_sig_kanshi);
        Serial.print("モーター電圧");
        Serial.println(fukki_time);
         if(BSE_on && b<=0){ 
          b=1;
          BSE_time=time; 
          }
          if(time-BSE_time<=100){
            b=0;
        if(BSE_on){
    digitalWrite(BSE_zyoutai_sig,HIGH);
  }
          }
          if(BSE_izyo){
        digitalWrite(BSE_zyoutai_sig,LOW);
       }
  if(BSE_off){
    digitalWrite(BSE_zyoutai_sig,LOW);
  }
        if( APPS_izyo || BSE_izyo ){ 
          fukki_time=time;
        }
        if(APPS_izyo_fukki && BSE_izyo_fukki && time-fukki_time>=50){
          digitalWrite(izyo_sig, HIGH);
          return;
        }
      } 
     }
    }
    if(APPS_25&&BSE_on){  //アクセル・ブレーキ同時踏み時に作動
      D=1;
       BSE = analogRead(A5);
         BSE_on = (BSE >= 1000);
  BSE_off = (BSE <= 990);
     if(re>=1) {
   BSE = analogRead(A5);      
   BSE_on = (BSE >= 1000);
  BSE_off = (BSE <= 990);
     analogWrite(Mot_analog_sig,Mot_power);
     if(BSE_off){
      D=0;
     }
     }
     gg=D>=1;
     if( time-douzi_time>=500){
      if(APPS_25&&BSE_on&&gg){
      while ( APPS1>=933 && APPS2>=933 ) {
        Mot_power = 0;
        //閾値設定
  APPS1_zyogen = (APPS1 >= 3000);
  APPS1_kagen = (APPS1 <= 500);
  APPS2_zyogen = (APPS2 >= 3000);
  APPS2_kagen = (APPS2 <= 500);
  BSE_zyogen = (BSE >= 3000);
  BSE_kagen = (BSE <= 500);
  APPS_anzeniki = ( APPS1<=3000 && APPS1>=500 && APPS2<=3000 && APPS2>=500);
  BSE_anzeniki = ( BSE<=3000 && BSE>=500);
        //作動設定
  Ready_on = (Ready_sig >= 400 && Seizyo_sig>=350);
  Ready_off = (Ready_sig <= 400 ||  Seizyo_sig<=350);
  APPS_on = (APPS1 >= 850);
  APPS_off = (APPS1 <=840);
  BSE_on = (BSE >= 1000);
  BSE_off = (BSE <= 990);

  //異常設定
  APPS_izyo = ( APPS1_zyogen || APPS1_kagen || APPS2_zyogen || APPS2_kagen);
  BSE_izyo = ( BSE_zyogen || BSE_kagen );
  Douzifumi = ( APPS_25 && BSE_on );
  APPS_sa = (APPS1 - APPS2);
        //復帰判定設定
  APPS1_zyogen_100 = (APPS1 <=2900);
  APPS1_kagen_100 = (APPS1 >=600);
  APPS2_zyogen_100 = (APPS2 <=2900);
  APPS2_kagen_100 = (APPS2 >=600);
  BSE_zyogen_100 = (BSE <=2900);
  BSE_kagen_100 = (BSE >=600);
  APPS_sa_fukki = (APPS_sa<=125 || APPS_sa>=-125);
  APPS_izyo_fukki = (APPS1_zyogen_100 && APPS1_kagen_100 && APPS2_zyogen_100 && APPS2_kagen_100 );
  BSE_izyo_fukki =  (BSE_zyogen_100 && BSE_kagen_100);
  //APPS_5 = (APPS1<=963 && APPS2<=963);
  if(BSE_on && b<=0){ 
          b=1;
          BSE_time=time; 
          }
          
          if(time-BSE_time<=100){
            b=0;
if(BSE_on){
    digitalWrite(BSE_zyoutai_sig,HIGH);
  }
          }
  if(BSE_off){
    digitalWrite(BSE_zyoutai_sig,LOW);
  }
  if( APPS_izyo || BSE_izyo ){
    digitalWrite(BSE_zyoutai_sig,LOW);
     digitalWrite(izyo_sig, LOW);
  }
  if(APPS_izyo_fukki && BSE_izyo_fukki){
     digitalWrite(izyo_sig, HIGH);
  }
        analogWrite(Mot_analog_sig,Mot_power);
      //  digitalWrite(izyo_sig, HIGH);
        Serial.print(Ready_sig);
        APPS1 = analogRead(A1);
        Ready_sig = analogRead (A2);
        APPS2 = analogRead(A3);
       Seizyo_sig = analogRead(A4);
        BSE = analogRead(A5);
        Mot_sig_kanshi = analogRead (A6);
        volt = map(APPS1, 0, 4095, 0, 33);
        volt2 = map(Mot_sig_kanshi, 0, 4095, 0, 33);
        time = millis();
        APPS_sa_seizyo_time =time;
        seizyo_time = time; 
        Serial.print("同時踏み");
        Serial.print("メイン時間");
        Serial.print(time);
        Serial.print("正常時の時間");
        Serial.print(seizyo_time);
        Serial.print("同時踏み時間");
        Serial.print(douzi_time);
        Serial.print("アクセルセンサの差に問題なし時間");
        Serial.print(APPS_sa_seizyo_time);
        Serial.print("アクセル開度①");
        Serial.print(APPS1);
        Serial.print("電圧");
        Serial.print(volt);
        Serial.print("アクセル開度②");
        Serial.print(APPS2);
        Serial.print("ブレーキ開度");
        Serial.print(BSE);
        Serial.print("モーター出力監視");
        Serial.print(Mot_sig_kanshi);
        Serial.print("モーター電圧");
        Serial.println(volt2);
        /* if(APBS ){
          digitalWrite(2, LOW);
          return;
        }*/
      }
      digitalWrite(izyo_sig, HIGH);
      }
     } 
    } 
   /* if(Seizyo_sig<=1000 && A<=0){
      MEGA_izyo_time = time;
      A=1;
    }
    if(time-MEGA_izyo_time>=5000){
    Seizyo_sig = analogRead(A4);
     if(Seizyo_sig>=1000){
     while(Ready_on){
      //復帰判定設定
  APPS1_zyogen_100 = (APPS1 <=3700);
  APPS1_kagen_100 = (APPS1 >=450);
  APPS2_zyogen_100 = (APPS2 <=3700);
  APPS2_kagen_100 = (APPS2 >=450);
  BSE_zyogen_100 = (BSE <=3700);
  BSE_kagen_100 = (BSE >=400);
  APPS_sa_fukki = (APPS_sa<=145 || APPS_sa>=-145);
  APPS_izyo_fukki = (APPS1_zyogen_100 && APPS1_kagen_100 && APPS2_zyogen_100 && APPS2_kagen_100 );
  BSE_izyo_fukki =  (BSE_zyogen_100 && BSE_kagen_100);
  APPS_5 = (APPS1<=963 && APPS2<=963);
       analogWrite(Mot_analog_sig,0);
        digitalWrite(izyo_sig, LOW);
        Serial.print(Ready_sig);
        APPS1 = analogRead(A1);
        Ready_sig = analogRead (A2);
        APPS2 = analogRead(A3);
       Seizyo_sig = analogRead(A4);
        BSE = analogRead(A5);
        Mot_sig_kanshi = analogRead (A6);
        volt = map(APPS1, 0, 4095, 0, 33);
        volt2 = map(Mot_sig_kanshi, 0, 4095, 0, 33);
        time = millis();
        APPS_sa_seizyo_time =time;
        seizyo_time = time; 
        Serial.print("MEGA異常");
        Serial.print("メイン時間");
        Serial.print(time);
        Serial.print("正常時の時間");
        Serial.print(seizyo_time);
        Serial.print("同時踏み時間");
        Serial.print(douzi_time);
        Serial.print("アクセルセンサの差に問題なし時間");
        Serial.print(APPS_sa_seizyo_time);
        Serial.print("アクセル開度①");
        Serial.print(APPS1);
        Serial.print("電圧");
        Serial.print(volt);
        Serial.print("アクセル開度②");
        Serial.print(APPS2);
        Serial.print("ブレーキ開度");
        Serial.print(BSE);
        Serial.print("モーター出力監視");
        Serial.print(Mot_sig_kanshi);
        Serial.print("モーター電圧");
        Serial.println( Seizyo_sig);
        if( Seizyo_sig<=1000 ){ 
          fukki_time=time;
        }
        if( Seizyo_sig>=1000&& time-fukki_time>=3000){
          digitalWrite(izyo_sig, HIGH);
          return;
        }
     }
     }
    }*/
    /*if(BSE_on){
      digitalWrite(Bre_lamp,HIGH);
    }*/
   /* if (APPS_on && AP2<=0){
      APPS_ON_time = time;
      AP2=1;
    }
    
   if (APPS_on &&time- APPS_ON_time>=500 && AP2>=1 ){
     analogWrite(Mot_analog_sig,Mot_power);
   AP2=0;
   }*/  if ( Ready_on ) {
    if(Ready_on && re<=0){
    go_time=time;
    re=1;
     }
     if(time-go_time>=0 && re<=1 && re>=1){
      if ( Ready_on ){
   if(APPS_on){
    analogWrite(Mot_analog_sig,Mot_power);
   }
      }
  }
   }
   if(Ready_off){
    analogWrite(Mot_analog_sig,0);
    re=0;
   }
  }
  //}
