#ifndef GUN_TURRET_INTERFACE_H_
#define GUN_TURRET_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Definitions */

/* Init API */
extern void turret_init();

/* Turret API */
extern void open_fire(int n_bullets);
extern void turn_by_degrees(int degrees);
extern void elevate_by_degrees(int degrees);

extern void burst_fire(int n_bullets);

/* Finalize API */
extern void turret_finalize();

#ifdef __cplusplus
}
#endif

#endif  // GUN_TURRET_INTERFACE_H_

