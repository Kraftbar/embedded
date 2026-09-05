const int seq1[] = {66,89,125,54,89,66,54,37};
const int seq2[] = {81,106,125,54,106,81,54,37};


  
int g(int i,int x,int t,int o){
    return((3&x&(i*(((3&i>>16)!=0?seq1[t%8]:seq2[t%8])+51)>>o))<<4);
}

int s(int i){
  int n, s;
    return(
        g(i,1,n=i>>14,12) +
        g(i,s=i>>17,n^i>>13,10) +
        g(i,s/3,n+((i>>11)%3),10) +
        g(i,s/5,8+n-((i>>10)%3),9)
  );
}



/* //////////////////////////
Simple audio resampling
Copyright 2017 Theron Tarigo
*/

float mainSound( float time )
{
    // Simple resampling algorithm
    int samprate = 8000;
    double test;
    
    int i = int(modf(time,&test)*float(samprate))+int(floor(time))*samprate;
    float rem = modf(modf(time,&test)*float(samprate),&test);
    uint s1 = uint(s(i))%256u;
    uint s2 = uint(s(i+1))%256u;
    float f1 = float(s1)/255.;
    float f2 = float(s2)/255.;
    float f = f1*(0.5+0.5*cos(3.141592653589793*rem)) +
              f2*(0.5-0.5*cos(3.141592653589793*rem)) ;
    return f ;
}

/*0
  float lollll= esp_timer_get_time()/1000000;
  float looll=mainSound(lollll);
  dacWrite(25,looll);
 Serial.println(looll);
*/
