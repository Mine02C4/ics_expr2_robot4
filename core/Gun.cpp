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
