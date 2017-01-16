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
