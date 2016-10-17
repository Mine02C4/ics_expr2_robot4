// 知的ロボ：モータ制御のサンプルプログラム
//   元ファイル：urobot_enc_pos.c
//   改＆コメ追加 2014.12.03 TSUCHIYA, Akihito
//

#include <stdio.h>
#include <stdlib.h>         // for Raspbian
#include <math.h>           // for Raspbian
#include <unistd.h>         /* for read, write, close */
#include <sys/ioctl.h>      /* for ioctl */
#include <sys/types.h>      /* for open */
#include <sys/stat.h>       /* for open */
#include <fcntl.h>          /* for open */
#include <signal.h>

#include "driver/urbtc.h"          /* Linux specific part */
#include "driver/urobotc.h"        /* OS independent part */

void exit_program();

// だいたい1回転分のセンサカウント値
const int ROT = 16*19/8;

// Ctl+Cが押されたかどうかのフラグ
int quit_flag = 1;

#define numOfControllers 1
static char *devfiles[ ] = { "/dev/urbtc0" };

int main( int argc, char **argv ) {
  struct uin ibuf;
  struct uout obuf[ numOfControllers ];
  struct ccmd cmd;
  int fd, fds[ numOfControllers ];
  int i, j;
  
  // 割込みハンドラ設定
  signal( SIGINT, exit_program );

  for ( i = 0; i < numOfControllers; i++ ) {
    if ( ( fd = open( devfiles[ i ], O_RDWR ) ) == -1 ) {
      fprintf( stderr, "%s: Open error\n", devfiles[ i ] );
      exit( 1 );
    }
    if ( ioctl( fd, URBTC_CONTINUOUS_READ ) < 0 ) {
      fprintf( stderr, "ioctl: URBTC_CONTINUOUS_READ error\n" );
      exit( 1 );
    }
    if ( read( fd, &ibuf, sizeof( ibuf ) ) != sizeof( ibuf ) ) {
      fprintf( stderr, "Read size mismatch.\n" );
      exit( 1 );
    }

#if __BYTE_ORDER == __BIG_ENDIAN
    ibuf.magicno = ( 0xff & ibuf.magicno ) << 8 | ( 0xff00 & ibuf.magicno ) >> 8;
#endif

    if ( ibuf.magicno == 0 ) {
      fds[ 0 ] = fd;
      fprintf( stderr, "Found controller #0.\n" );
    } else if ( ibuf.magicno == 1 ) {
      fds[ 1 ] = fd;
      fprintf( stderr, "Found controller #1.\n" );
    } else {
      fprintf( stderr, "Wrong magic no: %d.\n", ibuf.magicno );
      exit( 1 );
    }

    cmd.retval = 0 /* RETURN_VAL */;
    cmd.setoffset  = CH0 | CH1 | CH2 | CH3;
    cmd.setcounter = CH0 | CH1 | CH2 | CH3;
    cmd.resetint   = CH0 | CH1 | CH2 | CH3;

    //★とくに重要
    cmd.selin = CH0 | SET_SELECT; /* AD in:ch0  ENC in:ch1,ch2,ch3*/
    //★とくに重要
    cmd.selout = SET_SELECT | CH0 | CH1 | CH2 | CH3; /*  PWM out:ch0,ch1,ch2,ch3*/

#if __BYTE_ORDER == __LITTLE_ENDIAN
    cmd.offset[ 0 ] = cmd.offset[ 1 ] = cmd.offset[ 2 ] = cmd.offset[ 3 ] = 0x7fff;
    cmd.counter[ 0 ] = cmd.counter[ 1 ] = cmd.counter[ 2 ] = cmd.counter[ 3 ] = 0;
#else
    cmd.offset[ 0 ] = cmd.offset[ 1 ] = cmd.offset[ 2 ] = cmd.offset[ 3 ] = 0xff7f;
    cmd.counter[ 0 ] = cmd.counter[ 1 ] = cmd.counter[ 2 ] = cmd.counter[ 3 ] = 0;
#endif

    cmd.posneg = SET_POSNEG | CH0 | CH1 | CH2 | CH3; /*POS PWM out*/
    cmd.breaks = SET_BREAKS | CH0 | CH1 | CH2 | CH3; /*No Brake*/

    if ( ioctl( fd, URBTC_COUNTER_SET ) < 0 ) {
      fprintf( stderr, "ioctl: URBTC_COUNTER_SET error\n" );
      exit( 1 );
    }
    if ( write( fd, &cmd, sizeof( cmd ) ) < 0 ) {
      fprintf( stderr, "write error\n" );
      exit( 1 );
    }
    if ( ioctl( fd, URBTC_DESIRE_SET ) < 0 ) {
      fprintf( stderr, "ioctl: URBTC_DESIRE_SET error\n" );
      exit( 1 );
    }
  }

  for ( j = 0; j < numOfControllers; j++ ) {
    for ( i = 0; i < 4; i++ ) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
      obuf[ j ].ch[ i ].x = 0;
      obuf[ j ].ch[ i ].d = 0;
      obuf[ j ].ch[ i ].kp = 0;
      obuf[ j ].ch[ i ].kpx = 1;
      obuf[ j ].ch[ i ].kd = 0;
      obuf[ j ].ch[ i ].kdx = 1;
      obuf[ j ].ch[ i ].ki = 0;
      obuf[ j ].ch[ i ].kix = 1;
#else
      obuf[ j ].ch[ i ].x = 0;
      obuf[ j ].ch[ i ].d = 0;
      obuf[ j ].ch[ i ].kp = 0;
      obuf[ j ].ch[ i ].kpx = 0x0100;
      obuf[ j ].ch[ i ].kd = 0;
      obuf[ j ].ch[ i ].kdx = 0x0100;
      obuf[ j ].ch[ i ].ki = 0;
      obuf[ j ].ch[ i ].kix = 0x0100;
#endif
    }
#if __BYTE_ORDER == __LITTLE_ENDIAN
    obuf[ j ].ch[ 1 ].kp = 20;
    obuf[ j ].ch[ 2 ].kp = 20;
    obuf[ j ].ch[ 3 ].kp = 20;
    obuf[ j ].ch[ 1 ].kd = 10;
    obuf[ j ].ch[ 2 ].kd = 10;
    obuf[ j ].ch[ 3 ].kd = 10;
#else
    obuf[ j ].ch[ 1 ].kp = 0x1000;
    obuf[ j ].ch[ 2 ].kp = 0x1000;
    obuf[ j ].ch[ 3 ].kp = 0x1000;
#endif
  }


  // ブロック始まり
  {
    unsigned short a = 0;
    short b = 0;


    quit_flag = 1;
    
    // ・a は符号なしshort( 0～65535 )
    // ・モータはサインカーブに変化する目標値にしたがって回転する
    // ※81(84)行目によって cmd.counter(現在のカウンタ値)は0にセットされている
    //
    // 目標値をXにセット
    //   現在のカウンタ値がYとするなら
    //   -> X-Y > 0 の場合は正転（すすむ）
    //   -> X-Y < 0 の場合は逆転（もどる）
    // ※モータの回転スピードの制御にマニュアル4頁目の式(1)が用いられている
    // ※  ->目標値に近づくとゆっくり回るようになる。目標値を越えてしまったら逆回転して戻る etc...
    // ※  ->このような制御が、目標値を設定するだけで自動的に行われる(かしこいコントローラ)。
    //
    // aは unsigned shortだけれど、目標値やカウンタ値の型はshort( -32768～32767 )であることに注意。
    // ※ 目標値を50000などと指定すると、32767の次は-32768となってしまうので
    //    目標値に達することが無くモータが止まらないということになる（はず）。
    //
    // 下記 whileループでは
    //   モータ回転のイメージ(目標値正)：_/\__/\__/\__/\__/\__/
    //   モータ回転のイメージ(目標値負)：
    // となります。
    //
    // ch0には目標値出力していないので、ch0にモータをつないでも動きません。
    // 75行目でch0にはエンコーダ仕様の設定をしていません。
    // したがって、エンコーダ制御でモータを動作させられるのは ch1, ch2, ch3の３つ!!!
    // となる。
    i = 0;
    while ( quit_flag ) { // ctl+C でループを抜ける
      a = 300.0 * sin( (i/2)*3.14/655.360 );
      a <<= 5;
      for ( j=0; j < numOfControllers; j++ ) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
        obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = a;
        printf("little %d %d\r\n",a, obuf[j].ch[3].x);
#else
        obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = ( ( a & 0xff ) << 8 | ( a & 0xff00 ) >> 8 );
        printf( "big %d %d\r\n", a, obuf[ j ].ch[ 3 ].x );
#endif
        if (write( fds[ j ], &obuf[ j ], sizeof( obuf[ j ] ) ) > 0 ) {
          i++;
        } else {
          printf( "write err\n" );
          break;
        }
      }
      sleep( 0.1 );
    }


    quit_flag = 1;
    
    // ・bは符号つきshort( -32768～32767 )
    //
    // 下記 whileループでは
    //   モータ回転のイメージ（目標値正）：/\  /\  /\  /\  /\
    //   モータ回転のイメージ（目標値負）：  \/  \/  \/  \/  \/
    // となります。
    i = 0;
    while ( quit_flag ) { // ctl+C でループを抜ける
      b = 300.0*sin( (i/2)*3.14/655.360 );
      b <<= 5;
      for ( j=0; j < numOfControllers; j++ ) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
        obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = b;
        printf( "little %d %d\r\n",b, obuf[ j ].ch[ 3 ].x );
#else
        obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = ( ( b & 0xff ) << 8 | ( b & 0xff00 ) >> 8 );
        printf( "big %d %d\r\n", b, obuf[ j ].ch[ 3 ].x );
#endif
        if ( write( fds[ j ], &obuf[ j ], sizeof( obuf[ j ] ) ) > 0 ) {
          i++;
        } else {
          printf( "write err\n" );
          break;
        }
      }
      sleep( 0.1 );
    }


    quit_flag = 1;
    
    // 目標値を０にセットする＝元の位置に戻る
    b = 0;
    b <<= 5;
    for ( j = 0; j < numOfControllers; j++ ) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
      obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = b;
      printf( "little %d %d\r\n", b, obuf[ j ].ch[ 3 ].x );
#else
      obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = ( ( b & 0xff ) << 8 | ( b & 0xff00 ) >> 8 );
      printf( "big %d %d\r\n", b, obuf[ j ].ch[ 3 ].x );
#endif
      if ( write( fds[ j ], &obuf[ j ], sizeof( obuf[ j ] ) ) > 0 ) {
        i++;
      } else {
        printf( "write err\n" );
        break;
      }
    }
    while ( quit_flag ); // ctl+C でループを抜ける


    quit_flag = 1;
    
    // 正転3回転
    // エンコーダのカンタ値が目標値(3回転)に達した時点で回転は止まる
    b = 3*ROT;
    b <<= 5;
    for ( j = 0; j < numOfControllers; j++ ) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
      obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = b;
      printf( "little %d %d\r\n", b, obuf[ j ].ch[ 3 ].x);
#else
      obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = ( ( b & 0xff ) << 8 | ( b & 0xff00 ) >> 8 );
      printf( "big %d %d\r\n", b, obuf[ j ].ch[ 3 ].x );
#endif
      if ( write( fds[ j ], &obuf[ j ], sizeof( obuf[ j ] ) ) > 0 ) {
        i++;
      } else {
        printf( "write err\n" );
        break;
      }
    }
    while ( quit_flag ); // ctl+C でループを抜ける


    // ★★★★★
    // エンコーダのカウンタ値をリセットする
    // ここがモータ回転の新たな基準位置となる
    // ★★★★★
    cmd.counter[ 1 ] = cmd.counter[ 2 ] = cmd.counter[ 3 ] = 0;
    if ( ioctl( fd, URBTC_COUNTER_SET ) < 0 ){
      fprintf( stderr, "ioctl: URBTC_COUNTER_SET error\n" );
      exit( 1 );
    }
    if ( write( fd, &cmd, sizeof( cmd ) ) < 0 ) {
      fprintf( stderr, "write error\n" );
      exit( 1 );
    }
    if ( ioctl( fd, URBTC_DESIRE_SET ) < 0 ){
      fprintf( stderr, "ioctl: URBTC_DESIRE_SET error\n" );
      exit( 1 );
    }


    quit_flag = 1;
    
    // 逆転2回転
    b = -2*ROT;
    b <<= 5;
    for ( j = 0; j < numOfControllers; j++ ) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
      obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = b;
      printf( "little %d %d\r\n", b, obuf[ j ].ch[ 3 ].x);
#else
      obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = ( ( b & 0xff ) << 8 | ( b & 0xff00 ) >> 8 );
      printf( "big %d %d\r\n", b, obuf[ j ].ch[ 3 ].x );
#endif
      if ( write( fds[ j ], &obuf[ j ], sizeof( obuf[ j ] ) ) > 0 ) {
        i++;
      } else {
        printf( "write err\n" );
        break;
      }
    }
    while ( quit_flag ); // ctl+C でループを抜ける


    quit_flag = 1;
    
    // 目標値を０にセットする＝元の位置に戻る（正転2回転）
    // 元の位置に戻ったけど、トータルで考えるとモータ3回転分進んだ位置で止まっていることになる。
    b = 0;
    b <<= 5;
    for ( j = 0; j < numOfControllers; j++ ) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
      obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = b;
      printf( "little %d %d\r\n", b, obuf[ j ].ch[ 3 ].x );
#else
      obuf[ j ].ch[ 1 ].x = obuf[ j ].ch[ 2 ].x = obuf[ j ].ch[ 3 ].x = ( ( b & 0xff ) << 8 | ( b & 0xff00 ) >> 8 );
      printf( "big %d %d\r\n", b, obuf[ j ].ch[ 3 ].x );
#endif
      if ( write( fds[ j ], &obuf[ j ], sizeof( obuf[ j ] ) ) > 0 ) {
        i++;
      } else {
        printf("write err\n");
        break;
      }
    }
    while ( quit_flag ); // ctl+C でループを抜ける

    quit_flag = 1;


  }
  // ブロック終わり


  for ( i = 0; i < numOfControllers; i++ )
    close( fds[ i ] );

  return 0;
}

void exit_program( int sig, int code, struct sigcontext *scp, char *addr ) {
  quit_flag = 0;
  fprintf( stderr, "kill signal is received\n" );
}
