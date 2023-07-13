/*
Шаблон проекта для библиотек mpir и gmp.


Маткин Илья Александрович 27.02.2013
*/


#include <stdio.h>
#include <time.h>
#include <string.h>


//----------------------------------------

//#define _USE_GMP
//#define _USE_DLL


#ifdef _WIN64

#include <mpir.h>
#ifdef _USE_DLL
//#pragma comment( lib, "lib\\mpir\\64\\dll\\gc\\mpir.lib" )
#pragma comment( lib, "lib\\mpir\\64\\dll\\core2\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\64\\dll\\sandybridge\\mpir.lib" )
#else // _USE_DLL
#pragma comment( lib, "lib\\mpir\\64\\lib\\gc\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\64\\lib\\core2\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\64\\lib\\sandybridge\\mpir.lib" )
#endif // _USE_DLL

#else // _WIN64

#ifdef _USE_GMP

#ifdef _USE_DLL
#include <gmp_d.h>
#pragma comment( lib, "lib\\gmp\\32\\libgmp_5.1.1.dll.a" )
#else // _USE_DLL
#include <gmp_s.h>
#pragma comment( lib, "lib\\gmp\\32\\libgmp.a" )
#pragma comment( lib, "libgcc.a" )
#endif // _USE_DLL

#else // _USE_GMP

#include <mpir.h>
#ifdef _USE_DLL
#pragma comment( lib, "lib\\mpir\\32\\dll\\gc\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\32\\dll\\pentium4\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\32\\dll\\pentium4_sse2\\mpir.lib" )
#else // _USE_DLL
#pragma comment( lib, "lib\\mpir\\32\\lib\\gc\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\32\\lib\\pentium4\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\32\\lib\\pentium4_sse2\\mpir.lib" )
#endif // _USE_DLL

#endif // _USE_GMP

#endif // _WIN64


//----------------------------------------


clock_t start;
clock_t end;

#define calculate_time(start_time, end_time, action) \
    start_time = clock();\
    (action);\
    end_time = clock();\
    printf ("%f\n", (double)(end_time - start_time) / CLK_TCK);


//----------------------------------------


int main (unsigned int argc, char *argv[]) {

mpz_t a;
mpz_t b;
mpz_t c;
mpz_t res;


    mpz_init_set_str (a, "13134147134718327417348173294817394701639162309813204140518230471340176320513129846103247816034612034610234612390846102839641023419862130412034861023846102386401829364102834601892132412341234123412341234123412341234123413241234123412341234123412341234123412341234123412341234123412341234123412341234123412341234123412341234134132412341234123412341234123847189234912374987123984712398471298347981237498123741293874981234781810923481093284091238409123840912834091823409182309481230948123094812309481239041823490128349123849012384123413284183247891234781237418239041092384", 0);
    mpz_init_set_str (b, "8910326918657362489173049817048710364891326401982364019823409816234098123409861230984612389461203984610293846120934610293846102934612930846102398641289306419823748912374812348912734891720346121387419082374908172340981723049817230948712039847109834709832134134132412341234123412341234123412341234810293849123481092384091238409123849102384019238401923840912384901238401923849012384901238491283491324132419238409128340918234913413241234192384910238401923840912384091238409123840912384901238401923840912384019234890818324908123094812309481923048091234812034981920341341341234", 0);

    //mpz_init (a);
    mpz_inits (res, c, NULL);

    mpz_powm_ui (c, a, 2, b);    // c = a^2 (mod b)
    mpz_mul (c, a, b);
    
    // res = a^b (mod c)
    calculate_time (start, end, mpz_powm (res, a, b, c));
    //mpz_powm (res, a, b, c);

    gmp_printf ("%Zd\n*\n%Zd\n=\n%Zx\n", a, b, c);

    mpz_add (a, a, b);
    gmp_printf ("a+b = %Zx\n", a);

    mpz_clear (res);
    mpz_clear (b);
    mpz_clear (a);
    mpz_clear (c);

    return 0;
}
