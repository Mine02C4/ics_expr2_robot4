#ifndef I2C_SENSOR_INTERFACE_H_
#define I2C_SENSOR_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Definitions */
enum sensor { S1, S2 }; // TODO: 設計が固まったら意味のある名前にする。

/* Init API */
extern void sensor_init();

/* Sensor API */
extern int get_distance(enum sensor); // TODO: 処理待ちの都合上一気に取ったほうが良い

/* Finalize API */
extern void sensor_finalize();

#ifdef __cplusplus
}
#endif

#endif  // I2C_SENSOR_INTERFACE_H_

