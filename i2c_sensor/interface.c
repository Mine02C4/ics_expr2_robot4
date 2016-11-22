#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define SENSOR_NUM 2

const static char *i2cdev = "/dev/i2c-1";
static int fd[SENSOR_NUM];

void sensor_init()
{
  int i;
  for (i = 0; i < SENSOR_NUM; i++) {
    fd[i] = open(i2cdev, O_RDWR);
    if (fd[i] < 0) {
      fprintf(stderr, "Error: cannot open I2C device\n");
      exit(1);
    }
    char dev_addr = (0xE0 + 0x2 * i) >> 1;
    if (ioctl(fd[i], I2C_SLAVE, dev_addr) < 0) {
      fprintf(stderr, "Error: cannot select device 0x%x\n", dev_addr);
      exit(1);
    }
  }
}

int get_distance(enum sensor s)
{
  int id, i;
  switch (s) {
    case S1:
      id = 0;
      break;
    case S2:
      id = 1;
      break;
    default:
      fprintf(stderr, "Error: Unknown distance sensor.\n");
      exit(1);
  }
  char buf[10];
  // コマンドレジスタ0に 0x51:Real Ranging Mode - Result in centimeters を送ることによって測距が始まる
  buf[0] = 0x00;
  buf[1] = 0x51;
  for (i = 2; i < 10; i++) {
    buf[i] = 0x00;
  }
  if ((write(fd[id], buf, 2)) != 2) {
    fprintf(stderr, "Error: Error send the read command. Sensor = %d\n", id);
    exit(1);
  }
  // 音波だから、"行って" "帰ってくる" のに時間がかかる
  usleep(66000);
  int range = 0;
  // コマンドレジスタ2に対し測距値データの上位バイトをリクエスト
  buf[0] = 0x02;
  if ((write(fd[i], buf, 1)) != 1){
    fprintf(stderr, "Error: Error on select the range high byte. Sensor = %d\n", id);
    exit(1);
  }
  if ((read(fd[i], buf, 1)) != 1){
    fprintf(stderr, "Error: Error on read the range high byte. Sensor = %d\n", id);
    exit(1);
  }
  range = buf[0] << 8;

  // コマンドレジスタ3に対し測距値データの下位バイトをリクエスト
  buf[0] = 0x03;
  if ((write(fd[i], buf, 1)) != 1){
    fprintf(stderr, "Error: Error on select the range low byte. Sensor = %d\n", id);
    exit(1);
  }
  if ((read(fd[i], buf, 1)) != 1){
    fprintf(stderr, "Error: Error on read the range low byte. Sensor = %d\n", id);
    exit(1);
  }
  range |= buf[0];
  return range;
}

void
sensor_finalize()
{
  int i;
  for (i = 0; i < SENSOR_NUM; i++) {
    close(fd[i]);
  }
}

