#include "Gun.hpp"

#include "../gun_turret/interface.h"

void Gun::Init()
{
  turret_init();
}

void Gun::Finalize()
{
  turret_finalize();
}

void Gun::FireSingle()
{
	open_fire(1);
}

void Gun::FireNum(int numbullets)
{
	open_fire(numbullets);
}

void Gun::FirstBurst(int ntimes)
{
	burst_fire(ntimes);
}

void Gun::TurretUp()
{
  elevate_by_degrees(1);
}
void Gun::TurretDown()
{
  elevate_by_degrees(-1);
}


Gun::Gun()
{
}
