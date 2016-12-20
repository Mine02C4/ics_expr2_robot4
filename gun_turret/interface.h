#ifndef GUN_TURRET_INTERFACE_H_
#define GUN_TURRET_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Definitions */

/* Init API */
extern void turret_init();

/* Turret API */
extern void open_fire(int);

/* Finalize API */
extern void turret_finalize();

#ifdef __cplusplus
}
#endif

#endif  // GUN_TURRET_INTERFACE_H_
